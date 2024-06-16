#include "block.hh"

namespace lebai {
namespace block {
void Block::set_method(std::string method) { method_ = method; }
std::string Block::method() { return method_; }
std::string *Block::mutable_method() { return &method_; }

void Block::set_param(std::string param) { param_ = param; }
std::string Block::param() { return param_; }
std::string *Block::mutable_param() { return &param_; }

bool Block::Deserialize(const rapidjson::Value &obj) {
  if (obj.HasMember("method")) {
    std::string method_str = (std::string)(obj["method"].GetString());
    method_ = method_str;
  }
  if (obj.HasMember("param")) {
    std::string param_str = (std::string)(obj["param"].GetString());
    param_ = param_str;
  }
  return true;
}
bool Block::Serialize(
    rapidjson::Writer<rapidjson::StringBuffer> *writer) const {
  writer->StartObject();
  writer->Key("method");
  writer->String(method_.c_str());
  writer->Key("param");
  writer->String(param_.c_str());
  writer->EndObject();
  return true;
}
bool Block::IsNullJSONData() const { return false; }

void RunBlockRequest::set_name(std::string name) { name_ = name; }
std::string RunBlockRequest::name() { return name_; }
std::string *RunBlockRequest::mutable_name() { return &name_; }

void RunBlockRequest::set_dir(std::string dir) { dir_ = dir; }
std::string RunBlockRequest::dir() { return dir_; }
std::string *RunBlockRequest::mutable_dir() { return &dir_; }

bool RunBlockRequest::Deserialize(const rapidjson::Value &obj) {
  if (obj.HasMember("name")) {
    std::string name_str = (std::string)(obj["name"].GetString());
    name_ = name_str;
  }
  if (obj.HasMember("dir")) {
    std::string dir_str = (std::string)(obj["dir"].GetString());
    dir_ = dir_str;
  }
  return true;
}
bool RunBlockRequest::Serialize(
    rapidjson::Writer<rapidjson::StringBuffer> *writer) const {
  writer->StartObject();
  writer->Key("name");
  writer->String(name_.c_str());
  writer->Key("dir");
  writer->String(dir_.c_str());
  writer->EndObject();
  return true;
}
bool RunBlockRequest::IsNullJSONData() const { return false; }

void SaveBlockRequest::set_name(std::string name) { name_ = name; }
std::string SaveBlockRequest::name() { return name_; }
std::string *SaveBlockRequest::mutable_name() { return &name_; }

void SaveBlockRequest::set_data(Block data) { data_ = data; }
Block SaveBlockRequest::data() { return data_; }
Block *SaveBlockRequest::mutable_data() { return &data_; }

void SaveBlockRequest::set_dir(std::string dir) { dir_ = dir; }
std::string SaveBlockRequest::dir() { return dir_; }
std::string *SaveBlockRequest::mutable_dir() { return &dir_; }

bool SaveBlockRequest::Deserialize(const rapidjson::Value &obj) {
  if (obj.HasMember("name")) {
    std::string name_str = (std::string)(obj["name"].GetString());
    name_ = name_str;
  }
  if (obj.HasMember("data")) {
    Block data;
    data.FromJSONString(obj["data"].GetString());
    data_ = data;
  }
  if (obj.HasMember("dir")) {
    std::string dir_str = (std::string)(obj["dir"].GetString());
    dir_ = dir_str;
  }
  return true;
}
bool SaveBlockRequest::Serialize(
    rapidjson::Writer<rapidjson::StringBuffer> *writer) const {
  writer->StartObject();
  writer->Key("name");
  writer->String(name_.c_str());
  writer->Key("data");
  writer->String(data_.ToJSONString().c_str());
  writer->Key("dir");
  writer->String(dir_.c_str());
  writer->EndObject();
  return true;
}
bool SaveBlockRequest::IsNullJSONData() const { return false; }
}  // namespace block
}  // namespace lebai