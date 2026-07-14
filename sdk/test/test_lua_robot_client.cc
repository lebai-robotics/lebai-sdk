#include <gtest/gtest.h>

#include <asio.hpp>

#include <atomic>
#include <chrono>
#include <condition_variable>
#include <cstdint>
#include <exception>
#include <functional>
#include <future>
#include <memory>
#include <mutex>
#include <stdexcept>
#include <string>
#include <thread>

#include "lua_robot_impl.hh"

namespace {

using Client = lebai::l_master::LuaRobot::LuaRobotImpl;
using asio::ip::tcp;
using namespace std::chrono_literals;

constexpr auto kClientTimeout = 500ms;
constexpr auto kServerWatchdog = 1s;

std::string capture_runtime_error(const std::function<void()>& operation) {
  try {
    operation();
    ADD_FAILURE() << "Expected std::runtime_error";
  } catch (const std::runtime_error& error) {
    return error.what();
  } catch (const std::exception& error) {
    ADD_FAILURE() << "Expected std::runtime_error, got: " << error.what();
  } catch (...) {
    ADD_FAILURE()
        << "Expected std::runtime_error, got a non-standard exception";
  }
  return {};
}

std::string capture_invalid_argument(const std::function<void()>& operation) {
  try {
    operation();
    ADD_FAILURE() << "Expected std::invalid_argument";
  } catch (const std::invalid_argument& error) {
    return error.what();
  } catch (const std::exception& error) {
    ADD_FAILURE() << "Expected std::invalid_argument, got: " << error.what();
  } catch (...) {
    ADD_FAILURE()
        << "Expected std::invalid_argument, got a non-standard exception";
  }
  return {};
}

void expect_message_contains(const std::string& message,
                             const std::string& expected) {
  EXPECT_NE(std::string::npos, message.find(expected))
      << "message: " << message;
}

void read_exact_request(tcp::socket& socket, const std::string& expected) {
  std::string actual(expected.size(), '\0');
  std::error_code error;
  const auto bytes = asio::read(socket, asio::buffer(actual), error);
  if (error) {
    throw std::runtime_error("server read failed: " + error.message());
  }
  if (bytes != expected.size() || actual != expected) {
    throw std::runtime_error("server received unexpected request: " + actual);
  }
}

void write_all(tcp::socket& socket, const std::string& data) {
  std::error_code error;
  asio::write(socket, asio::buffer(data), error);
  if (error) {
    throw std::runtime_error("server write failed: " + error.message());
  }
}

class LoopbackServer {
 public:
  using Handler = std::function<void(tcp::socket&)>;

  explicit LoopbackServer(Handler handler) : acceptor_(io_), peer_(io_) {
    std::error_code error;
    acceptor_.open(tcp::v4(), error);
    if (error) {
      throw std::runtime_error("server open failed: " + error.message());
    }
    acceptor_.set_option(tcp::acceptor::reuse_address(false), error);
    if (error) {
      throw std::runtime_error("server option failed: " + error.message());
    }
    acceptor_.bind({asio::ip::address_v4::loopback(), 0}, error);
    if (error) {
      throw std::runtime_error("server bind failed: " + error.message());
    }
    acceptor_.listen(asio::socket_base::max_listen_connections, error);
    if (error) {
      throw std::runtime_error("server listen failed: " + error.message());
    }
    port_ = acceptor_.local_endpoint().port();

    thread_ = std::thread([this, handler = std::move(handler)] {
      try {
        std::error_code error;
        acceptor_.accept(peer_, error);
        if (error) {
          if (!stopping_) {
            throw std::runtime_error("server accept failed: " +
                                     error.message());
          }
        } else {
          accepted_ = true;
          handler(peer_);
        }
      } catch (const std::exception& error) {
        std::lock_guard<std::mutex> lock(mutex_);
        server_error_ = error.what();
      } catch (...) {
        std::lock_guard<std::mutex> lock(mutex_);
        server_error_ = "non-standard server exception";
      }
      std::error_code ignored;
      peer_.close(ignored);
      {
        std::lock_guard<std::mutex> lock(mutex_);
        finished_ = true;
      }
      finished_condition_.notify_all();
    });
  }

  LoopbackServer(const LoopbackServer&) = delete;
  LoopbackServer& operator=(const LoopbackServer&) = delete;

