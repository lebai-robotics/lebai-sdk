#pragma once

#include <cstdlib>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <mutex>
#include <condition_variable>
#define ASIO_STANDALONE
// #define _WEBSOCKETPP_CPP11_TYPE_TRAITS_
// #ifndef _WEBSOCKETPP_CPP11_STL_
// #define _WEBSOCKETPP_CPP11_STL_
// #endif
#include <websocketpp/client.hpp>
#include <websocketpp/common/memory.hpp>
#include <websocketpp/common/thread.hpp>
#include <websocketpp/config/asio_no_tls_client.hpp>

namespace lebai
{
  typedef websocketpp::client<websocketpp::config::asio_client> WSClient;

  class ConnectionMetadata
  {
  public:
    using ptr = websocketpp::lib::shared_ptr<ConnectionMetadata>;

    ConnectionMetadata(int id, websocketpp::connection_hdl hdl, std::string uri)
        : id_(id),
          hdl_(hdl),
          status_("Connecting"),
          uri_(uri),
          server_("N/A") {}

    void onOpen(WSClient *c, websocketpp::connection_hdl hdl)
    {
      status_ = "Open";

      WSClient::connection_ptr con = c->get_con_from_hdl(hdl);
      server_ = con->get_response_header("Server");
    }

    void onFail(WSClient *c, websocketpp::connection_hdl hdl)
    {
      status_ = "Failed";
      WSClient::connection_ptr con = c->get_con_from_hdl(hdl);
      server_ = con->get_response_header("Server");
      error_reason_ = con->get_ec().message();
    }

    void onClose(WSClient *c, websocketpp::connection_hdl hdl)
    {
      status_ = "Closed";
      WSClient::connection_ptr con = c->get_con_from_hdl(hdl);
      std::stringstream s;
      s << "close code: " << con->get_remote_close_code() << " ("
        << websocketpp::close::status::get_string(con->get_remote_close_code())
        << "), close reason: " << con->get_remote_close_reason();
      error_reason_ = s.str();
    }

    void onMessage(websocketpp::connection_hdl, WSClient::message_ptr msg)
    {
      std::unique_lock<std::mutex> lock(mutex_);
      cv_.wait(lock, [&]
               { return req_flag_; });
      message_str_ = msg->get_payload();
      lock.unlock();
      resp_flag_ = true;
      cv_.notify_one();
    }

    websocketpp::connection_hdl getHDL() const { return hdl_; }

    int getID() const { return id_; }

    std::string getStatus() const { return status_; }

    // void record_sent_message(std::string message) {
    //   m_messages.push_back(">> " + message);
    // }

    void notify()
    {
      cv_.notify_one();
    }
    void waitCV()
    {
      std::unique_lock<std::mutex> lock(mutex_);
      cv_.wait(lock, [&]
               { return resp_flag_; });
    }

    void SetReqFlag(bool flag)
    {
      req_flag_ = flag;
    }
    void SetRespFlag(bool flag)
    {
      resp_flag_ = flag;
    }

    const std::string &GetMessageStr()
    {
      return message_str_;
    }

    // friend std::ostream& operator<<(std::ostream& out,
    //                                 ConnectionMetadata const& data);

  private:
    int id_;
    websocketpp::connection_hdl hdl_;
    std::string status_;
    std::string uri_;
    std::string server_;
    std::string error_reason_;
    // std::vector<std::string> m_messages;
    std::mutex mutex_;
    std::condition_variable cv_;
    bool req_flag_ = false;
    bool resp_flag_ = false;
    std::string message_str_;
  };

  // std::ostream& operator<<(std::ostream& out, ConnectionMetadata const& data) {
  //   out << "> URI: " << data.m_uri << "\n"
  //       << "> Status: " << data.m_status << "\n"
  //       << "> Remote Server: "
  //       << (data.m_server.empty() ? "None Specified" : data.m_server) << "\n"
  //       << "> Error/close reason: "
  //       << (data.m_error_reason.empty() ? "N/A" : data.m_error_reason) << "\n";
  //   out << "> Messages Processed: (" << data.m_messages.size() << ") \n";

  //   std::vector<std::string>::const_iterator it;
  //   for (it = data.m_messages.begin(); it != data.m_messages.end(); ++it) {
  //     out << *it << "\n";
  //   }

  //   return out;
  // }

