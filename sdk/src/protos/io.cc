#include <string>
#include "io.hh"


namespace lebai
{
  namespace io
  {
    
    void GetDioPinRequest::set_device(IoDevice device)
    {
      device_ = device;
    }
    IoDevice GetDioPinRequest::device() const
    {
      return device_;
    }
    IoDevice * GetDioPinRequest::mutable_device()
    {
      return &device_;
    }
    void GetDioPinRequest::set_pin(unsigned int pin)
    {
      pin_ = pin;
    }
    unsigned int GetDioPinRequest::pin() const
    {
      return pin_;
    }
    unsigned int * GetDioPinRequest::mutable_pin()
    {
      return &pin_;
    }

    // Deserialize
    bool GetDioPinRequest::Deserialize(const rapidjson::Value &obj)
    {
      if(obj.HasMember("device"))
      {
        std::string device_str = std::string(obj["device"].GetString());
        if(device_str == "ROBOT")
        {
          device_ = ROBOT;
        }
        else if(device_str == "FLANGE")
        {
          device_ = FLANGE;
        }
        else if(device_str == "EXTRA")
        {
          device_ = EXTRA;
        }
      }
      if(obj.HasMember("pin"))
      {
        pin_ = obj["pin"].GetUint();
      }
      return true;
    }
    // Serialize
    bool GetDioPinRequest::Serialize(rapidjson::Writer<rapidjson::StringBuffer> *writer) const
    {
      writer->StartObject();
      writer->Key("device");
      writer->Int(device_);
      writer->Key("pin");
      writer->Int(pin_);
      writer->EndObject();
      return true;
    }
    // IsNullJSONData
    bool GetDioPinRequest::IsNullJSONData() const
    {
      return false;
    }

    void GetDioPinResponse::set_value(unsigned int value)
    {
      value_ = value;
    }
    unsigned int GetDioPinResponse::value() const
    {
      return value_;
    }
    unsigned int * GetDioPinResponse::mutable_value()
    {
      return &value_;
    }
    // Deserialize
    bool GetDioPinResponse::Deserialize(const rapidjson::Value &obj)
    {
      if(obj.HasMember("value"))
      {
        value_ = obj["value"].GetUint();
      }
      return true;
    }
    // Serialize
    bool GetDioPinResponse::Serialize(rapidjson::Writer<rapidjson::StringBuffer> *writer) const
    {
      writer->StartObject();
      writer->Key("value");
      writer->Int(value_);
      writer->EndObject();
      return true;
    }
    // IsNullJSONData
    bool GetDioPinResponse::IsNullJSONData() const
    {
      return false;
    }


    void SetDoPinRequest::set_device(IoDevice device)
    {
      device_ = device;
    }
    IoDevice SetDoPinRequest::device() const
    {
      return device_;
    }
    IoDevice * SetDoPinRequest::mutable_device()
    {
      return &device_;
    }
    void SetDoPinRequest::set_pin(unsigned int pin)
    {
      pin_ = pin;
    }
    unsigned int SetDoPinRequest::pin() const
    {
      return pin_;
    }
    unsigned int * SetDoPinRequest::mutable_pin()
    {
      return &pin_;
    }
    void SetDoPinRequest::set_value(unsigned int value)
    {
      value_ = value;
    }
    unsigned int SetDoPinRequest::value() const
    {
      return value_;
    }
    unsigned int * SetDoPinRequest::mutable_value()
    {
      return &value_;
    }
    // Deserialize
    bool SetDoPinRequest::Deserialize(const rapidjson::Value &obj)
    {
      if(obj.HasMember("device"))
      {
        std::string device_str = std::string(obj["device"].GetString());
        if(device_str == "ROBOT")
        {
          device_ = ROBOT;
        }
        else if(device_str == "FLANGE")
        {
          device_ = FLANGE;
        }
        else if(device_str == "EXTRA")
        {
          device_ = EXTRA;
        }
      }
      if(obj.HasMember("pin"))
      {
        pin_ = obj["pin"].GetUint();
      }
      if(obj.HasMember("value"))
      {
        value_ = obj["value"].GetUint();
      }
      return true;
    }
    // Serialize
    bool SetDoPinRequest::Serialize(rapidjson::Writer<rapidjson::StringBuffer> *writer) const
    {
      writer->StartObject();
      writer->Key("device");
      writer->Int(device_);
      writer->Key("pin");
      writer->Int(pin_);
      writer->Key("value");
      writer->Int(value_);
      writer->EndObject();
      return true;
    }
    // IsNullJSONData
    bool SetDoPinRequest::IsNullJSONData() const
    {
      return false;
    }

