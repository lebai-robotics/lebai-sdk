#pragma once

#include "jsonbase.hh"
#include <vector>

namespace lebai {
namespace safety {
enum CollisionDetectorAction {
  ESTOP = 0,
  PAUSE = 1,
  STOP_MOVE = 2,
  NONE = 99,
};

class CartesianLimit : public JSONBase {
 public:
  void set_max_a(double max_a);
  double max_a() const;
  double *mutable_max_a();

  void set_max_v(double max_v);
  double max_v() const;
  double *mutable_max_v();

  void set_eq_radius(double eq_radius);
  double eq_radius() const;
  double *mutable_eq_radius();

 protected:
  double max_a_ = 0.0;
  double max_v_ = 0.0;
  double eq_radius_ = 0.0;

 public:
  virtual bool Deserialize(const rapidjson::Value &obj);
  virtual bool Serialize(
      rapidjson::Writer<rapidjson::StringBuffer> *writer) const;
  virtual bool IsNullJSONData() const;
};

class CollisionDetector : public JSONBase {
 public:
  void set_action(CollisionDetectorAction action);
  CollisionDetectorAction action() const;
  CollisionDetectorAction *mutable_action();

  void set_pause_time(unsigned int pause_time);
  unsigned int pause_time() const;
  unsigned int *mutable_pause_time();

  void set_sensitivity(unsigned int sensitivity);
  unsigned int sensitivity() const;
  unsigned int *mutable_sensitivity();

 protected:
  CollisionDetectorAction action_ = CollisionDetectorAction::NONE;
  unsigned int pause_time_ = 0;
  unsigned int sensitivity_ = 0;

 public:
  virtual bool Deserialize(const rapidjson::Value &obj);
  virtual bool Serialize(
      rapidjson::Writer<rapidjson::StringBuffer> *writer) const;
  virtual bool IsNullJSONData() const;
};

class CollisionTorqueDiff : public JSONBase {
 public:
  void set_diffs(const std::vector<double> &diffs);
  std::vector<double> diffs() const;
  std::vector<double> *mutable_diffs();

 protected:
  std::vector<double> diffs_;

 public:
  virtual bool Deserialize(const rapidjson::Value &obj);
  virtual bool Serialize(
      rapidjson::Writer<rapidjson::StringBuffer> *writer) const;
  virtual bool IsNullJSONData() const;
};

class JointLimit : public JSONBase {
 public:
  void set_min_position(double min_position);
  double min_position() const;
  double *mutable_min_position();

  void set_max_position(double max_position);
  double max_position() const;
  double *mutable_max_position();

  void set_max_a(double max_a);
  double max_a() const;
  double *mutable_max_a();

  void set_max_v(double max_v);
  double max_v() const;
  double *mutable_max_v();

 protected:
  double min_position_ = 0.0;
  double max_position_ = 0.0;
  double max_a_ = 0.0;
  double max_v_ = 0.0;

 public:
  virtual bool Deserialize(const rapidjson::Value &obj);
  virtual bool Serialize(
      rapidjson::Writer<rapidjson::StringBuffer> *writer) const;
  virtual bool IsNullJSONData() const;
};

class JointsLimit : public JSONBase {
 public:
  void set_joints(const std::vector<JointLimit> &joints);
  std::vector<JointLimit> joints() const;
  std::vector<JointLimit> *mutable_joints();

 protected:
  std::vector<JointLimit> joints_;

 public:
  virtual bool Deserialize(const rapidjson::Value &obj);
  virtual bool Serialize(
      rapidjson::Writer<rapidjson::StringBuffer> *writer) const;
  virtual bool IsNullJSONData() const;
};
}  // namespace safety
}  // namespace lebai