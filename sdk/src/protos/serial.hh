#pragma once

#include "jsonbase.hh"
#include <vector>
#include <memory>

namespace lebai {
namespace serial {
class SetSerialBaudRateRequest : public JSONBase {
 public:
  void set_device(std::string device);
  std::string device() const;
  std::string *mutable_device();

  void set_baud_rate(unsigned int baud_rate);
  unsigned int baud_rate() const;
  unsigned int *mutable_baud_rate();

 protected:
  std::string device_;
  unsigned int baud_rate_ = 115200;
  // These methods are used to serialize and deserialize the class.
  // They will not be wrapped in the SDK.
 public:
  virtual bool Deserialize(const rapidjson::Value &obj);
  virtual bool Serialize(
      rapidjson::Writer<rapidjson::StringBuffer> *writer) const;
  virtual bool IsNullJSONData() const;
};

enum Parity {
  None = 0,
  Odd = 1,
  Even = 2,
};

class SetSerialParityRequest : public JSONBase {
 public:
  void set_device(std::string device);
  std::string device() const;
  std::string *mutable_device();

  void set_parity(Parity parity);
  Parity parity() const;
  Parity *mutable_parity();

 protected:
  std::string device_;
  Parity parity_ = Parity::None;
  // These methods are used to serialize and deserialize the class.
  // They will not be wrapped in the SDK.
 public:
  virtual bool Deserialize(const rapidjson::Value &obj);
  virtual bool Serialize(
      rapidjson::Writer<rapidjson::StringBuffer> *writer) const;
  virtual bool IsNullJSONData() const;
};
}  // namespace serial
}  // namespace lebai