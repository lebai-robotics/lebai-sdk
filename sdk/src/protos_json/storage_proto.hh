#pragma once

#include <nlohmann/json.hpp>

#include <string>
#include <vector>

namespace protos_json::storage_proto {

struct GetItemsRequest {
  std::string prefix;
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(GetItemsRequest, prefix)
};

struct Item {
  std::string key;
  std::string value;
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(Item, key, value)
};

struct ItemIndex {
  std::string key;
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(ItemIndex, key)
};

struct Items {
  std::vector<Item> items;
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(Items, items)
};

}  // namespace protos_json::storage_proto
