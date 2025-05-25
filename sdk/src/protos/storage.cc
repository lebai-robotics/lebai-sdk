#include <iostream>
#include "storage.hh"

namespace lebai {
namespace storage {

// GetItemsRequest implementation
void GetItemsRequest::set_prefix(std::string prefix) { prefix_ = prefix; }
std::string GetItemsRequest::prefix() const { return prefix_; }
std::string* GetItemsRequest::mutable_prefix() { return &prefix_; }

bool GetItemsRequest::Deserialize(const rapidjson::Value& obj) {
  if (obj.HasMember("prefix")) {
    prefix_ = obj["prefix"].GetString();
  }
  return true;
}

bool GetItemsRequest::Serialize(
    rapidjson::Writer<rapidjson::StringBuffer>* writer) const {
  writer->StartObject();
  writer->Key("prefix");
  writer->String(prefix_.c_str());
  writer->EndObject();
  return true;
}

bool GetItemsRequest::IsNullJSONData() const { return false; }

// Item implementation
void Item::set_key(std::string key) { key_ = key; }
std::string Item::key() const { return key_; }
std::string* Item::mutable_key() { return &key_; }

void Item::set_value(std::string value) { value_ = value; }
std::string Item::value() const { return value_; }
std::string* Item::mutable_value() { return &value_; }

bool Item::Deserialize(const rapidjson::Value& obj) {
  if (obj.HasMember("key")) {
    key_ = obj["key"].GetString();
  }
  if (obj.HasMember("value")) {
    value_ = obj["value"].GetString();
  }
  return true;
}

bool Item::Serialize(rapidjson::Writer<rapidjson::StringBuffer>* writer) const {
  writer->StartObject();
  writer->Key("key");
  writer->String(key_.c_str());
  writer->Key("value");
  writer->String(value_.c_str());
  writer->EndObject();
  return true;
}

bool Item::IsNullJSONData() const { return false; }

// ItemIndex implementation
void ItemIndex::set_key(std::string key) { key_ = key; }
std::string ItemIndex::key() const { return key_; }
std::string* ItemIndex::mutable_key() { return &key_; }

bool ItemIndex::Deserialize(const rapidjson::Value& obj) {
  if (obj.HasMember("key")) {
    key_ = obj["key"].GetString();
  }
  return true;
}

bool ItemIndex::Serialize(
    rapidjson::Writer<rapidjson::StringBuffer>* writer) const {
  writer->StartObject();
  writer->Key("key");
  writer->String(key_.c_str());
  writer->EndObject();
  return true;
}

bool ItemIndex::IsNullJSONData() const { return false; }

// Items implementation
void Items::set_items(std::vector<Item> items) { items_ = std::move(items); }
std::vector<Item> Items::items() const { return items_; }
std::vector<Item>* Items::mutable_items() { return &items_; }

bool Items::Deserialize(const rapidjson::Value& obj) {
  if (obj.HasMember("items")) {
    const rapidjson::Value& items_array = obj["items"];
    items_.clear();
    for (rapidjson::SizeType i = 0; i < items_array.Size(); i++) {
      Item item;
      if (item.Deserialize(items_array[i])) {
        items_.push_back(std::move(item));
      }
    }
  }
  return true;
}

bool Items::Serialize(
    rapidjson::Writer<rapidjson::StringBuffer>* writer) const {
  writer->StartObject();
  writer->Key("items");
  writer->StartArray();
  for (const auto& item : items_) {
    item.Serialize(writer);
  }
  writer->EndArray();
  writer->EndObject();
  return true;
}

bool Items::IsNullJSONData() const { return false; }

}  // namespace storage
}  // namespace lebai