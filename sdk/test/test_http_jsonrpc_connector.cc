#ifdef _WIN32
#include <winsock2.h>
#else
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#endif

#include <gtest/gtest.h>

#include <httplib.h>
#include <nlohmann/json.hpp>

#include <atomic>
#include <chrono>
#include <condition_variable>
#include <future>
#include <mutex>
#include <stdexcept>
#include <string>
#include <thread>

#include "http_jsonrpc_connector.hh"

namespace {

constexpr char kSuccessResponse[] = R"({"jsonrpc":"2.0","id":1,"result":true})";

#ifdef _WIN32
using NativeSocket = SOCKET;
using SocketLength = int;
constexpr NativeSocket kInvalidSocket = INVALID_SOCKET;
#else
using NativeSocket = int;
using SocketLength = socklen_t;
constexpr NativeSocket kInvalidSocket = -1;
#endif

class LocalHttpServer {
 public:
  LocalHttpServer() {
    server_.new_task_queue = [] { return new httplib::ThreadPool(4); };
  }

  ~LocalHttpServer() { Stop(); }

  LocalHttpServer(const LocalHttpServer&) = delete;
  auto operator=(const LocalHttpServer&) -> LocalHttpServer& = delete;

  auto server() -> httplib::Server& { return server_; }
  auto port() const -> int { return port_; }

  void Start() {
    port_ = server_.bind_to_any_port("127.0.0.1");
    if (port_ <= 0) {
      throw std::runtime_error("failed to bind local HTTP server");
    }

    server_thread_ = std::thread([this] { server_.listen_after_bind(); });
    server_.wait_until_ready();
    if (!server_.is_running()) {
      Stop();
      throw std::runtime_error("failed to start local HTTP server");
    }
  }

  void Stop() {
    server_.stop();
    if (server_thread_.joinable()) {
      server_thread_.join();
    }
  }

 private:
  httplib::Server server_;
  int port_{-1};
  std::thread server_thread_;
};

class BoundNonListeningSocket {
 public:
  BoundNonListeningSocket() {
    socket_ = ::socket(AF_INET, SOCK_STREAM, 0);
    if (socket_ == kInvalidSocket) {
      throw std::runtime_error("failed to create port reservation socket");
    }

    sockaddr_in address{};
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    address.sin_port = 0;
    if (::bind(socket_, reinterpret_cast<sockaddr*>(&address),
               static_cast<SocketLength>(sizeof(address))) != 0) {
      Close();
      throw std::runtime_error("failed to bind port reservation socket");
    }

    SocketLength address_length = static_cast<SocketLength>(sizeof(address));
    if (::getsockname(socket_, reinterpret_cast<sockaddr*>(&address),
                      &address_length) != 0) {
      Close();
      throw std::runtime_error("failed to inspect port reservation socket");
    }
    port_ = ntohs(address.sin_port);
  }

  ~BoundNonListeningSocket() { Close(); }

  BoundNonListeningSocket(const BoundNonListeningSocket&) = delete;
  auto operator=(const BoundNonListeningSocket&)
      -> BoundNonListeningSocket& = delete;

  auto port() const -> int { return port_; }

 private:
  void Close() noexcept {
    if (socket_ != kInvalidSocket) {
#ifdef _WIN32
      ::closesocket(socket_);
#else
      ::close(socket_);
#endif
      socket_ = kInvalidSocket;
    }
  }

