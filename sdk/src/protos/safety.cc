#include "safety.hh"

namespace lebai {
namespace safety {

void CartesianLimit::set_max_a(double max_a) { max_a_ = max_a; }
double CartesianLimit::max_a() const { return max_a_; }
double *CartesianLimit::mutable_max_a() { return &max_a_; }

void CartesianLimit::set_max_v(double max_v) { max_v_ = max_v; }
double CartesianLimit::max_v() const { return max_v_; }
double *CartesianLimit::mutable_max_v() { return &max_v_; }

void CartesianLimit::set_eq_radius(double eq_radius) { eq_radius_ = eq_radius; }
double CartesianLimit::eq_radius() const { return eq_radius_; }
double *CartesianLimit::mutable_eq_radius() { return &eq_radius_; }

bool CartesianLimit::Deserialize(const rapidjson::Value &obj) {
  if (obj.HasMember("max_a")) {
    max_a_ = obj["max_a"].GetDouble();
  }
  if (obj.HasMember("max_v")) {
    max_v_ = obj["max_v"].GetDouble();
  }
  if (obj.HasMember("eq_radius")) {
    eq_radius_ = obj["eq_radius"].GetDouble();
  }
  return true;
}

bool CartesianLimit::Serialize(
    rapidjson::Writer<rapidjson::StringBuffer> *writer) const {
  writer->StartObject();
  writer->String("max_a");
  writer->Double(max_a_);
  writer->String("max_v");
  writer->Double(max_v_);
  writer->String("eq_radius");
  writer->Double(eq_radius_);
  writer->EndObject();
  return true;
}

bool CartesianLimit::IsNullJSONData() const { return false; }

void CollisionDetector::set_action(CollisionDetectorAction action) {
  action_ = action;
}
CollisionDetectorAction CollisionDetector::action() const { return action_; }
CollisionDetectorAction *CollisionDetector::mutable_action() { return &action_; }

void CollisionDetector::set_pause_time(unsigned int pause_time) {
  pause_time_ = pause_time;
}
unsigned int CollisionDetector::pause_time() const { return pause_time_; }
unsigned int *CollisionDetector::mutable_pause_time() { return &pause_time_; }

void CollisionDetector::set_sensitivity(unsigned int sensitivity) {
  sensitivity_ = sensitivity;
}
unsigned int CollisionDetector::sensitivity() const { return sensitivity_; }
unsigned int *CollisionDetector::mutable_sensitivity() {
  return &sensitivity_;
}

bool CollisionDetector::Deserialize(const rapidjson::Value &obj) {
  if (obj.HasMember("action")) {
    action_ =
        static_cast<CollisionDetectorAction>(obj["action"].GetInt());
  }
  if (obj.HasMember("pause_time")) {
    pause_time_ = obj["pause_time"].GetUint();
  }
  if (obj.HasMember("sensitivity")) {
    sensitivity_ = obj["sensitivity"].GetUint();
  }
  return true;
}

bool CollisionDetector::Serialize(
    rapidjson::Writer<rapidjson::StringBuffer> *writer) const {
  writer->StartObject();
  writer->String("action");
  writer->Int(static_cast<int>(action_));
  writer->String("pause_time");
  writer->Uint(pause_time_);
  writer->String("sensitivity");
  writer->Uint(sensitivity_);
  writer->EndObject();
  return true;
}

bool CollisionDetector::IsNullJSONData() const { return false; }

void CollisionTorqueDiff::set_diffs(const std::vector<double> &diffs) {
  diffs_ = diffs;
}
std::vector<double> CollisionTorqueDiff::diffs() const { return diffs_; }
std::vector<double> *CollisionTorqueDiff::mutable_diffs() { return &diffs_; }

bool CollisionTorqueDiff::Deserialize(const rapidjson::Value &obj) {
  diffs_.clear();
  if (obj.HasMember("diffs")) {
    for (auto iter = obj["diffs"].GetArray().Begin();
         iter != obj["diffs"].GetArray().End(); ++iter) {
      diffs_.push_back(iter->GetDouble());
    }
  }
  return true;
}

bool CollisionTorqueDiff::Serialize(
    rapidjson::Writer<rapidjson::StringBuffer> *writer) const {
  writer->StartObject();
  writer->String("diffs");
  writer->StartArray();
  for (auto it = diffs_.begin(); it != diffs_.end(); ++it) {
    writer->Double(*it);
  }
  writer->EndArray();
  writer->EndObject();
  return true;
}

bool CollisionTorqueDiff::IsNullJSONData() const { return diffs_.empty(); }

void JointLimit::set_min_position(double min_position) {
  min_position_ = min_position;
}
double JointLimit::min_position() const { return min_position_; }
double *JointLimit::mutable_min_position() { return &min_position_; }

void JointLimit::set_max_position(double max_position) {
  max_position_ = max_position;
}
double JointLimit::max_position() const { return max_position_; }
double *JointLimit::mutable_max_position() { return &max_position_; }

void JointLimit::set_max_a(double max_a) { max_a_ = max_a; }
double JointLimit::max_a() const { return max_a_; }
double *JointLimit::mutable_max_a() { return &max_a_; }

void JointLimit::set_max_v(double max_v) { max_v_ = max_v; }
double JointLimit::max_v() const { return max_v_; }
double *JointLimit::mutable_max_v() { return &max_v_; }

bool JointLimit::Deserialize(const rapidjson::Value &obj) {
  if (obj.HasMember("min_position")) {
    min_position_ = obj["min_position"].GetDouble();
  }
  if (obj.HasMember("max_position")) {
    max_position_ = obj["max_position"].GetDouble();
  }
  if (obj.HasMember("max_a")) {
    max_a_ = obj["max_a"].GetDouble();
  }
  if (obj.HasMember("max_v")) {
    max_v_ = obj["max_v"].GetDouble();
  }
  return true;
}

bool JointLimit::Serialize(
    rapidjson::Writer<rapidjson::StringBuffer> *writer) const {
  writer->StartObject();
  writer->String("min_position");
  writer->Double(min_position_);
  writer->String("max_position");
  writer->Double(max_position_);
  writer->String("max_a");
  writer->Double(max_a_);
  writer->String("max_v");
  writer->Double(max_v_);
  writer->EndObject();
  return true;
}

bool JointLimit::IsNullJSONData() const { return false; }

void JointsLimit::set_joints(const std::vector<JointLimit> &joints) {
  joints_ = joints;
}
std::vector<JointLimit> JointsLimit::joints() const { return joints_; }
std::vector<JointLimit> *JointsLimit::mutable_joints() { return &joints_; }

bool JointsLimit::Deserialize(const rapidjson::Value &obj) {
  joints_.clear();
  if (obj.HasMember("joints")) {
    for (auto iter = obj["joints"].GetArray().Begin();
         iter != obj["joints"].GetArray().End(); ++iter) {
      JointLimit joint;
      joint.Deserialize(*iter);
      joints_.push_back(joint);
    }
  }
  return true;
}

bool JointsLimit::Serialize(
    rapidjson::Writer<rapidjson::StringBuffer> *writer) const {
  writer->StartObject();
  writer->String("joints");
  writer->StartArray();
  for (auto it = joints_.begin(); it != joints_.end(); ++it) {
    it->Serialize(writer);
  }
  writer->EndArray();
  writer->EndObject();
  return true;
}

bool JointsLimit::IsNullJSONData() const { return joints_.empty(); }

}  // namespace safety
}  // namespace lebai
