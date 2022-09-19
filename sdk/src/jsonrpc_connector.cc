#include "jsonrpc_connector.hh"

#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

namespace lebai
{

JSONRpcConnector::JSONRpcConnector(const std::string & ip, uint16_t port)
{
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
  req.AddMember("id", jsonrpc_id_++, d.GetAllocator());
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

int JSONRpcConnector::CallString(const std::string &method, const std::string & req_str, std::string * resp_str)
{
  if(GetConnectionStatus() != kOpen)
  {
    return -1;
  }  
  // std::string str_jsonrpc = "\"jsonrpc\":\"2.0\",";
  // std::string str_method = "\"method\":\"" + method + "\",";
  // std::string str_params = "\"params\":" + req_str + ",";
  // std::string str_id = "\"id\":" + std::to_string(jsonrpc_id_); 
  jsonrpc_id_++;
  std::string jsonrpc_req = "{\"jsonrpc\":\"2.0\",\"method\":\"" +method + 
  "\",\"params\":"+req_str+",\"id\":"+ std::to_string(jsonrpc_id_)+ "}";
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

int JSONRpcConnector::CallRpc(const std::string & req_str, std::string * resp_str)
{
  std::cout<<"req_str "<<req_str<<"\n";
  endpoint_.send(id_, req_str);
  if(resp_str)
  {
    *resp_str = endpoint_.get_metadata(id_)->GetMessageStr();
    std::cout<<"resp_str "<<*resp_str<<"\n";
  }

  
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