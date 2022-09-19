#pragma once

#include "websocket.hh"
#include <rapidjson/document.h>

namespace lebai
{
  class JSONRpcConnector
  {
  public:
    enum ConnectionStatus
    {
      kConnecting,
      kOpen,
      kClosed,
      kFailed,
    };
    JSONRpcConnector(const std::string &ip, uint16_t port);
    virtual ~JSONRpcConnector();
    int Call(const std::string &method, rapidjson::Value &req_data, rapidjson::Value &resp_data);
    int CallString(const std::string &method, const std::string & req_str, std::string * resp_str);
    int CallRpc(const std::string & req_str, std::string * resp_str);

  protected:
    ConnectionStatus GetConnectionStatus();
    WebSocketEndPoint endpoint_;
    int id_;
    int jsonrpc_id_ = 0;
  };

};