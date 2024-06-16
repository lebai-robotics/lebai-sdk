#pragma once

#include "jsonbase.hh"
#include <vector>
#include <memory>

namespace lebai {
namespace modbus {
class SetCoilRequest : public JSONBase {
 public:
  void set_device(std::string device);
  std::string device() const;
  std::string *mutable_device();

  void set_pin(std::string pin);
  std::string pin() const;
  std::string *mutable_pin();

  void set_value(bool values);
  bool value() const;
  bool *mutable_value();

 protected:
  std::string device_;
  std::string pin_;
  bool value_;
  // These methods are used to serialize and deserialize the class.
  // They will not be wrapped in the SDK.
 public:
  virtual bool Deserialize(const rapidjson::Value &obj);
  virtual bool Serialize(
      rapidjson::Writer<rapidjson::StringBuffer> *writer) const;
  virtual bool IsNullJSONData() const;
};

class SetCoilsRequest : public JSONBase {
 public:
  void set_device(std::string device);
  std::string device() const;
  std::string *mutable_device();

  void set_pin(std::string pin);
  std::string pin() const;
  std::string *mutable_pin();

  void set_values(std::vector<bool> values);
  std::vector<bool> values() const;
  std::vector<bool> *mutable_values();

 protected:
  std::string device_;
  std::string pin_;
  std::vector<bool> values_;
  // These methods are used to serialize and deserialize the class.
  // They will not be wrapped in the SDK.
 public:
  virtual bool Deserialize(const rapidjson::Value &obj);
  virtual bool Serialize(
      rapidjson::Writer<rapidjson::StringBuffer> *writer) const;
  virtual bool IsNullJSONData() const;
};

class GetCoilsRequest : public JSONBase {
 public:
  void set_device(std::string device);
  std::string device() const;
  std::string *mutable_device();

  void set_pin(std::string pin);
  std::string pin() const;
  std::string *mutable_pin();

  void set_count(unsigned int count);
  unsigned int count() const;
  unsigned int *mutable_count();

 protected:
  std::string device_;
  std::string pin_;
  unsigned int count_;
  // These methods are used to serialize and deserialize the class.
  // They will not be wrapped in the SDK.
 public:
  virtual bool Deserialize(const rapidjson::Value &obj);
  virtual bool Serialize(
      rapidjson::Writer<rapidjson::StringBuffer> *writer) const;
  virtual bool IsNullJSONData() const;
};

class GetCoilsResponse : public JSONBase {
 public:
  void set_values(std::vector<bool> values);
  std::vector<bool> values() const;
  std::vector<bool> *mutable_values();

 protected:
  std::vector<bool> values_;
  // These methods are used to serialize and deserialize the class.
  // They will not be wrapped in the SDK.
 public:
  virtual bool Deserialize(const rapidjson::Value &obj);
  virtual bool Serialize(
      rapidjson::Writer<rapidjson::StringBuffer> *writer) const;
  virtual bool IsNullJSONData() const;
};

class SetRegisterRequest : public JSONBase {
 public:
  void set_device(std::string device);
  std::string device() const;
  std::string *mutable_device();

  void set_pin(std::string pin);
  std::string pin() const;
  std::string *mutable_pin();

  void set_value(unsigned int values);
  unsigned int value() const;
  unsigned int *mutable_value();

 protected:
  std::string device_;
  std::string pin_;
  unsigned int value_;
  // These methods are used to serialize and deserialize the class.
  // They will not be wrapped in the SDK.
 public:
  virtual bool Deserialize(const rapidjson::Value &obj);
  virtual bool Serialize(
      rapidjson::Writer<rapidjson::StringBuffer> *writer) const;
  virtual bool IsNullJSONData() const;
};

class SetRegistersRequest : public JSONBase {
 public:
  void set_device(std::string device);
  std::string device() const;
  std::string *mutable_device();

  void set_pin(std::string pin);
  std::string pin() const;
  std::string *mutable_pin();

  void set_values(std::vector<unsigned int> values);
  std::vector<unsigned int> values() const;
  std::vector<unsigned int> *mutable_values();

 protected:
  std::string device_;
  std::string pin_;
  std::vector<unsigned int> values_;
  // These methods are used to serialize and deserialize the class.
  // They will not be wrapped in the SDK.
 public:
  virtual bool Deserialize(const rapidjson::Value &obj);
  virtual bool Serialize(
      rapidjson::Writer<rapidjson::StringBuffer> *writer) const;
  virtual bool IsNullJSONData() const;
};

class GetRegistersRequest : public JSONBase {
 public:
  void set_device(std::string device);
  std::string device() const;
  std::string *mutable_device();

  void set_pin(std::string pin);
  std::string pin() const;
  std::string *mutable_pin();

  void set_count(unsigned int count);
  unsigned int count() const;
  unsigned int *mutable_count();

 protected:
  std::string device_;
  std::string pin_;
  unsigned int count_;
  // These methods are used to serialize and deserialize the class.
  // They will not be wrapped in the SDK.
 public:
  virtual bool Deserialize(const rapidjson::Value &obj);
  virtual bool Serialize(
      rapidjson::Writer<rapidjson::StringBuffer> *writer) const;
  virtual bool IsNullJSONData() const;
};

class GetRegistersResponse : public JSONBase {
 public:
  void set_values(std::vector<unsigned int> values);
  std::vector<unsigned int> values() const;
  std::vector<unsigned int> *mutable_values();

 protected:
  std::vector<unsigned int> values_;
  // These methods are used to serialize and deserialize the class.
  // They will not be wrapped in the SDK.
 public:
  virtual bool Deserialize(const rapidjson::Value &obj);
  virtual bool Serialize(
      rapidjson::Writer<rapidjson::StringBuffer> *writer) const;
  virtual bool IsNullJSONData() const;
};
}  // namespace modbus
}  // namespace lebai