  ~LoopbackServer() { stop_and_join(); }

  uint16_t port() const { return port_; }

  std::string finish() {
    {
      std::unique_lock<std::mutex> lock(mutex_);
      finished_condition_.wait_for(lock, 200ms, [this] { return finished_; });
    }
    stop_and_join();
    std::lock_guard<std::mutex> lock(mutex_);
    return server_error_;
  }

 private:
  void stop_and_join() noexcept {
    if (!thread_.joinable()) {
      return;
    }

    stopping_ = true;

    // A loopback connection releases a blocking accept without relying on
    // platform-specific cross-thread acceptor cancellation behavior.
    if (!accepted_) {
      try {
        asio::io_context escape_io;
        tcp::socket escape(escape_io);
        std::error_code ignored;
        escape.connect({asio::ip::address_v4::loopback(), port_}, ignored);
        escape.close(ignored);
      } catch (...) {
      }
    }

    std::error_code ignored;
    peer_.close(ignored);
    acceptor_.close(ignored);
    thread_.join();
  }

  asio::io_context io_;
  tcp::acceptor acceptor_;
  tcp::socket peer_;
  uint16_t port_{};
  std::thread thread_;
  std::atomic<bool> accepted_{false};
  std::atomic<bool> stopping_{false};
  std::mutex mutex_;
  std::condition_variable finished_condition_;
  bool finished_{};
  std::string server_error_;
};

Client::Config client_config(uint16_t port,
                             std::size_t max_response_bytes = 64) {
  Client::Config config;
  config.port = port;
  config.timeout = kClientTimeout;
  config.max_response_bytes = max_response_bytes;
  return config;
}

std::unique_ptr<Client> connect_client(
    uint16_t port, std::size_t max_response_bytes = 64,
    std::chrono::milliseconds timeout = kClientTimeout) {
  try {
    auto config = client_config(port, max_response_bytes);
    config.timeout = timeout;
    return std::make_unique<Client>("127.0.0.1", config);
  } catch (const std::exception& error) {
    ADD_FAILURE() << "Client construction failed: " << error.what();
    return nullptr;
  }
}

TEST(LuaRobotClientTest, ConstructionFailsWhenLoopbackPortDoesNotListen) {
  asio::io_context io;
  tcp::acceptor reserved(io);
  std::error_code error;
  reserved.open(tcp::v4(), error);
  ASSERT_FALSE(error);
  reserved.set_option(tcp::acceptor::reuse_address(false), error);
  ASSERT_FALSE(error);
  reserved.bind({asio::ip::address_v4::loopback(), 0}, error);
  ASSERT_FALSE(error);

  const auto message = capture_runtime_error([&] {
    Client client("127.0.0.1", client_config(reserved.local_endpoint().port()));
  });

  expect_message_contains(message, "connect");
}

TEST(LuaRobotClientTest, ReadsResponseFragmentedBeforeCrLf) {
  std::promise<void> first_fragment_sent;
  auto first_fragment = first_fragment_sent.get_future();
  std::promise<void> release_terminator;
  auto release = release_terminator.get_future().share();

  LoopbackServer server([&](tcp::socket& socket) {
    read_exact_request(socket, "print(fragmented())");
    write_all(socket, "abc");
    first_fragment_sent.set_value();
    release.wait_for(kServerWatchdog);
    write_all(socket, "\r\n");
  });
  auto client = connect_client(server.port());

  std::future<std::string> response;
  if (client) {
    response = std::async(std::launch::async,
                          [&] { return client->call("fragmented()"); });
    EXPECT_EQ(std::future_status::ready, first_fragment.wait_for(300ms));
    EXPECT_EQ(std::future_status::timeout, response.wait_for(40ms));
  }

  release_terminator.set_value();
  if (response.valid()) {
    EXPECT_EQ("abc", response.get());
  }
  EXPECT_TRUE(server.finish().empty());
}

TEST(LuaRobotClientTest, ReadsControllerTabLfResponseTerminator) {
  LoopbackServer server([](tcp::socket& socket) {
    read_exact_request(socket, "print(controller_value())");
    write_all(socket, "value\t\n");
  });
  auto client = connect_client(server.port());

  if (client) {
    EXPECT_EQ("value", client->call("controller_value()"));
  }
  EXPECT_TRUE(server.finish().empty());
}

TEST(LuaRobotClientTest, PreservesCoalescedResponseForNextCall) {
  std::promise<void> release_server;
  auto release = release_server.get_future().share();
  LoopbackServer server([&](tcp::socket& socket) {
    read_exact_request(socket, "print(first())");
    write_all(socket, "one\r\ntwo\t\n");
    read_exact_request(socket, "print(second())");
    release.wait_for(kServerWatchdog);
  });
  auto client = connect_client(server.port(), 32);

  if (client) {
    EXPECT_EQ("one", client->call("first()"));
    EXPECT_EQ("two", client->call("second()"));
  }
  release_server.set_value();
  EXPECT_TRUE(server.finish().empty());
}

TEST(LuaRobotClientTest, SilentPeerReportsConfiguredTimeout) {
  std::promise<void> request_received;
  auto received = request_received.get_future();
  std::promise<void> release_server;
  auto release = release_server.get_future().share();
  LoopbackServer server([&](tcp::socket& socket) {
    read_exact_request(socket, "print(silent())");
    request_received.set_value();
    release.wait_for(kServerWatchdog);
  });
  auto client = connect_client(server.port(), 64, 100ms);

  auto started = std::chrono::steady_clock::now();
  std::string message;
  if (client) {
    message = capture_runtime_error([&] { client->call("silent()"); });
  }
  const auto elapsed = std::chrono::steady_clock::now() - started;

  release_server.set_value();
  EXPECT_EQ(std::future_status::ready, received.wait_for(300ms));
  expect_message_contains(message, "timed out");
  EXPECT_LT(elapsed, 500ms);
  EXPECT_TRUE(server.finish().empty());
}

TEST(LuaRobotClientTest, EofBeforeTerminatorReportsIncompleteResponse) {
  LoopbackServer server([](tcp::socket& socket) {
    read_exact_request(socket, "print(partial())");
    write_all(socket, "partial");
    std::error_code ignored;
    socket.shutdown(tcp::socket::shutdown_send, ignored);
  });
  auto client = connect_client(server.port());

  std::string message;
  if (client) {
    message = capture_runtime_error([&] { client->call("partial()"); });
  }

  expect_message_contains(message, "incomplete response");
  EXPECT_TRUE(server.finish().empty());
}

TEST(LuaRobotClientTest, ResponseBeyondMaximumReportsSizeError) {
  constexpr std::size_t kMaximum = 8;
  std::promise<void> release_server;
  auto release = release_server.get_future().share();
  LoopbackServer server([&](tcp::socket& socket) {
    read_exact_request(socket, "print(oversized())");
    write_all(socket, std::string(kMaximum + 1, 'x'));
    release.wait_for(kServerWatchdog);
  });
  auto client = connect_client(server.port(), kMaximum);

  std::string message;
  if (client) {
    message = capture_runtime_error([&] { client->call("oversized()"); });
  }

  release_server.set_value();
  expect_message_contains(message, "maximum response size");
  EXPECT_TRUE(server.finish().empty());
}

TEST(LuaRobotClientTest, ResponseAtMaximumIncludingCrLfSucceeds) {
  constexpr std::size_t kMaximum = 8;
  LoopbackServer server([](tcp::socket& socket) {
    read_exact_request(socket, "print(boundary())");
    write_all(socket, "123456\r\n");
  });
  auto client = connect_client(server.port(), kMaximum);

  if (client) {
    EXPECT_EQ("123456", client->call("boundary()"));
  }
  EXPECT_TRUE(server.finish().empty());
}

TEST(LuaRobotClientTest, RejectsInvalidConfiguration) {
  auto config = client_config(5180);
  config.timeout = 0ms;
  auto message =
      capture_invalid_argument([&] { Client client("127.0.0.1", config); });
  expect_message_contains(message, "timeout");

  config.timeout = kClientTimeout;
  config.max_response_bytes = 1;
  message =
      capture_invalid_argument([&] { Client client("127.0.0.1", config); });
  expect_message_contains(message, "maximum response size");
}

TEST(LuaRobotClientTest, DefaultTimeoutAllowsQueuedControllerExecution) {
  EXPECT_EQ(30s, Client::Config{}.timeout);
}

}  // namespace

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
