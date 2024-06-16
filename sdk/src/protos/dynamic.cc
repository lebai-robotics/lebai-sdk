#include <string>
#include "dynamic.hh"

namespace lebai {
namespace dynamic {
void SetMassRequest::set_mass(double mass) { mass_ = mass; }
double SetMassRequest::mass() const { return mass_; }
double *SetMassRequest::mutable_mass() { return &mass_; }
bool SetMassRequest::Deserialize(const rapidjson::Value &obj) {
  if (obj.HasMember("mass")) {
    double mass = (double)(obj["mass"].GetDouble());
    mass_ = mass;
  }
  return true;
}
bool SetMassRequest::Serialize(
    rapidjson::Writer<rapidjson::StringBuffer> *writer) const {
  writer->StartObject();
  writer->Key("mass");
  writer->Double(mass_);
  writer->EndObject();
  return true;
}
bool SetMassRequest::IsNullJSONData() const { return false; }

void SetCogRequest::set_cog(posture::Position cog) { cog_ = cog; }
posture::Position SetCogRequest::cog() const { return cog_; }
posture::Position *SetCogRequest::mutable_cog() { return &cog_; }

bool SetCogRequest::Deserialize(const rapidjson::Value &obj) {
  if (obj.HasMember("cog")) {
    posture::Position cog;
    cog.set_x(obj["cog"].GetObject()["x"].GetDouble());
    cog.set_y(obj["cog"].GetObject()["y"].GetDouble());
    cog.set_z(obj["cog"].GetObject()["z"].GetDouble());
    cog_ = cog;
  }
  return true;
}
bool SetCogRequest::Serialize(
    rapidjson::Writer<rapidjson::StringBuffer> *writer) const {
  writer->StartObject();
  writer->Key("cog");
  cog_.Serialize(writer);
  writer->EndObject();
  return true;
}
bool SetCogRequest::IsNullJSONData() const { return false; }

void SetPayloadRequest::set_mass(double mass) { mass_ = mass; }
double SetPayloadRequest::mass() const { return mass_; }
double *SetPayloadRequest::mutable_mass() { return &mass_; }

void SetPayloadRequest::set_cog(posture::Position cog) { cog_ = cog; }
posture::Position SetPayloadRequest::cog() const { return cog_; }
posture::Position *SetPayloadRequest::mutable_cog() { return &cog_; }

bool SetPayloadRequest::Deserialize(const rapidjson::Value &obj) {
  if (obj.HasMember("mass")) {
    double mass = (double)(obj["mass"].GetDouble());
    mass_ = mass;
  }
  if (obj.HasMember("cog")) {
    posture::Position cog;
    cog.set_x(obj["cog"].GetObject()["x"].GetDouble());
    cog.set_y(obj["cog"].GetObject()["y"].GetDouble());
    cog.set_z(obj["cog"].GetObject()["z"].GetDouble());
    cog_ = cog;
  }
  return true;
}
bool SetPayloadRequest::Serialize(
    rapidjson::Writer<rapidjson::StringBuffer> *writer) const {
  writer->StartObject();
  writer->Key("mass");
  writer->Double(mass_);
  writer->Key("cog");
  cog_.Serialize(writer);
  writer->EndObject();
  return true;
}
bool SetPayloadRequest::IsNullJSONData() const { return false; }

void Payload::set_mass(double mass) { mass_ = mass; }
double Payload::mass() const { return mass_; }
double *Payload::mutable_mass() { return &mass_; }

void Payload::set_cog(posture::Position cog) { cog_ = cog; }
posture::Position Payload::cog() const { return cog_; }
posture::Position *Payload::mutable_cog() { return &cog_; }

bool Payload::Deserialize(const rapidjson::Value &obj) {
  if (obj.HasMember("mass")) {
    double mass = (double)(obj["mass"].GetDouble());
    mass_ = mass;
  }
  if (obj.HasMember("cog")) {
    posture::Position cog;
    cog.set_x(obj["cog"].GetObject()["x"].GetDouble());
    cog.set_y(obj["cog"].GetObject()["y"].GetDouble());
    cog.set_z(obj["cog"].GetObject()["z"].GetDouble());
    cog_ = cog;
  }
  return true;
}
bool Payload::Serialize(
    rapidjson::Writer<rapidjson::StringBuffer> *writer) const {
  writer->StartObject();
  writer->Key("mass");
  writer->Double(mass_);
  writer->Key("cog");
  cog_.Serialize(writer);
  writer->EndObject();
  return true;
}
bool Payload::IsNullJSONData() const { return false; }

void SavePayloadRequest::set_name(std::string name) { name_ = name; }
std::string SavePayloadRequest::name() const { return name_; }
std::string *SavePayloadRequest::mutable_name() { return &name_; }

void SavePayloadRequest::set_data(Payload data) { data_ = data; }
Payload SavePayloadRequest::data() const { return data_; }
Payload *SavePayloadRequest::mutable_data() { return &data_; }

void SavePayloadRequest::set_dir(std::string dir) { dir_ = dir; }
std::string SavePayloadRequest::dir() const { return dir_; }
std::string *SavePayloadRequest::mutable_dir() { return &dir_; }
bool SavePayloadRequest::Deserialize(const rapidjson::Value &obj) {
  if (obj.HasMember("name")) {
    std::string name = (std::string)(obj["name"].GetString());
    name_ = name;
  }
  if (obj.HasMember("data")) {
    Payload data;
    data.FromJSONString(obj["data"].GetString());
    data_ = data;
  }
  if (obj.HasMember("dir")) {
    std::string dir = (std::string)(obj["dir"].GetString());
    dir_ = dir;
  }
  return true;
}
bool SavePayloadRequest::Serialize(
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
bool SavePayloadRequest::IsNullJSONData() const { return false; }
}  // namespace dynamic
}  // namespace lebai