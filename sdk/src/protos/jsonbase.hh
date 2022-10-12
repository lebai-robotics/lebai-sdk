#pragma once

#include <string>
#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"		// rapidjson's DOM-style API
#include "rapidjson/stringbuffer.h"	// wrapper of C stream for prettywriter as output
#include "rapidjson/prettywriter.h"	// for stringify JSON

namespace lebai
{
  
  class JSONBase
  {
  public:    
    std::string ToJSONString() const;
    // std::string ToJSONRpcReqString(const std::string & method, int id) const;
    bool FromJSONString(const std::string &s);
    // bool FromJSONRpcRespString(const std::string &s);
    // virtual const char * ToString() const;
    virtual bool Deserialize(const rapidjson::Value &obj) = 0;
    virtual bool Serialize(rapidjson::Writer<rapidjson::StringBuffer> *writer) const = 0;
    virtual bool IsNullJSONData() const = 0;
  // public:
    bool InitDocument(const std::string &s, rapidjson::Document &doc);
    // std::string internal_json_string_;
  };

} // namespace lebai
