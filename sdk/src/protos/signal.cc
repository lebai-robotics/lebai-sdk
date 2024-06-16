#include <iostream>
#include "signal.hh"

namespace lebai {
namespace signal {
void SetSignalRequest::set_key(unsigned int key) { key_ = key; }
unsigned int SetSignalRequest::key() const { return key_; }
unsigned int *SetSignalRequest::mutable_key() { return &key_; }
void SetSignalRequest::set_value(int value) { value_ = value; }
int SetSignalRequest::value() const { return value_; }
int *SetSignalRequest::mutable_value() { return &value_; }
bool SetSignalRequest::Deserialize(const rapidjson::Value &obj) {
  if (obj.HasMember("key")) {
    unsigned int key_int = (unsigned int)(obj["key"].GetUint());
    key_ = key_int;
  }
  if (obj.HasMember("value")) {
    int value_int = int(obj["value"].GetInt());
    value_ = value_int;
  }
  return true;
}
bool SetSignalRequest::Serialize(
    rapidjson::Writer<rapidjson::StringBuffer> *writer) const {
  writer->StartObject();
  writer->Key("key");
  writer->Uint(key_);
  writer->Key("value");
  writer->Int(value_);
  writer->EndObject();
  return true;
}
bool SetSignalRequest::IsNullJSONData() const { return false; }

void GetSignalRequest::set_key(unsigned int key) { key_ = key; }
unsigned int GetSignalRequest::key() const { return key_; }
unsigned int *GetSignalRequest::mutable_key() { return &key_; }
bool GetSignalRequest::Deserialize(const rapidjson::Value &obj) {
  if (obj.HasMember("key")) {
    unsigned int key_int = (unsigned int)(obj["key"].GetUint());
    key_ = key_int;
  }
  return true;
}
bool GetSignalRequest::Serialize(
    rapidjson::Writer<rapidjson::StringBuffer> *writer) const {
  writer->StartObject();
  writer->Key("key");
  writer->Uint(key_);
  writer->EndObject();
  return true;
}
bool GetSignalRequest::IsNullJSONData() const { return false; }

void GetSignalResponse::set_value(int value) { value_ = value; }
int GetSignalResponse::value() const { return value_; }
int *GetSignalResponse::mutable_value() { return &value_; }
bool GetSignalResponse::Deserialize(const rapidjson::Value &obj) {
  if (obj.HasMember("value")) {
    int value_int = int(obj["value"].GetInt());
    value_ = value_int;
  }
  return true;
}
bool GetSignalResponse::Serialize(
    rapidjson::Writer<rapidjson::StringBuffer> *writer) const {
  writer->StartObject();
  writer->Key("value");
  writer->Int(value_);
  writer->EndObject();
  return true;
}
bool GetSignalResponse::IsNullJSONData() const { return false; }
}  // namespace signal
}  // namespace lebai