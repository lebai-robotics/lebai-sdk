#include "utils.hh"
#include <iostream>
namespace lebai
{
  std::string ToJSONRpcReqString(int id, const std::string & method, const JSONBase & data)
  {
    rapidjson::StringBuffer ss;
    rapidjson::Writer<rapidjson::StringBuffer> writer(ss);		// Can also use Writer for condensed formatting
    writer.StartObject();
    writer.String("jsonrpc");
    writer.String("2.0");
    writer.String("method");
    writer.String(method.c_str());
    writer.String("params");
    writer.StartArray();
    data.Serialize(&writer);
    writer.EndArray();
    writer.String("id");
    writer.Int(id);
    writer.EndObject();
    return std::string(ss.GetString());
  }
  std::string ToJSONRpcReqString(int id, const std::string & method, const std::string & data)
  {
      return std::string("{\"jsonrpc\":\"2.0\",\"method\":\"" +method + 
  "\",\"params\":["+data+"],\"id\":"+ std::to_string(id)+ "}");
  }


//   int FromJSONRpcRespString(const std::string &s, JSONBase & data)
//   {
//     rapidjson::Document doc;
//     if (!data.InitDocument(s, doc))
//     {
//       return -1;
//     }

//     if(doc.HasMember("error"))
//     {
//       // Error code and message.
//       int error_id = doc["error"]["code"].GetInt();
//       std::string error_msg = doc["error"]["message"].GetString();
//       // std::cout << "Error: " << error_id << " " << error_msg << std::endl;
//       return error_id;
//     }

//     if (doc.HasMember("result"))
//     {
//       data.Deserialize(doc["result"]);
//     }
// // rapidjson::Document doc;
//   //   	if (s.empty())
// 	// 	return false;

// 	// std::string validJson(s);

// 	// return !doc.Parse(validJson.c_str()).HasParseError() ? true : false;


//     return true;    
//   }


  JSONRpcRespParseResult ExtractJSONRpcRespString(const std::string &s, int & id, int & error_code, std::string & data)
  {
    rapidjson::Document doc;
    if(doc.Parse(s.c_str()).HasParseError())
    {
      return kInvalid;
    }
    if(!doc.HasMember("jsonrpc"))
    {
      return kInvalid;
    }
    // std::string v = std::string(doc["jsonrpc"].GetString());
    if(std::string(doc["jsonrpc"].GetString()) != "2.0")
    {
      // std::cout<<"ccc\n";
      return kInvalid;
    }
    if(!doc.HasMember("id"))
    {
      return kInvalid;
    }
    id = doc["id"].GetInt();

    if(doc.HasMember("error"))
    {
      // Error code and message.
      error_code = doc["error"]["code"].GetInt();
      data = doc["error"]["message"].GetString();
      // std::cout << "Error: " << error_id << " " << error_msg << std::endl;
      return kError;
    }

    if (doc.HasMember("result"))
    {
      rapidjson::StringBuffer buffer;
      buffer.Clear();
      rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
      doc["result"].Accept(writer);
      data =  buffer.GetString();
      return kResult;
    }
    return kInvalid;
    
  }
}