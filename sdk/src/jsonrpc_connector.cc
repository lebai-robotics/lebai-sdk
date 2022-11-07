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

#include "jsonrpc_connector.hh"

#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>
#include "protos/utils.hh"

namespace lebai
{

JSONRpcConnector::JSONRpcConnector(const std::string & ip, uint16_t port)
{
  jsonrpc_id_ = 0;
  std::string url = "ws://"+ip + ":" + std::to_string(port);
  id_ = endpoint_.connect(url);
}
JSONRpcConnector::~JSONRpcConnector(){}
int JSONRpcConnector::Call(const std::string & method, rapidjson::Value & req_data, rapidjson::Value & resp_data)
{
  rapidjson::Document d;
  rapidjson::Value req;
  req.SetObject();
  req.AddMember("jsonrpc", "2.0", d.GetAllocator());
  // req.AddMember("id", jsonrpc_id_++, d.GetAllocator());
  rapidjson::Value method_value;
  method_value.SetString(method.c_str(), method.size(), d.GetAllocator());
  req.AddMember("method", method_value, d.GetAllocator());
  req.AddMember("params", req_data, d.GetAllocator());
  rapidjson::StringBuffer buffer;
  rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
  req.Accept(writer);
  std::string req_str = buffer.GetString();
  if(GetConnectionStatus() != kOpen)
  {
    return -1;
  }  
  // std::cout << "req_str: " << req_str << std::endl;
  endpoint_.send(id_, req_str);
  auto const & resp_str = endpoint_.get_metadata(id_)->GetMessageStr();
  // std::cout<<"resp_str: "<<resp_str<<"\n";
  // ret_str to json
  d.Parse<0>(resp_str.c_str());
  rapidjson::Value::MemberIterator iter = d.FindMember("result");
  if(iter == d.MemberEnd())
  {
    return -1;
  }
  resp_data = iter->value;
  return 0;    
}

int JSONRpcConnector::CallString(int id, const std::string &method, const std::string & req_str, std::string * resp_str)
{
  if(GetConnectionStatus() != kOpen)
  {
    return -1;
  }  
  // std::string str_jsonrpc = "\"jsonrpc\":\"2.0\",";
  // std::string str_method = "\"method\":\"" + method + "\",";
  // std::string str_params = "\"params\":" + req_str + ",";
  // std::string str_id = "\"id\":" + std::to_string(jsonrpc_id_); 
  // jsonrpc_id_++;
  std::string jsonrpc_req = "{\"jsonrpc\":\"2.0\",\"method\":\"" +method + 
  "\",\"params\":"+req_str+",\"id\":"+ std::to_string(id)+ "}";
  // str_method + str_params + str_id + "}";
  std::cerr<<"xxx jsonrpc_req: "<<jsonrpc_req<<"\n";
  endpoint_.send(id_, jsonrpc_req);  
  if(resp_str)
  {
    std::string jsonrpc_resp;
    jsonrpc_resp = endpoint_.get_metadata(id_)->GetMessageStr();
    *resp_str = jsonrpc_resp;

    // std::cerr << "jsonrpc_resp: " << *resp_str << std::endl;
  }
  // {"jsonrpc": "2.0", "error": {"code": -32600, "message": "Invalid Request"}, "id": null}

  return 0;  
}

// int JSONRpcConnector::CallRpc(const std::string & req_str, std::string * resp_str)
// {
//   std::cout<<"req_str "<<req_str<<"\n";
//   endpoint_.send(id_, req_str);
//   if(resp_str)
//   {
//     *resp_str = endpoint_.get_metadata(id_)->GetMessageStr();
//     int resp_id;
//     ExtractJSONRpcRespString(*resp_str, resp_id, *resp_str);
//     if(resp_id != id)
//     {
//       return -1;
//     }
//     std::cout<<"resp_str "<<*resp_str<<"\n";
//   }  
// }

int JSONRpcConnector::CallRpc(const std::string & method, const std::string & req_data_str, std::string * resp_data_str)
{
  // std::cout<<"req_data_str "<<req_data_str<<"\n";
  int call_jsonrpc_id = jsonrpc_id_++;
  std::string jsonrpc_req;
  std::string jsonrpc_resp;
  jsonrpc_req = ToJSONRpcReqString(call_jsonrpc_id, method, req_data_str);
  // std::cout<<"jsonrpc_req "<<jsonrpc_req<<"\n";
  endpoint_.send(id_, jsonrpc_req);
  if(resp_data_str)
  {
    jsonrpc_resp = endpoint_.get_metadata(id_)->GetMessageStr();
    // std::cout<<"jsonrpc_resp "<<jsonrpc_resp<<"\n";
    int callback_jsonrpc_id;
    auto ret = ExtractJSONRpcRespString(jsonrpc_resp, callback_jsonrpc_id, *resp_data_str);
    if(ret < 0)
    {
      std::cout<<"Invalid jsonrpc response! ret code is: "<<ret<<"\n";
      switch (ret)
      {
      case -1:throw (std::string)"Response Parse Error";break;
      case -2:throw (std::string)"Response Is Not \" jsonrpc \"";break;
      case -3:throw (std::string)"Response \" jsonrpc \" Version Is Not 2.0";break;
      case -4:throw (std::string)"Response Do Not Have \" id \"";break;
      case -5:throw (std::string)"Response Do Not Have \" error \" Or \" result \"";break;
      default:
        throw (std::string)"Response Error";
        break;
      }
      return -1;
    }
    // std::cout<<"resp_data_str "<<*resp_data_str<<"\n";
    if(callback_jsonrpc_id != call_jsonrpc_id)
    {
      throw (std::string)"Dismatch in Request and Response";
      return -1;
    }    
  }
  return 0;
}

JSONRpcConnector::ConnectionStatus JSONRpcConnector::GetConnectionStatus()
{
  auto status = endpoint_.get_metadata(id_)->getStatus();
  if(status == "Open")
  {
    return kOpen;
  }
  else if(status == "Connecting")
  {
    return kConnecting;
  }
  else if(status == "Closed")
  {
    return kClosed;
  }
  else
  {
    return kFailed;
  }

}

};