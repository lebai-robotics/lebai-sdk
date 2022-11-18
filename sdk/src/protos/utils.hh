#include "jsonbase.hh"
#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"		// rapidjson's DOM-style API
#include "rapidjson/stringbuffer.h"	// wrapper of C stream for prettywriter as output
#include "rapidjson/prettywriter.h"	// for stringify JSON

#pragma once

namespace lebai
{
  /**
   * @brief Generate a json rpc request string from id, method and a json data as params. 
   * 
   * @param id The id of the request.
   * @param method The method name.
   * @param data The json data strucutre for params.
   * @return std::string A json rpc request string.
   */
  std::string ToJSONRpcReqString(int id, const std::string & method, const JSONBase & data);
  /**
   * @brief Generate a json rpc request string from id, method and a params string. 
   * 
   * @param id The id of the request.
   * @param method The method name.
   * @param data The params strucutre.
   * @return std::string A json rpc request string.
   */  
  std::string ToJSONRpcReqString(int id, const std::string & method, const std::string & data);
  // int FromJSONRpcRespString(const std::string &s, JSONBase & data);
  enum JSONRpcRespParseResult
  {
    kResult = 0,
    kError = -1,
    kInvalid = -2,
  };
  /**
   * @brief Get the result from a json rpc response string.
   * 
   * @param s The json rpc response string.
   * @param id The id of the response.
   * @param error_code The error code of the response.
   * @param data The response string data.
   * @return int return code. if 0, success. if not 0, it's not a valid response.
   */

  JSONRpcRespParseResult ExtractJSONRpcRespString(const std::string &s, int & id, int & error_code, std::string & data);
}