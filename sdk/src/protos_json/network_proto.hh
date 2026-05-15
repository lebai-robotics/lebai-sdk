#pragma once

#include <nlohmann/json.hpp>

#include <map>
#include <string>

namespace protos_json::network_proto {

struct HttpRequest {
  std::string method;
  std::string url;
  std::map<std::string, std::string> headers;
  std::string body;
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(HttpRequest, method, url, headers, body)
};

struct HttpResponse {
  unsigned int status{};
  std::map<std::string, std::string> headers;
  std::string body;
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(HttpResponse, status, headers, body)
};

}  // namespace protos_json::network_proto