  class WebSocketEndPoint
  {
  public:
    WebSocketEndPoint() : next_id_(0)
    {
      endpoint_.clear_access_channels(websocketpp::log::alevel::all);
      endpoint_.clear_error_channels(websocketpp::log::elevel::all);

      endpoint_.init_asio();
      endpoint_.start_perpetual();

      thread_ = websocketpp::lib::make_shared<websocketpp::lib::thread>(
          &WSClient::run, &endpoint_);
    }

    ~WebSocketEndPoint()
    {
      endpoint_.stop_perpetual();

      for (ConList::const_iterator it = connection_list_.begin();
           it != connection_list_.end(); ++it)
      {
        if (it->second->getStatus() != "Open")
        {
          // Only close open connections
          continue;
        }

        websocketpp::lib::error_code ec;
        endpoint_.close(it->second->getHDL(),
                        websocketpp::close::status::going_away, "", ec);
        if (ec)
        {
          std::cout << "> Error closing connection " << it->second->getID()
                    << ": " << ec.message() << std::endl;
        }
      }
      thread_->join();
    }

    int connect(std::string const &uri)
    {
      websocketpp::lib::error_code ec;

      WSClient::connection_ptr con = endpoint_.get_connection(uri, ec);

      if (ec)
      {
        std::cout << "> Connect initialization error: " << ec.message()
                  << std::endl;
        return -1;
      }

      int new_id = next_id_++;
      ConnectionMetadata::ptr metadata_ptr =
          websocketpp::lib::make_shared<ConnectionMetadata>(
              new_id, con->get_handle(), uri);
      connection_list_[new_id] = metadata_ptr;

      con->set_open_handler(websocketpp::lib::bind(
          &ConnectionMetadata::onOpen, metadata_ptr, &endpoint_,
          websocketpp::lib::placeholders::_1));
      con->set_fail_handler(websocketpp::lib::bind(
          &ConnectionMetadata::onFail, metadata_ptr, &endpoint_,
          websocketpp::lib::placeholders::_1));
      con->set_close_handler(websocketpp::lib::bind(
          &ConnectionMetadata::onClose, metadata_ptr, &endpoint_,
          websocketpp::lib::placeholders::_1));
      con->set_message_handler(
          websocketpp::lib::bind(&ConnectionMetadata::onMessage, metadata_ptr,
                                 websocketpp::lib::placeholders::_1,
                                 websocketpp::lib::placeholders::_2));

      endpoint_.connect(con);
      return new_id;
    }

    void close(int id, websocketpp::close::status::value code,
               std::string reason)
    {
      websocketpp::lib::error_code ec;

      ConList::iterator metadata_it = connection_list_.find(id);
      if (metadata_it == connection_list_.end())
      {
        std::cout << "> No connection found with id " << id << std::endl;
        return;
      }

      endpoint_.close(metadata_it->second->getHDL(), code, reason, ec);
      if (ec)
      {
        std::cout << "> Error initiating close: " << ec.message() << std::endl;
      }
    }

    void send(int id, std::string message)
    {

      websocketpp::lib::error_code ec;

      ConList::iterator metadata_it = connection_list_.find(id);
      if (metadata_it == connection_list_.end())
      {
        std::cout << "> No connection found with id " << id << std::endl;
        return;
      }

      endpoint_.send(metadata_it->second->getHDL(), message,
                     websocketpp::frame::opcode::text, ec);

      if (ec)
      {
        std::cout << "> Error sending message: " << ec.message() << std::endl;
        return;
      }
      metadata_it->second->SetReqFlag(true);
      metadata_it->second->notify();
      metadata_it->second->waitCV();
      metadata_it->second->SetReqFlag(false);
      metadata_it->second->SetRespFlag(false);

      // std::unique_lock<std::mutex> lock(mutex);
      // cv.wait(lock, [] { return processed; });
      // metadata_it->second->record_sent_message(message);
    }

    ConnectionMetadata::ptr get_metadata(int id) const
    {
      ConList::const_iterator metadata_it = connection_list_.find(id);
      if (metadata_it == connection_list_.end())
      {
        return ConnectionMetadata::ptr();
      }
      else
      {
        return metadata_it->second;
      }
    }

  private:
    typedef std::map<int, ConnectionMetadata::ptr> ConList;

    WSClient endpoint_;
    websocketpp::lib::shared_ptr<websocketpp::lib::thread> thread_;

    ConList connection_list_;
    int next_id_;
  };

}