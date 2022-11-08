/**
 * Copyright 2022 lebai.ltd
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
*/

#pragma once

#include "websocket.hh"
#include <rapidjson/document.h>
#include <atomic>

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
    int CallString(int id, const std::string &method, const std::string & req_str, std::string * resp_str);
    // /**
    // int CallRpc(const std::string & req_str, std::string * resp_str);
    int CallRpc(const std::string &method, const std::string & req_data_str, std::string * resp_data_str);
    ConnectionStatus GetConnectionStatus();
  protected:
    
    WebSocketEndPoint endpoint_;
    int id_;
    std::atomic<int> jsonrpc_id_;
    // int jsonrpc_id_ = 0;
  };

};