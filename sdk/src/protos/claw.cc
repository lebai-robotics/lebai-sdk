#include <string>
#include "claw.hh"

namespace lebai {
namespace claw {

void SetClawRequest::set_force(double force) { force_ = force; }
double SetClawRequest::force() const { return force_; }
double *SetClawRequest::mutable_force() { return &force_; }
void SetClawRequest::set_amplitude(double amplitude) { amplitude_ = amplitude; }
double SetClawRequest::amplitude() const { return amplitude_; }
double *SetClawRequest::mutable_amplitude() { return &amplitude_; }
// Deserialize
bool SetClawRequest::Deserialize(const rapidjson::Value &obj) {
  if (obj.HasMember("force")) {
    force_ = obj["force"].GetDouble();
  }
  if (obj.HasMember("amplitude")) {
    amplitude_ = obj["amplitude"].GetDouble();
  }
  return true;
}
// Serialize
bool SetClawRequest::Serialize(
    rapidjson::Writer<rapidjson::StringBuffer> *writer) const {
  writer->StartObject();
  writer->Key("force");
  writer->Double(force_);
  writer->Key("amplitude");
  writer->Double(amplitude_);
  writer->EndObject();
  return true;
}
// IsNullJSONData
bool SetClawRequest::IsNullJSONData() const { return false; }

void Claw::set_force(double force) { force_ = force; }
double Claw::force() const { return force_; }
double *Claw::mutable_force() { return &force_; }
void Claw::set_amplitude(double amplitude) { amplitude_ = amplitude; }
double Claw::amplitude() const { return amplitude_; }
double *Claw::mutable_amplitude() { return &amplitude_; }
void Claw::set_hold_on(bool hold_on) { hold_on_ = hold_on; }
bool Claw::hold_on() const { return hold_on_; }
bool *Claw::mutable_hold_on() { return &hold_on_; }
// Deserialize
bool Claw::Deserialize(const rapidjson::Value &obj) {
  if (obj.HasMember("force")) {
    force_ = obj["force"].GetDouble();
  }
  if (obj.HasMember("amplitude")) {
    amplitude_ = obj["amplitude"].GetDouble();
  }
  if (obj.HasMember("hold_on")) {
    hold_on_ = obj["hold_on"].GetBool();
  }
  return true;
}
// Serialize
bool Claw::Serialize(rapidjson::Writer<rapidjson::StringBuffer> *writer) const {
  writer->StartObject();
  writer->Key("force");
  writer->Double(force_);
  writer->Key("amplitude");
  writer->Double(amplitude_);
  writer->Key("hold_on");
  writer->Bool(hold_on_);
  writer->EndObject();
  return true;
}
// IsNullJSONData
bool Claw::IsNullJSONData() const { return false; }

}  // namespace claw
}  // namespace lebai