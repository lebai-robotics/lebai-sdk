#pragma once

#include "jsonbase.hh"
#include <vector>
#include <memory>

namespace lebai {
namespace claw {

class InitClawRequest : public JSONBase {
 public:
  void set_force_initilization(bool force_initilization);
  bool force_initilization() const;
  bool *mutable_force_initilization();

 protected:
  bool force_initilization_;
  // These methods are used to serialize and deserialize the class.
  // They will not be wrapped in the SDK.
 public:
  virtual bool Deserialize(const rapidjson::Value &obj);
  virtual bool Serialize(
      rapidjson::Writer<rapidjson::StringBuffer> *writer) const;
  virtual bool IsNullJSONData() const;
};

class SetClawRequest : public JSONBase {
 public:
  void set_force(double force);
  double force() const;
  double *mutable_force();

  void set_amplitude(double amplitude);
  double amplitude() const;
  double *mutable_amplitude();

 protected:
  double force_;
  double amplitude_;
  // These methods are used to serialize and deserialize the class.
  // They will not be wrapped in the SDK.
 public:
  virtual bool Deserialize(const rapidjson::Value &obj);
  virtual bool Serialize(
      rapidjson::Writer<rapidjson::StringBuffer> *writer) const;
  virtual bool IsNullJSONData() const;
};

class Claw : public JSONBase {
 public:
  void set_force(double force);
  double force() const;
  double *mutable_force();

  void set_amplitude(double amplitude);
  double amplitude() const;
  double *mutable_amplitude();

  void set_hold_on(bool hold_on);
  bool hold_on() const;
  bool *mutable_hold_on();

 protected:
  double force_;
  double amplitude_;
  bool hold_on_;
  // These methods are used to serialize and deserialize the class.
  // They will not be wrapped in the SDK.
 public:
  virtual bool Deserialize(const rapidjson::Value &obj);
  virtual bool Serialize(
      rapidjson::Writer<rapidjson::StringBuffer> *writer) const;
  virtual bool IsNullJSONData() const;
};
}  // namespace claw
}  // namespace lebai