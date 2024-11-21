#include <string>
#include <cmath>
#include "motion.hh"

namespace lebai {
namespace motion {
// MoveParam begin
MoveParam::MoveParam() {}
MoveParam::MoveParam(const MoveParam& other) {
  *this = other;
  // if(other.v_)
  // {
  //   v_ = std::make_unique<double>(other.v_);
  // }
  // if(other.a_)
  // {
  //   a_ = std::make_unique<double>(other.a_);
  // }
  // if(other.t_)
  // {
  //   t_ = std::make_unique<double>(other.t_);
  // }
  // if(other.r_)
  // {
  //   r_ = std::make_unique<double>(other.r_);
  // }
}
MoveParam& MoveParam::operator=(const MoveParam& other) {
  velocity_.reset();
  if (other.velocity_) {
    velocity_ = std::make_unique<double>(*other.velocity_);
  }
  acc_.reset();
  if (other.acc_) {
    acc_ = std::make_unique<double>(*other.acc_);
  }
  time_.reset();
  if (other.time_) {
    time_ = std::make_unique<double>(*other.time_);
  }
  radius_.reset();
  if (other.radius_) {
    radius_ = std::make_unique<double>(*other.radius_);
  }
  return *this;
}
void MoveParam::set_velocity(double v) {
  velocity_ = std::make_unique<double>(v);
}
// double v() const;
double* MoveParam::velocity() const { return velocity_.get(); }
double* MoveParam::mutable_velocity() {
  if (velocity_ == nullptr) {
    velocity_ = std::make_unique<double>();
  }
  return velocity_.get();
}
// void set_a(double);
void MoveParam::set_acc(double a) { acc_ = std::make_unique<double>(a); }
// double a() const;
double* MoveParam::acc() const { return acc_.get(); }
double* MoveParam::mutable_acc() {
  if (acc_ == nullptr) {
    acc_ = std::make_unique<double>();
  }
  return acc_.get();
}
// void set_t(double);
void MoveParam::set_time(double time) {
  time_ = std::make_unique<double>(time);
}
// double t() const;
double* MoveParam::t() const { return time_.get(); }
double* MoveParam::mutable_time() {
  if (time_ == nullptr) {
    time_ = std::make_unique<double>();
  }
  return time_.get();
}
// void set_r(double);
void MoveParam::set_radius(double r) { radius_ = std::make_unique<double>(r); }
// double r() const;
double* MoveParam::radius() const { return radius_.get(); }
// double & mutable_r();
double* MoveParam::mutable_radius() {
  if (radius_ == nullptr) {
    radius_ = std::make_unique<double>();
  }
  return radius_.get();
}

bool MoveParam::Deserialize(const rapidjson::Value& obj) {
  if (obj.HasMember("velocity")) {
    if (!velocity_) {
      velocity_ = std::make_unique<double>();
      *velocity_ = obj["velocity"].GetDouble();
    }
  } else {
    velocity_.reset();
  }
  if (obj.HasMember("acc")) {
    if (!acc_) {
      acc_ = std::make_unique<double>();
      *acc_ = obj["acc"].GetDouble();
    }
  } else {
    acc_.reset();
  }
  if (obj.HasMember("time")) {
    if (!time_) {
      time_ = std::make_unique<double>();
      *time_ = obj["time"].GetDouble();
    }
  } else {
    time_.reset();
  }
  if (obj.HasMember("radius")) {
    if (!radius_) {
      radius_ = std::make_unique<double>();
      *radius_ = obj["radius"].GetDouble();
    }
  } else {
    radius_.reset();
  }
  return true;
}

bool MoveParam::Serialize(
    rapidjson::Writer<rapidjson::StringBuffer>* writer) const {
  writer->StartObject();
  if (velocity_) {
    writer->String("velocity");
    writer->Double(*velocity_);
  }
  if (acc_) {
    writer->String("acc");
    writer->Double(*acc_);
  }
  if (time_) {
    writer->String("time");
    writer->Double(*time_);
  }
  if (radius_) {
    writer->String("radius");
    writer->Double(*radius_);
  }
  writer->EndObject();
  return true;
}
bool MoveParam::IsNullJSONData() const {
  if (velocity_ || acc_ || time_ || radius_) {
    return false;
  }
  return true;
}
// MoveParam end

// MoveRequest begin
void MoveRequest::set_param(const MoveParam& param) { param_ = param; }
const MoveParam& MoveRequest::param() const { return param_; }
MoveParam* MoveRequest::mutable_param() { return &param_; }

void MoveRequest::set_pose(const posture::Pose& pose) { pose_ = pose; }
const posture::Pose& MoveRequest::pose() const { return pose_; }
posture::Pose* MoveRequest::mutable_pose() { return &pose_; }

bool MoveRequest::Deserialize(const rapidjson::Value& obj) {
  if (obj.HasMember("param")) {
    param_.Deserialize(obj["param"]);
  }
  if (obj.HasMember("pose")) {
    pose_.Deserialize(obj["pose"]);
  }
  return true;
}

bool MoveRequest::Serialize(
    rapidjson::Writer<rapidjson::StringBuffer>* writer) const {
  writer->StartObject();
  if (!param_.IsNullJSONData()) {
    writer->String("param");
    param_.Serialize(writer);
  }
  if (!pose_.IsNullJSONData()) {
    writer->String("pose");
    pose_.Serialize(writer);
  }
  writer->EndObject();
  return true;
}
bool MoveRequest::IsNullJSONData() const {
  if (param_.IsNullJSONData() && pose_.IsNullJSONData()) {
    return true;
  }
  return false;
}
// MoveRequest end

// pose_via_
void MovecRequest::set_pose_via(const posture::Pose& pose) { pose_via_ = pose; }
const posture::Pose& MovecRequest::pose_via() const { return pose_via_; }
posture::Pose* MovecRequest::mutable_pose_via() { return &pose_via_; }
// pose_
void MovecRequest::set_pose(const posture::Pose& pose) { pose_ = pose; }
const posture::Pose& MovecRequest::pose() const { return pose_; }
posture::Pose* MovecRequest::mutable_pose() { return &pose_; }
// param_
void MovecRequest::set_param(const MoveParam& param) { param_ = param; }
const MoveParam& MovecRequest::param() const { return param_; }
MoveParam* MovecRequest::mutable_param() { return &param_; }
// rad_
void MovecRequest::set_rad(double rad) { rad_ = rad; }
double MovecRequest::rad() const { return rad_; }
double* MovecRequest::mutable_rad() { return &rad_; }
bool MovecRequest::Deserialize(const rapidjson::Value& obj) {
  if (obj.HasMember("pose_via")) {
    pose_via_.Deserialize(obj["pose_via"]);
  }
  if (obj.HasMember("pose")) {
    pose_.Deserialize(obj["pose"]);
  }
  if (obj.HasMember("param")) {
    param_.Deserialize(obj["param"]);
  }
  if (obj.HasMember("rad")) {
    rad_ = obj["rad"].GetDouble();
  }
  return true;
}
bool MovecRequest::Serialize(
    rapidjson::Writer<rapidjson::StringBuffer>* writer) const {
  writer->StartObject();
  if (!pose_via_.IsNullJSONData()) {
    writer->String("pose_via");
    pose_via_.Serialize(writer);
  }
  if (!pose_.IsNullJSONData()) {
    writer->String("pose");
    pose_.Serialize(writer);
  }
  if (!param_.IsNullJSONData()) {
    writer->String("param");
    param_.Serialize(writer);
  }
  writer->String("rad");
  writer->Double(rad_);
  writer->EndObject();
  return true;
}
bool MovecRequest::IsNullJSONData() const {
  if (pose_via_.IsNullJSONData() && pose_.IsNullJSONData() &&
      param_.IsNullJSONData() && std::abs(rad_) < 1e-6) {
    return true;
  }
  { return true; }
  return false;
}

void MotionIndex::set_id(unsigned int id) { id_ = id; }
unsigned int MotionIndex::id() const { return id_; }
unsigned int* MotionIndex::mutable_id() { return &id_; }

bool MotionIndex::Deserialize(const rapidjson::Value& obj) {
  if (obj.HasMember("id")) {
    unsigned int id_int = (unsigned int)(obj["id"].GetUint());
    id_ = id_int;
  }
  return true;
}
bool MotionIndex::Serialize(
    rapidjson::Writer<rapidjson::StringBuffer>* writer) const {
  writer->StartObject();
  writer->Key("id");
  writer->Uint(id_);
  writer->EndObject();
  return true;
}
bool MotionIndex::IsNullJSONData() const { return false; }

void GetMotionStateResponse::set_state(MotionState state) { state_ = state; }
const MotionState GetMotionStateResponse::state() const { return state_; }
MotionState* GetMotionStateResponse::mutable_state() { return &state_; }

bool GetMotionStateResponse::Deserialize(const rapidjson::Value& obj) {
  if (obj.HasMember("state")) {
    int state_int = (int)(obj["state"].GetInt());
    switch (state_int) {
      case 0:
        state_ = WAIT;
        break;
      case 1:
        state_ = RUNNING;
        break;
      case 2:
        state_ = FINISHED;
        break;
      default:
        state_ = WAIT;
        break;
    }
  }
  return true;
}
bool GetMotionStateResponse::Serialize(
    rapidjson::Writer<rapidjson::StringBuffer>* writer) const {
  writer->StartObject();
  writer->Key("state");
  writer->Int(state_);
  writer->EndObject();
  return true;
}
bool GetMotionStateResponse::IsNullJSONData() const { return false; }

void JointMove::set_pose(double pose) { pose_ = pose; }
const double JointMove::pose() const { return pose_; }
double* JointMove::mutable_pose() { return &pose_; }
void JointMove::set_velocity(double velocity) { velocity_ = velocity; }
const double JointMove::velocity() const { return velocity_; }
double* JointMove::mutable_velocity() { return &velocity_; }
void JointMove::set_acc(double acc) { acc_ = acc; }
const double JointMove::acc() const { return acc_; }
double* JointMove::mutable_acc() { return &acc_; }
bool JointMove::Deserialize(const rapidjson::Value& obj) {
  if (obj.HasMember("pose")) {
    pose_ = obj["pose"].GetDouble();
  }
  if (obj.HasMember("velocity")) {
    velocity_ = obj["velocity"].GetDouble();
  }
  if (obj.HasMember("acc")) {
    acc_ = obj["acc"].GetDouble();
  }
  return true;
}
bool JointMove::Serialize(
    rapidjson::Writer<rapidjson::StringBuffer>* writer) const {
  writer->StartObject();
  writer->Key("pose");
  writer->Double(pose_);
  writer->Key("velocity");
  writer->Double(velocity_);
  writer->Key("acc");
  writer->Double(acc_);
  writer->EndObject();
  return true;
}
bool JointMove::IsNullJSONData() const { return false; }
void MovePvatRequest::set_duration(double duration) { duration_ = duration; }
const double MovePvatRequest::duration() const { return duration_; }
double* MovePvatRequest::mutable_duration() { return &duration_; }
void MovePvatRequest::set_joints(std::vector<JointMove> joints) {
  joints_ = joints;
}
const std::vector<JointMove> MovePvatRequest::joints() const { return joints_; }
std::vector<JointMove>* MovePvatRequest::mutable_joints() { return &joints_; }
bool MovePvatRequest::Deserialize(const rapidjson::Value& obj) {
  if (obj.HasMember("duration")) {
    duration_ = (double)(obj["duration"].GetDouble());
  }
  if (obj.HasMember("joints")) {
    std::vector<JointMove> j;
    for (auto joint = obj["joints"].GetArray().Begin();
         joint != obj["joints"].GetArray().End(); joint++) {
      JointMove _j;
      _j.FromJSONString(joint->GetString());
      j.push_back(_j);
    }
    joints_ = j;
  }
  return true;
}
bool MovePvatRequest::Serialize(
    rapidjson::Writer<rapidjson::StringBuffer>* writer) const {
  writer->StartObject();
  writer->Key("duration");
  writer->Double(duration_);
  writer->Key("joints");
  writer->StartArray();
  for (auto j : joints_) {
    j.Serialize(writer);
  }
  writer->EndArray();
  writer->EndObject();
  return true;
}
bool MovePvatRequest::IsNullJSONData() const { return false; }

// SpeedParam begin
SpeedParam::SpeedParam() {}
SpeedParam::SpeedParam(const SpeedParam& other) { *this = other; }
SpeedParam& SpeedParam::operator=(const SpeedParam& other) {
  acc_.reset();
  if (other.acc_) {
    acc_ = std::make_unique<double>(*other.acc_);
  }
  time_.reset();
  if (other.time_) {
    time_ = std::make_unique<double>(*other.time_);
  }
  constrained_.reset();
  if (other.constrained_) {
    constrained_ = std::make_unique<bool>(*other.constrained_);
  }
  return *this;
}
void SpeedParam::set_acc(double acc) { acc_ = std::make_unique<double>(acc); }
// double v() const;
double* SpeedParam::acc() const { return acc_.get(); }
double* SpeedParam::mutable_acc() {
  if (acc_ == nullptr) {
    acc_ = std::make_unique<double>();
  }
  return acc_.get();
}
void SpeedParam::set_time(double time) {
  time_ = std::make_unique<double>(time);
}
double* SpeedParam::time() const { return time_.get(); }
double* SpeedParam::mutable_time() {
  if (time_ == nullptr) {
    time_ = std::make_unique<double>();
  }
  return time_.get();
}
void SpeedParam::set_constrained(bool constrained) {
  if (constrained_ == nullptr) {
    constrained_ = std::make_unique<bool>();
  }
  constrained_ = std::make_unique<bool>(constrained);
}
bool* SpeedParam::constrained() const { return constrained_.get(); }
bool* SpeedParam::mutable_constrained() {
  if (constrained_ == nullptr) {
    constrained_ = std::make_unique<bool>();
  }
  return constrained_.get();
}

bool SpeedParam::Deserialize(const rapidjson::Value& obj) {
  if (obj.HasMember("acc")) {
    if (!acc_) {
      acc_ = std::make_unique<double>();
      *acc_ = obj["acc"].GetDouble();
    }
  } else {
    acc_.reset();
  }
  if (obj.HasMember("time")) {
    if (!time_) {
      time_ = std::make_unique<double>();
      *time_ = obj["time"].GetDouble();
    }
  } else {
    time_.reset();
  }
  if (obj.HasMember("constrained")) {
    if (!constrained_) {
      constrained_ = std::make_unique<bool>();
      *constrained_ = obj["constrained"].GetBool();
    }
  } else {
    constrained_.reset();
  }
  return true;
}

bool SpeedParam::Serialize(
    rapidjson::Writer<rapidjson::StringBuffer>* writer) const {
  writer->StartObject();
  if (acc_) {
    writer->String("acc");
    writer->Double(*acc_);
  }
  if (time_) {
    writer->String("time");
    writer->Double(*time_);
  }
  if (constrained_) {
    writer->String("constrained");
    writer->Bool(*constrained_);
  }
  writer->EndObject();
  return true;
}
bool SpeedParam::IsNullJSONData() const {
  if (acc_ || time_ || constrained_) {
    return false;
  }
  return true;
}
// SpeedJRequest
void SpeedJRequest::set_speed(const posture::JointPose& speed) {
  speed_ = speed;
}
const posture::JointPose& SpeedJRequest::speed() const { return speed_; }

posture::JointPose* SpeedJRequest::mutable_speed() { return &speed_; }
void SpeedJRequest::set_param(const SpeedParam& param) { param_ = param; }
const SpeedParam& SpeedJRequest::param() const { return param_; }
SpeedParam* SpeedJRequest::mutable_param() { return &param_; }
bool SpeedJRequest::Deserialize(const rapidjson::Value& obj) {
  if (obj.HasMember("speed")) {
    speed_.Deserialize(obj["speed"]);
  }
  if (obj.HasMember("param")) {
    param_.Deserialize(obj["param"]);
  }
  return true;
}
bool SpeedJRequest::Serialize(
    rapidjson::Writer<rapidjson::StringBuffer>* writer) const {
  writer->StartObject();
  if (!speed_.IsNullJSONData()) {
    writer->String("speed");
    speed_.Serialize(writer);
  }
  if (!param_.IsNullJSONData()) {
    writer->String("param");
    param_.Serialize(writer);
  }
  writer->EndObject();
  return true;
}
bool SpeedJRequest::IsNullJSONData() const {
  if (speed_.IsNullJSONData() && param_.IsNullJSONData()) {
    return true;
  }
  return false;
}
// SpeedJRequest end
// SpeedJRequest
void SpeedLRequest::set_speed(const posture::CartesianPose& speed) {
  speed_ = speed;
}
const posture::CartesianPose& SpeedLRequest::speed() const { return speed_; }

posture::CartesianPose* SpeedLRequest::mutable_speed() { return &speed_; }
void SpeedLRequest::set_param(const SpeedParam& param) { param_ = param; }
const SpeedParam& SpeedLRequest::param() const { return param_; }
SpeedParam* SpeedLRequest::mutable_param() { return &param_; }
void SpeedLRequest::set_frame(const posture::CartesianFrame& frame) {
  frame_ = frame;
}
const posture::CartesianFrame& SpeedLRequest::frame() const { return frame_; }
posture::CartesianFrame* SpeedLRequest::mutable_frame() { return &frame_; }
bool SpeedLRequest::Deserialize(const rapidjson::Value& obj) {
  if (obj.HasMember("speed")) {
    speed_.Deserialize(obj["speed"]);
  }
  if (obj.HasMember("param")) {
    param_.Deserialize(obj["param"]);
  }
  if (obj.HasMember("frame")) {
    frame_.Deserialize(obj["frame"]);
  }
  return true;
}
bool SpeedLRequest::Serialize(
    rapidjson::Writer<rapidjson::StringBuffer>* writer) const {
  writer->StartObject();
  if (!speed_.IsNullJSONData()) {
    writer->String("speed");
    speed_.Serialize(writer);
  }
  if (!param_.IsNullJSONData()) {
    writer->String("param");
    param_.Serialize(writer);
  }
  if (!frame_.IsNullJSONData()) {
    writer->String("frame");
    frame_.Serialize(writer);
  }
  writer->EndObject();
  return true;
}
bool SpeedLRequest::IsNullJSONData() const {
  if (speed_.IsNullJSONData() && param_.IsNullJSONData()) {
    return true;
  }
  return false;
}
// SpeedJRequest end
}  // namespace motion
}  // namespace lebai