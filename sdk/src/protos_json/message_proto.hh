#pragma once

#include <nlohmann/json.hpp>

#include <string>
#include <vector>

namespace protos_json::message_proto {

struct Message {
  std::string level;
  std::string kind;
  std::string detail;
  std::string time;
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(Message, level, kind, detail, time)
};

struct Messages {
  std::vector<Message> messages;
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(Messages, messages)
};

}  // namespace protos_json::message_proto
