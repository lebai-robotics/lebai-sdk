#include <string>
#include <cmath>
#include "kinematic.hh"

namespace lebai {
namespace kinematic {
KinData &KinData::operator=(const KinData &other) {
  actual_joint_pose_ = other.actual_joint_pose_;
  actual_joint_speed_ = other.actual_joint_speed_;
  actual_joint_acc_ = other.actual_joint_acc_;
  actual_joint_torque_ = other.actual_joint_torque_;
  target_joint_pose_ = other.target_joint_pose_;
  target_joint_speed_ = other.target_joint_speed_;
  target_joint_acc_ = other.target_joint_acc_;
  target_joint_torque_ = other.target_joint_torque_;
  // actual_tcp_pose_ = other.actual_tcp_pose_;
  // target_tcp_pose_ = other.target_tcp_pose_;
  // actual_flange_pose_ = other.actual_flange_pose_;
  return *this;
}
void KinData::set_actual_joint_pose(
    const std::vector<double> &actual_joint_pose) {
  actual_joint_pose_ = actual_joint_pose;
}
const std::vector<double> &KinData::actual_joint_pose() const {
  return actual_joint_pose_;
}
std::vector<double> *KinData::mutable_actual_joint_pose() {
  return &actual_joint_pose_;
}
void KinData::set_actual_joint_speed(
    const std::vector<double> &actual_joint_speed) {
  actual_joint_speed_ = actual_joint_speed;
}
const std::vector<double> &KinData::actual_joint_speed() const {
  return actual_joint_speed_;
}
std::vector<double> *KinData::mutable_actual_joint_speed() {
  return &actual_joint_speed_;
}
void KinData::set_actual_joint_acc(
    const std::vector<double> &actual_joint_acc) {
  actual_joint_acc_ = actual_joint_acc;
}
const std::vector<double> &KinData::actual_joint_acc() const {
  return actual_joint_acc_;
}
std::vector<double> *KinData::mutable_actual_joint_acc() {
  return &actual_joint_acc_;
}
void KinData::set_actual_joint_torque(
    const std::vector<double> &actual_joint_torque) {
  actual_joint_torque_ = actual_joint_torque;
}
const std::vector<double> &KinData::actual_joint_torque() const {
  return actual_joint_torque_;
}
std::vector<double> *KinData::mutable_actual_joint_torque() {
  return &actual_joint_torque_;
}
void KinData::set_target_joint_pose(
    const std::vector<double> &target_joint_pose) {
  target_joint_pose_ = target_joint_pose;
}
const std::vector<double> &KinData::target_joint_pose() const {
  return target_joint_pose_;
}
std::vector<double> *KinData::mutable_target_joint_pose() {
  return &target_joint_pose_;
}
void KinData::set_target_joint_speed(
    const std::vector<double> &target_joint_speed) {
  target_joint_speed_ = target_joint_speed;
}
const std::vector<double> &KinData::target_joint_speed() const {
  return target_joint_speed_;
}
std::vector<double> *KinData::mutable_target_joint_speed() {
  return &target_joint_speed_;
}
void KinData::set_target_joint_acc(
    const std::vector<double> &target_joint_acc) {
  target_joint_acc_ = target_joint_acc;
}
const std::vector<double> &KinData::target_joint_acc() const {
  return target_joint_acc_;
}
std::vector<double> *KinData::mutable_target_joint_acc() {
  return &target_joint_acc_;
}
void KinData::set_target_joint_torque(
    const std::vector<double> &target_joint_torque) {
  target_joint_torque_ = target_joint_torque;
}
const std::vector<double> &KinData::target_joint_torque() const {
  return target_joint_torque_;
}
std::vector<double> *KinData::mutable_target_joint_torque() {
  return &target_joint_torque_;
}
void KinData::set_actual_tcp_pose(
    const posture::CartesianPose &actual_tcp_pose) {
  actual_tcp_pose_ = actual_tcp_pose;
}
const posture::CartesianPose &KinData::actual_tcp_pose() const {
  return actual_tcp_pose_;
}
posture::CartesianPose *KinData::mutable_actual_tcp_pose() {
  return &actual_tcp_pose_;
}
// target_tcp_pose_
void KinData::set_target_tcp_pose(
    const posture::CartesianPose &target_tcp_pose) {
  target_tcp_pose_ = target_tcp_pose;
}
const posture::CartesianPose &KinData::target_tcp_pose() const {
  return target_tcp_pose_;
}
posture::CartesianPose *KinData::mutable_target_tcp_pose() {
  return &target_tcp_pose_;
}
// actual_flange_pose_
void KinData::set_actual_flange_pose(
    const posture::CartesianPose &actual_flange_pose) {
  actual_flange_pose_ = actual_flange_pose;
}
const posture::CartesianPose &KinData::actual_flange_pose() const {
  return actual_flange_pose_;
}
posture::CartesianPose *KinData::mutable_actual_flange_pose() {
  return &actual_flange_pose_;
}
// Deserialize
bool KinData::Deserialize(const rapidjson::Value &obj) {
  actual_joint_pose_.clear();
  for (auto iter = obj["actual_joint_pose"].GetArray().Begin();
       iter != obj["actual_joint_pose"].GetArray().End(); iter++) {
    actual_joint_pose_.push_back(iter->GetDouble());
  }
  actual_joint_speed_.clear();
  for (auto iter = obj["actual_joint_speed"].GetArray().Begin();
       iter != obj["actual_joint_speed"].GetArray().End(); iter++) {
    actual_joint_speed_.push_back(iter->GetDouble());
  }
  actual_joint_acc_.clear();
  for (auto iter = obj["actual_joint_acc"].GetArray().Begin();
       iter != obj["actual_joint_acc"].GetArray().End(); iter++) {
    actual_joint_acc_.push_back(iter->GetDouble());
  }
  actual_joint_torque_.clear();
  for (auto iter = obj["actual_joint_torque"].GetArray().Begin();
       iter != obj["actual_joint_torque"].GetArray().End(); iter++) {
    actual_joint_torque_.push_back(iter->GetDouble());
  }
  target_joint_pose_.clear();
  for (auto iter = obj["target_joint_pose"].GetArray().Begin();
       iter != obj["target_joint_pose"].GetArray().End(); iter++) {
    target_joint_pose_.push_back(iter->GetDouble());
  }
  target_joint_speed_.clear();
  for (auto iter = obj["target_joint_speed"].GetArray().Begin();
       iter != obj["target_joint_speed"].GetArray().End(); iter++) {
    target_joint_speed_.push_back(iter->GetDouble());
  }
  target_joint_acc_.clear();
  for (auto iter = obj["target_joint_acc"].GetArray().Begin();
       iter != obj["target_joint_acc"].GetArray().End(); iter++) {
    target_joint_acc_.push_back(iter->GetDouble());
  }
  target_joint_torque_.clear();
  for (auto iter = obj["target_joint_torque"].GetArray().Begin();
       iter != obj["target_joint_torque"].GetArray().End(); iter++) {
    target_joint_torque_.push_back(iter->GetDouble());
  }
  if (obj.HasMember("actual_tcp_pose")) {
    actual_tcp_pose_.Deserialize(obj["actual_tcp_pose"]);
  }
  if (obj.HasMember("target_tcp_pose")) {
    target_tcp_pose_.Deserialize(obj["target_tcp_pose"]);
  }
  if (!obj.HasMember("actual_flange_pose")) {
    actual_flange_pose_.Deserialize(obj["actual_flange_pose"]);
  }
  return true;
}
// Serialize
bool KinData::Serialize(
    rapidjson::Writer<rapidjson::StringBuffer> *writer) const {
  writer->StartObject();
  writer->Key("actual_joint_pose");
  writer->StartArray();
  for (auto iter = actual_joint_pose_.begin(); iter != actual_joint_pose_.end();
       iter++) {
    writer->Double(*iter);
  }
  writer->EndArray();
  writer->Key("actual_joint_speed");
  writer->StartArray();
  for (auto iter = actual_joint_speed_.begin();
       iter != actual_joint_speed_.end(); iter++) {
    writer->Double(*iter);
  }
  writer->EndArray();
  writer->Key("actual_joint_acc");
  writer->StartArray();
  for (auto iter = actual_joint_acc_.begin(); iter != actual_joint_acc_.end();
       iter++) {
    writer->Double(*iter);
  }
  writer->EndArray();
  writer->Key("actual_joint_torque");
  writer->StartArray();
  for (auto iter = actual_joint_torque_.begin();
       iter != actual_joint_torque_.end(); iter++) {
    writer->Double(*iter);
  }
  writer->EndArray();
  writer->Key("target_joint_pose");
  writer->StartArray();
  for (auto iter = target_joint_pose_.begin(); iter != target_joint_pose_.end();
       iter++) {
    writer->Double(*iter);
  }
  writer->EndArray();
  writer->Key("target_joint_speed");
  writer->StartArray();
  for (auto iter = target_joint_speed_.begin();
       iter != target_joint_speed_.end(); iter++) {
    writer->Double(*iter);
  }
  writer->EndArray();
  writer->Key("target_joint_acc");
  writer->StartArray();
  for (auto iter = target_joint_acc_.begin(); iter != target_joint_acc_.end();
       iter++) {
    writer->Double(*iter);
  }
  writer->EndArray();
  writer->Key("target_joint_torque");
  writer->StartArray();
  for (auto iter = target_joint_torque_.begin();
       iter != target_joint_torque_.end(); iter++) {
    writer->Double(*iter);
  }
  writer->EndArray();
  if (!actual_tcp_pose_.IsNullJSONData()) {
    writer->Key("actual_tcp_pose");
    actual_tcp_pose_.Serialize(writer);
  }
  if (!target_tcp_pose_.IsNullJSONData()) {
    writer->Key("target_tcp_pose");
    target_tcp_pose_.Serialize(writer);
  }
  if (!actual_flange_pose_.IsNullJSONData()) {
    writer->Key("actual_flange_pose");
    actual_flange_pose_.Serialize(writer);
  }
  writer->EndObject();
  return true;
}
// IsNullJSONData
bool KinData::IsNullJSONData() const {
  if (actual_joint_pose_.size() > 0) {
    return false;
  }
  if (actual_joint_speed_.size() > 0) {
    return false;
  }
  if (actual_joint_acc_.size() > 0) {
    return false;
  }
  if (actual_joint_torque_.size() > 0) {
    return false;
  }
  if (target_joint_pose_.size() > 0) {
    return false;
  }
  if (target_joint_speed_.size() > 0) {
    return false;
  }
  if (target_joint_acc_.size() > 0) {
    return false;
  }
  if (target_joint_torque_.size() > 0) {
    return false;
  }
  if (!actual_tcp_pose_.IsNullJSONData()) {
    return false;
  }
  if (!target_tcp_pose_.IsNullJSONData()) {
    return false;
  }
  if (!actual_flange_pose_.IsNullJSONData()) {
    return false;
  }
  return true;
}

void KinFactor::set_factor(int factor) { factor_ = factor; }
int KinFactor::factor() { return factor_; }
int *KinFactor::mutable_factor() { return &factor_; }
bool KinFactor::Deserialize(const rapidjson::Value &obj) {
  if (obj.HasMember("speed_factor")) {
    int factor = (int)(obj["speed_factor"].GetInt());
    factor_ = factor;
  }
  return true;
}
bool KinFactor::Serialize(
    rapidjson::Writer<rapidjson::StringBuffer> *writer) const {
  writer->StartObject();
  writer->Key("speed_factor");
  writer->Int(factor_);
  writer->EndObject();
  return true;
}
bool KinFactor::IsNullJSONData() const { return false; }
}  // namespace kinematic
}  // namespace lebai