  NativeSocket socket_{kInvalidSocket};
  int port_{-1};
};

auto JsonRpcRequest(const std::string& method) -> std::string {
  return nlohmann::json{{"jsonrpc", "2.0"},
                        {"id", 1},
                        {"method", method},
                        {"params", nlohmann::json::array()}}
      .dump();
}

void SetSuccessResponse(httplib::Response& response) {
  response.status = 200;
  response.set_content(kSuccessResponse, "application/json");
}

auto ShortTimeouts() -> lebai::HttpJsonRpcTimeouts {
  lebai::HttpJsonRpcTimeouts timeouts;
  timeouts.connect = std::chrono::seconds(1);
  timeouts.read = std::chrono::milliseconds(50);
  timeouts.write = std::chrono::seconds(1);
  timeouts.wait_read = std::chrono::seconds(2);
  return timeouts;
}

void ExpectTransportError(lebai::HttpJsonRpcConnector& connector,
                          const std::string& request,
                          const std::string& expected_detail) {
  try {
    static_cast<void>(connector.Send(request));
    ADD_FAILURE() << "expected connector transport error";
  } catch (const jsonrpccxx::JsonRpcException& exception) {
    EXPECT_EQ(exception.Code(), -32003);
    EXPECT_NE(exception.Message().find(expected_detail), std::string::npos)
        << exception.Message();
  } catch (const std::exception& exception) {
    ADD_FAILURE() << "unexpected exception: " << exception.what();
  }
}

TEST(HttpJsonRpcConnectorTest, RejectsNegativeTimeoutDurations) {
  struct TimeoutField {
    const char* name;
    std::chrono::microseconds lebai::HttpJsonRpcTimeouts::*value;
  };
  const TimeoutField timeout_fields[] = {
      {"connect", &lebai::HttpJsonRpcTimeouts::connect},
      {"read", &lebai::HttpJsonRpcTimeouts::read},
      {"write", &lebai::HttpJsonRpcTimeouts::write},
      {"wait_read", &lebai::HttpJsonRpcTimeouts::wait_read},
  };

  for (const auto& field : timeout_fields) {
    SCOPED_TRACE(field.name);
    auto timeouts = ShortTimeouts();
    timeouts.*field.value = std::chrono::microseconds(-1);
    try {
      lebai::HttpJsonRpcConnector connector("127.0.0.1", 80, timeouts);
      ADD_FAILURE() << "expected negative " << field.name
                    << " timeout to be rejected";
    } catch (const std::invalid_argument& exception) {
      const std::string message = exception.what();
      EXPECT_NE(message.find(field.name), std::string::npos) << message;
      EXPECT_NE(message.find("non-negative"), std::string::npos) << message;
    } catch (const std::exception& exception) {
      ADD_FAILURE() << "unexpected exception: " << exception.what();
    }
  }
}

TEST(HttpJsonRpcConnectorTest,
     CancelReachesServerWhileWaitRequestUsesSameConnector) {
  std::mutex wait_mutex;
  std::condition_variable wait_condition;
  bool release_wait = false;
  std::promise<void> wait_arrived_promise;
  auto wait_arrived = wait_arrived_promise.get_future();
  std::promise<void> cancel_arrived_promise;
  auto cancel_arrived = cancel_arrived_promise.get_future();

  LocalHttpServer local_server;
  local_server.server().Post("/jsonrpc", [&](const httplib::Request& request,
                                             httplib::Response& response) {
    const auto method =
        nlohmann::json::parse(request.body).at("method").get<std::string>();
    if (method == "wait_task") {
      wait_arrived_promise.set_value();
      std::unique_lock<std::mutex> lock(wait_mutex);
      wait_condition.wait(lock, [&] { return release_wait; });
    } else if (method == "cancel_task") {
      cancel_arrived_promise.set_value();
    }
    SetSuccessResponse(response);
  });
  local_server.Start();

  lebai::HttpJsonRpcConnector connector("127.0.0.1", local_server.port());

  std::mutex watchdog_mutex;
  std::condition_variable watchdog_condition;
  bool finish_watchdog = false;
  std::atomic<bool> watchdog_expired{false};
  std::thread watchdog([&] {
    std::unique_lock<std::mutex> lock(watchdog_mutex);
    const bool finished_early = watchdog_condition.wait_for(
        lock, std::chrono::seconds(1), [&] { return finish_watchdog; });
    watchdog_expired.store(!finished_early);
    {
      std::lock_guard<std::mutex> wait_lock(wait_mutex);
      release_wait = true;
    }
    wait_condition.notify_all();
  });

  auto wait_request = std::async(std::launch::async, [&] {
    return connector.Send(JsonRpcRequest("wait_task"));
  });
  EXPECT_EQ(wait_arrived.wait_for(std::chrono::milliseconds(500)),
            std::future_status::ready);

  auto cancel_request = std::async(std::launch::async, [&] {
    return connector.Send(JsonRpcRequest("cancel_task"));
  });
  const auto cancel_status =
      cancel_arrived.wait_for(std::chrono::milliseconds(250));
  EXPECT_EQ(cancel_status, std::future_status::ready);

  if (cancel_status == std::future_status::ready) {
    {
      std::lock_guard<std::mutex> lock(watchdog_mutex);
      finish_watchdog = true;
    }
    watchdog_condition.notify_one();
  }
  watchdog.join();

  EXPECT_FALSE(watchdog_expired.load());
  EXPECT_EQ(cancel_arrived.wait_for(std::chrono::milliseconds(500)),
            std::future_status::ready);

  std::string wait_response;
  std::string cancel_response;
  EXPECT_NO_THROW(wait_response = wait_request.get());
  EXPECT_NO_THROW(cancel_response = cancel_request.get());
  EXPECT_EQ(wait_response, kSuccessResponse);
  EXPECT_EQ(cancel_response, kSuccessResponse);

  local_server.Stop();
}

TEST(HttpJsonRpcConnectorTest, OrdinaryRequestUsesInjectedReadTimeout) {
  LocalHttpServer local_server;
  local_server.server().Post(
      "/jsonrpc", [](const httplib::Request&, httplib::Response& response) {
        std::this_thread::sleep_for(std::chrono::milliseconds(150));
        SetSuccessResponse(response);
      });
  local_server.Start();

  lebai::HttpJsonRpcConnector connector("127.0.0.1", local_server.port(),
                                        ShortTimeouts());
  ExpectTransportError(connector, JsonRpcRequest("get_robot_state"),
                       "error=" + httplib::to_string(httplib::Error::Read));

  local_server.Stop();
}

TEST(HttpJsonRpcConnectorTest, WaitMethodsUseInjectedWaitReadTimeout) {
  LocalHttpServer local_server;
  local_server.server().Post(
      "/jsonrpc", [](const httplib::Request&, httplib::Response& response) {
        std::this_thread::sleep_for(std::chrono::milliseconds(150));
        SetSuccessResponse(response);
      });
  local_server.Start();

  lebai::HttpJsonRpcConnector connector("127.0.0.1", local_server.port(),
                                        ShortTimeouts());
  EXPECT_EQ(connector.Send(JsonRpcRequest("wait_signal")), kSuccessResponse);
  EXPECT_EQ(connector.Send(JsonRpcRequest("wait_claw_ai")), kSuccessResponse);

  local_server.Stop();
}

TEST(HttpJsonRpcConnectorTest, MalformedJsonUsesOrdinaryReadTimeout) {
  LocalHttpServer local_server;
  local_server.server().Post(
      "/jsonrpc", [](const httplib::Request&, httplib::Response& response) {
        std::this_thread::sleep_for(std::chrono::milliseconds(150));
        SetSuccessResponse(response);
      });
  local_server.Start();

  lebai::HttpJsonRpcConnector connector("127.0.0.1", local_server.port(),
                                        ShortTimeouts());
  ExpectTransportError(connector, "{malformed-json",
                       "error=" + httplib::to_string(httplib::Error::Read));

  local_server.Stop();
}

TEST(HttpJsonRpcConnectorTest, HttpErrorIncludesStatusAndBody) {
  LocalHttpServer local_server;
  local_server.server().Post(
      "/jsonrpc", [](const httplib::Request&, httplib::Response& response) {
        response.status = 503;
        response.set_content("controller unavailable", "text/plain");
      });
  local_server.Start();

  lebai::HttpJsonRpcConnector connector("127.0.0.1", local_server.port(),
                                        ShortTimeouts());
  try {
    static_cast<void>(connector.Send(JsonRpcRequest("get_robot_state")));
    ADD_FAILURE() << "expected HTTP status error";
  } catch (const jsonrpccxx::JsonRpcException& exception) {
    EXPECT_EQ(exception.Code(), -32003);
    EXPECT_NE(exception.Message().find("status=503"), std::string::npos);
    EXPECT_NE(exception.Message().find("body=controller unavailable"),
              std::string::npos);
  } catch (const std::exception& exception) {
    ADD_FAILURE() << "unexpected exception: " << exception.what();
  }

  local_server.Stop();
}

TEST(HttpJsonRpcConnectorTest, ConnectionErrorIncludesHttplibDetail) {
  BoundNonListeningSocket port_reservation;
  lebai::HttpJsonRpcConnector connector("127.0.0.1", port_reservation.port(),
                                        ShortTimeouts());
  ExpectTransportError(
      connector, JsonRpcRequest("get_robot_state"),
      "error=" + httplib::to_string(httplib::Error::Connection));
}

}  // namespace

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
