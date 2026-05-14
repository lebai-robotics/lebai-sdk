#pragma once

#include <nlohmann/json.hpp>

#include <string>

namespace protos_json::structure_proto {

struct Structure {
  bool active{};
  std::string name;
  std::string kind;
  unsigned int dof{};
  std::string dh;
  std::string dyn;
  std::string servo;
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(Structure, active, name, kind, dof, dh, dyn,
                                 servo)
};

}  // namespace protos_json::structure_proto