    void GetAioPinRequest::set_device(IoDevice device)
    {
      device_ = device;
    }
    IoDevice GetAioPinRequest::device() const
    {
      return device_;
    }
    IoDevice * GetAioPinRequest::mutable_device()
    {
      return &device_;
    }
    void GetAioPinRequest::set_pin(unsigned int pin)
    {
      pin_ = pin;
    }
    unsigned int GetAioPinRequest::pin() const
    {
      return pin_;
    }
    unsigned int * GetAioPinRequest::mutable_pin()
    {
      return &pin_;
    }
    // Deserialize
    bool GetAioPinRequest::Deserialize(const rapidjson::Value &obj)
    {
      if(obj.HasMember("device"))
      {
        std::string device_str = std::string(obj["device"].GetString());
        if(device_str == "ROBOT")
        {
          device_ = ROBOT;
        }
        else if(device_str == "FLANGE")
        {
          device_ = FLANGE;
        }
        else if(device_str == "EXTRA")
        {
          device_ = EXTRA;
        }
      }
      if(obj.HasMember("pin"))
      {
        pin_ = obj["pin"].GetUint();
      }
      return true;
    }
    // Serialize
    bool GetAioPinRequest::Serialize(rapidjson::Writer<rapidjson::StringBuffer> *writer) const
    {
      writer->StartObject();
      writer->Key("device");
      writer->Int(device_);
      writer->Key("pin");
      writer->Int(pin_);
      writer->EndObject();
      return true;
    }
    // IsNullJSONData
    bool GetAioPinRequest::IsNullJSONData() const
    {
      return false;
    }

    void GetAioPinResponse::set_value(double value)
    {
      value_ = value;
    }
    double GetAioPinResponse::value() const
    {
      return value_;
    }
    double * GetAioPinResponse::mutable_value()
    {
      return &value_;
    }
    // Deserialize
    bool GetAioPinResponse::Deserialize(const rapidjson::Value &obj)
    {
      if(obj.HasMember("value"))
      {
        value_ = obj["value"].GetDouble();
      }
      return true;
    }
    // Serialize
    bool GetAioPinResponse::Serialize(rapidjson::Writer<rapidjson::StringBuffer> *writer) const
    {
      writer->StartObject();
      writer->Key("value");
      writer->Double(value_);
      writer->EndObject();
      return true;
    }
    // IsNullJSONData
    bool GetAioPinResponse::IsNullJSONData() const
    {
      return false;
    }


    void SetAoPinRequest::set_device(IoDevice device)
    {
      device_ = device;
    }
    IoDevice SetAoPinRequest::device() const
    {
      return device_;
    }
    IoDevice * SetAoPinRequest::mutable_device()
    {
      return &device_;
    }
    void SetAoPinRequest::set_pin(unsigned int pin)
    {
      pin_ = pin;
    }
    unsigned int SetAoPinRequest::pin() const
    {
      return pin_;
    }
    unsigned int * SetAoPinRequest::mutable_pin()
    {
      return &pin_;
    }
    void SetAoPinRequest::set_value(double value)
    {
      value_ = value;
    }
    double SetAoPinRequest::value() const
    {
      return value_;
    }
    double * SetAoPinRequest::mutable_value()
    {
      return &value_;
    }
    // Deserialize
    bool SetAoPinRequest::Deserialize(const rapidjson::Value &obj)
    {
      if(obj.HasMember("device"))
      {
        std::string device_str = std::string(obj["device"].GetString());
        if(device_str == "ROBOT")
        {
          device_ = ROBOT;
        }
        else if(device_str == "FLANGE")
        {
          device_ = FLANGE;
        }
        else if(device_str == "EXTRA")
        {
          device_ = EXTRA;
        }
      }
      if(obj.HasMember("pin"))
      {
        pin_ = obj["pin"].GetUint();
      }
      if(obj.HasMember("value"))
      {
        value_ = obj["value"].GetDouble();
      }
      return true;
    }
    // Serialize
    bool SetAoPinRequest::Serialize(rapidjson::Writer<rapidjson::StringBuffer> *writer) const
    {
      writer->StartObject();
      writer->Key("device");
      writer->Int(device_);
      writer->Key("pin");
      writer->Int(pin_);
      writer->Key("value");
      writer->Double(value_);
      writer->EndObject();
      return true;
    }
    // IsNullJSONData
    bool SetAoPinRequest::IsNullJSONData() const
    {
      return false;
    }

  }
}