#pragma once

#include "jsonbase.hh"
#include <string>
#include <vector>

namespace lebai {
namespace storage {

class GetItemsRequest : public JSONBase {
 public:
  void set_prefix(std::string prefix);
  std::string prefix() const;
  std::string *mutable_prefix();

 protected:
  std::string prefix_;
  // These methods are used to serialize and deserialize the class.
  // They will not be wrapped in the SDK.
 public:
  virtual bool Deserialize(const rapidjson::Value &obj);
  virtual bool Serialize(
      rapidjson::Writer<rapidjson::StringBuffer> *writer) const;
  virtual bool IsNullJSONData() const;
};

class Item : public JSONBase {
 public:
  void set_key(std::string key);
  std::string key() const;
  std::string *mutable_key();

  void set_value(std::string value);
  std::string value() const;
  std::string *mutable_value();

 protected:
  std::string key_;
  std::string value_;
  // These methods are used to serialize and deserialize the class.
  // They will not be wrapped in the SDK.
 public:
  virtual bool Deserialize(const rapidjson::Value &obj);
  virtual bool Serialize(
      rapidjson::Writer<rapidjson::StringBuffer> *writer) const;
  virtual bool IsNullJSONData() const;
};

class ItemIndex : public JSONBase {
 public:
  void set_key(std::string key);
  std::string key() const;
  std::string *mutable_key();

 protected:
  std::string key_;
  // These methods are used to serialize and deserialize the class.
  // They will not be wrapped in the SDK.
 public:
  virtual bool Deserialize(const rapidjson::Value &obj);
  virtual bool Serialize(
      rapidjson::Writer<rapidjson::StringBuffer> *writer) const;
  virtual bool IsNullJSONData() const;
};

class Items : public JSONBase {
 public:
  void set_items(std::vector<Item> items);
  std::vector<Item> items() const;
  std::vector<Item> *mutable_items();

 private:
  std::vector<Item> items_;
  // These methods are used to serialize and deserialize the class.
  // They will not be wrapped in the SDK.
 public:
  virtual bool Deserialize(const rapidjson::Value &obj);
  virtual bool Serialize(
      rapidjson::Writer<rapidjson::StringBuffer> *writer) const;
  virtual bool IsNullJSONData() const;
};

}  // namespace storage
}  // namespace lebai