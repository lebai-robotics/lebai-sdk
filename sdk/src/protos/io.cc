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

    void GetDioPinsRequest::set_device(IoDevice device)
    {
      device_ = device;
    }
    IoDevice GetDioPinsRequest::device() const
    {
      return device_;
    }
    IoDevice * GetDioPinsRequest::mutable_device()
    {
      return &device_;
    }
    void GetDioPinsRequest::set_pin(unsigned int pin)
    {
      pin_ = pin;
    }
    unsigned int GetDioPinsRequest::pin() const
    {
      return pin_;
    }
    unsigned int * GetDioPinsRequest::mutable_pin()
    {
      return &pin_;
    }

    void GetDioPinsRequest::set_count(unsigned int count)
    {
      count_ = count;
    }
    unsigned int GetDioPinsRequest::count() const
    {
      return count_;
    }
    unsigned int * GetDioPinsRequest::mutable_count()
    {
      return &count_;
    }
    // Deserialize
    bool GetDioPinsRequest::Deserialize(const rapidjson::Value &obj)
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
      if(obj.HasMember("count"))
      {
        count_ = obj["count"].GetUint();
      }
      return true;
    }
    // Serialize
    bool GetDioPinsRequest::Serialize(rapidjson::Writer<rapidjson::StringBuffer> *writer) const
    {
      writer->StartObject();
      writer->Key("device");
      writer->Int(device_);
      writer->Key("pin");
      writer->Uint(pin_);
      writer->Key("count");
      writer->Uint(count_);
      writer->EndObject();
      return true;
    }
    // IsNullJSONData
    bool GetDioPinsRequest::IsNullJSONData() const
    {
      return false;
    }
    void GetDioPinsResponse::set_values(std::vector<unsigned int> values)
    {
      values_ = values;
    }
    std::vector<unsigned int> GetDioPinsResponse::values() const
    {
      return values_;
    }
    std::vector<unsigned int> * GetDioPinsResponse::mutable_values()
    {
      return &values_;
    }
    // Deserialize
    bool GetDioPinsResponse::Deserialize(const rapidjson::Value &obj)
    {
      if(obj.HasMember("values"))
      {
        std::vector<unsigned int> values;
        for(auto i = obj["values"].GetArray().Begin();i != obj["values"].GetArray().End();i++)
        {
          values.push_back(i->GetUint());
        }
        values_ = values;
      }
      return true;
    }
    // Serialize
    bool GetDioPinsResponse::Serialize(rapidjson::Writer<rapidjson::StringBuffer> *writer) const
    {
      writer->StartObject();
      writer->Key("value");
      writer->StartArray();
      for(auto i:values_)
      {
        writer->Uint(i);
      }
      writer->EndArray();
      writer->EndObject();
      return true;
    }
    // IsNullJSONData
    bool GetDioPinsResponse::IsNullJSONData() const
    {
      return false;
    }

    void SetDoPinsRequest::set_device(IoDevice device)
    {
      device_ = device;
    }
    IoDevice SetDoPinsRequest::device() const
    {
      return device_;
    }
    IoDevice * SetDoPinsRequest::mutable_device()
    {
      return &device_;
    }
    void SetDoPinsRequest::set_pin(unsigned int pin)
    {
      pin_ = pin;
    }
    unsigned int SetDoPinsRequest::pin() const
    {
      return pin_;
    }
    unsigned int * SetDoPinsRequest::mutable_pin()
    {
      return &pin_;
    }
    void SetDoPinsRequest::set_values(std::vector<unsigned int> values)
    {
      values_ = values;
    }
    std::vector<unsigned int> SetDoPinsRequest::values() const
    {
      return values_;
    }
    std::vector<unsigned int> * SetDoPinsRequest::mutable_values()
    {
      return &values_;
    }
    // Deserialize
    bool SetDoPinsRequest::Deserialize(const rapidjson::Value &obj)
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
      if(obj.HasMember("values"))
      {
        std::vector<unsigned int> values;
        for(auto i = obj["values"].GetArray().Begin();i != obj["values"].GetArray().End();i++)
        {
          values.push_back(i->GetUint());
        }
        values_ = values;
      }
      return true;
    }
    // Serialize
    bool SetDoPinsRequest::Serialize(rapidjson::Writer<rapidjson::StringBuffer> *writer) const
    {
      writer->StartObject();
      writer->Key("device");
      writer->Int(device_);
      writer->Key("pin");
      writer->Int(pin_);
      writer->Key("value");
      writer->StartArray();
      for(auto i:values_)
      {
        writer->Uint(i);
      }
      writer->EndArray();
      writer->EndObject();
      return true;
    }
    // IsNullJSONData
    bool SetDoPinsRequest::IsNullJSONData() const
    {
      return false;
    }
    void GetAioPinsRequest::set_device(IoDevice device)
    {
      device_ = device;
    }
    IoDevice GetAioPinsRequest::device() const
    {
      return device_;
    }
    IoDevice * GetAioPinsRequest::mutable_device()
    {
      return &device_;
    }
    void GetAioPinsRequest::set_pin(unsigned int pin)
    {
      pin_ = pin;
    }
    unsigned int GetAioPinsRequest::pin() const
    {
      return pin_;
    }
    unsigned int * GetAioPinsRequest::mutable_pin()
    {
      return &pin_;
    }
    void GetAioPinsRequest::set_count(unsigned int count)
    {
      count_ = count;
    }
    unsigned int GetAioPinsRequest::count() const
    {
      return count_;
    }
    unsigned int * GetAioPinsRequest::mutable_count()
    {
      return &count_;
    }
    // Deserialize
    bool GetAioPinsRequest::Deserialize(const rapidjson::Value &obj)
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
      if(obj.HasMember("count"))
      {
        count_ = obj["count"].GetUint();
      }
      return true;
    }
    // Serialize
    bool GetAioPinsRequest::Serialize(rapidjson::Writer<rapidjson::StringBuffer> *writer) const
    {
      writer->StartObject();
      writer->Key("device");
      writer->Int(device_);
      writer->Key("pin");
      writer->Uint(pin_);
      writer->Key("count");
      writer->Uint(count_);
      writer->EndObject();
      return true;
    }
    // IsNullJSONData
    bool GetAioPinsRequest::IsNullJSONData() const
    {
      return false;
    }
    void GetAioPinsResponse::set_values(std::vector<double> values)
    {
      values_ = values;
    }
    std::vector<double> GetAioPinsResponse::values() const
    {
      return values_;
    }
    std::vector<double> * GetAioPinsResponse::mutable_values()
    {
      return &values_;
    }
    // Deserialize
    bool GetAioPinsResponse::Deserialize(const rapidjson::Value &obj)
    {
      if(obj.HasMember("values"))
      {
        std::vector<double> values;
        for(auto i = obj["values"].GetArray().Begin();i != obj["values"].GetArray().End();i++)
        {
          values.push_back(i->GetDouble());
        }
        values_ = values;
      }
      return true;
    }
    // Serialize
    bool GetAioPinsResponse::Serialize(rapidjson::Writer<rapidjson::StringBuffer> *writer) const
    {
      writer->StartObject();
      writer->Key("values");
      writer->StartArray();
      for(auto i:values_)
      {
        writer->Double(i);
      }
      writer->EndArray();
      writer->EndObject();
      return true;
    }
    // IsNullJSONData
    bool GetAioPinsResponse::IsNullJSONData() const
    {
      return false;
    }
    void SetAoPinsRequest::set_device(IoDevice device)
    {
      device_ = device;
    }
    IoDevice SetAoPinsRequest::device() const
    {
      return device_;
    }
    IoDevice * SetAoPinsRequest::mutable_device()
    {
      return &device_;
    }
    void SetAoPinsRequest::set_pin(unsigned int pin)
    {
      pin_ = pin;
    }
    unsigned int SetAoPinsRequest::pin() const
    {
      return pin_;
    }
    unsigned int * SetAoPinsRequest::mutable_pin()
    {
      return &pin_;
    }
    void SetAoPinsRequest::set_values(std::vector<double> values)
    {
      values_ = values;
    }
    std::vector<double> SetAoPinsRequest::values() const
    {
      return values_;
    }
    std::vector<double> * SetAoPinsRequest::mutable_values()
    {
      return &values_;
    }
    // Deserialize
    bool SetAoPinsRequest::Deserialize(const rapidjson::Value &obj)
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
      if(obj.HasMember("values"))
      {
        std::vector<double> values;
        for(auto i = obj["values"].GetArray().Begin();i != obj["values"].GetArray().End();i++)
        {
          values.push_back(i->GetDouble());
        }
        values_ = values;
      }
      return true;
    }
    // Serialize
    bool SetAoPinsRequest::Serialize(rapidjson::Writer<rapidjson::StringBuffer> *writer) const
    {
      writer->StartObject();
      writer->Key("device");
      writer->Int(device_);
      writer->Key("pin");
      writer->Int(pin_);
      writer->Key("values");
      writer->StartArray();
      for(auto i:values_)
      {
        writer->Double(i);
      }
      writer->EndArray();
      writer->EndObject();
      return true;
    }
    // IsNullJSONData
    bool SetAoPinsRequest::IsNullJSONData() const
    {
      return false;
    }

    void SetDioModeRequest::set_device(IoDevice device)
    {
      device_ = device;
    }
    IoDevice SetDioModeRequest::device()
    {
      return device_;
    }
    IoDevice* SetDioModeRequest::mutable_device()
    {
      return &device_;
    }

    void SetDioModeRequest::set_pin(unsigned int pin)
    {
      pin_ = pin;
    }
    unsigned int SetDioModeRequest::pin() const
    {
      return pin_;
    }
    unsigned int *SetDioModeRequest::mutable_pin()
    {
      return &pin_;
    }

    void SetDioModeRequest::set_value(bool value)
    {
      value_ = value;
    }
    bool SetDioModeRequest::value() const
    {
      return value_;
    }
    bool *SetDioModeRequest::mutable_value()
    {
      return &value_;
    }

    bool SetDioModeRequest::Deserialize(const rapidjson::Value &obj)
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
        value_ = obj["value"].GetBool();
      }
      return true;
    }
    bool SetDioModeRequest::Serialize(rapidjson::Writer<rapidjson::StringBuffer> *writer) const
    {
      writer->StartObject();
      writer->Key("device");
      writer->Int(device_);
      writer->Key("pin");
      writer->Uint(pin_);
      writer->Key("value");
      writer->Bool(value_);
      writer->EndObject();
      return true;
    }
    bool SetDioModeRequest::IsNullJSONData() const
    {
      return false;
    }

    void SetDioModeResponse::set_success(bool success)
    {
      success_ = success;
    }
    bool SetDioModeResponse::success() const
    {
      return success_;
    }
    bool *SetDioModeResponse::mutable_success()
    {
      return &success_;
    }
    bool SetDioModeResponse::Deserialize(const rapidjson::Value &obj)
    {
      if(obj.HasMember("success"))
      {
        success_ = obj["success"].GetBool();
      }
      return true;
    }
    bool SetDioModeResponse::Serialize(rapidjson::Writer<rapidjson::StringBuffer> *writer) const
    {
      writer->StartObject();
      writer->Key("success");
      writer->Bool(success_);
      writer->EndObject();
      return true;
    }
    bool SetDioModeResponse::IsNullJSONData() const
    {
      return false;
    }

    void SetDioRequest::set_device(IoDevice device)
    {
      device_ = device;
    }
    IoDevice SetDioRequest::device()
    {
      return device_;
    }
    IoDevice* SetDioRequest::mutable_device()
    {
      return &device_;
    }
    void SetDioRequest::set_pin(unsigned int pin)
    {
      pin_ = pin;
    }
    unsigned int SetDioRequest::pin() const
    {
      return pin_;
    }
    unsigned int *SetDioRequest::mutable_pin()
    {
      return &pin_;
    }

    void SetDioRequest::set_value(bool value)
    {
      value_ = value;
    }
    bool SetDioRequest::value() const
    {
      return value_;
    }
    bool *SetDioRequest::mutable_value()
    {
      return &value_;
    }

    bool SetDioRequest::Deserialize(const rapidjson::Value &obj)
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
        value_ = obj["value"].GetBool();
      }
      return true;
    }
    bool SetDioRequest::Serialize(rapidjson::Writer<rapidjson::StringBuffer> *writer) const
    {
      writer->StartObject();
      writer->Key("device");
      writer->Int(device_);
      writer->Key("pin");
      writer->Uint(pin_);
      writer->Key("value");
      writer->Bool(value_);
      writer->EndObject();
      return true;
    }
    bool SetDioRequest::IsNullJSONData() const
    {
      return false;
    }

    void SetDioResponse::set_success(bool success)
    {
      success_ = success;
    }
    bool SetDioResponse::success() const
    {
      return success_;
    }
    bool *SetDioResponse::mutable_success()
    {
      return &success_;
    }
    bool SetDioResponse::Deserialize(const rapidjson::Value &obj)
    {
      if(obj.HasMember("success"))
      {
        success_ = obj["success"].GetBool();
      }
      return true;
    }
    bool SetDioResponse::Serialize(rapidjson::Writer<rapidjson::StringBuffer> *writer) const
    {
      writer->StartObject();
      writer->Key("success");
      writer->Bool(success_);
      writer->EndObject();
      return true;
    }
    bool SetDioResponse::IsNullJSONData() const
    {
      return false;
    }

    void GetDiosRequest::set_pin(unsigned int pin)
    {
      pin_ = pin;
    }
    unsigned int GetDiosRequest::pin() const
    {
      return pin_;
    }
    unsigned int *GetDiosRequest::mutable_pin()
    {
      return &pin_;
    }

    void GetDiosRequest::set_count(unsigned int count)
    {
      count_ = count;
    }
    unsigned int GetDiosRequest::count() const
    {
      return count_;
    }
    unsigned int *GetDiosRequest::mutable_count()
    {
      return &count_;
    }

    bool GetDiosRequest::Deserialize(const rapidjson::Value &obj)
    {
      if(obj.HasMember("pin"))
      {
        pin_ = obj["pin"].GetUint();
      }
      if(obj.HasMember("count"))
      {
        count_ = obj["count"].GetUint();
      }
      return true;
    }
    bool GetDiosRequest::Serialize(rapidjson::Writer<rapidjson::StringBuffer> *writer) const
    {
      writer->StartObject();
      writer->Key("pin");
      writer->Uint(pin_);
      writer->Key("count");
      writer->Uint(count_);
      writer->EndObject();
      return true;
    }
    bool GetDiosRequest::IsNullJSONData() const
    {
      return false;
    }

    void GetDiosResponse::set_values(std::vector<bool> values)
    {
      values_ = values;
    }
    std::vector<bool> GetDiosResponse::values() const
    {
      return values_;
    }
    std::vector<bool> *GetDiosResponse::mutable_values()
    {
      return &values_;
    }
    bool GetDiosResponse::Deserialize(const rapidjson::Value &obj)
    {
      if(obj.HasMember("values"))
      {
        std::vector<bool> values;
        for(auto i = obj["values"].GetArray().Begin();i != obj["values"].GetArray().End();i++)
        {
          values.push_back(i->GetBool());
        }
        values_ = values;
      }
      return true;
    }
    bool GetDiosResponse::Serialize(rapidjson::Writer<rapidjson::StringBuffer> *writer) const
    {
      writer->StartObject();
      writer->Key("values");
      writer->StartArray();
      for(auto i:values_)
      {
        writer->Bool(i);
      }
      writer->EndArray();
      writer->EndObject();
      return true;
    }
    bool GetDiosResponse::IsNullJSONData() const
    {
      return false;
    }

    void GetDiosModeRequest::set_device(IoDevice device)
    {
      device_ = device;
    }
    IoDevice GetDiosModeRequest::device()
    {
      return device_;
    }
    IoDevice* GetDiosModeRequest::mutable_device()
    {
      return &device_;
    }
    void GetDiosModeRequest::set_pin(unsigned int pin)
    {
      pin_ = pin;
    }
    unsigned int GetDiosModeRequest::pin() const
    {
      return pin_;
    }
    unsigned int *GetDiosModeRequest::mutable_pin()
    {
      return &pin_;
    }

    void GetDiosModeRequest::set_count(unsigned int count)
    {
      count_ = count;
    }
    unsigned int GetDiosModeRequest::count() const
    {
      return count_;
    }
    unsigned int *GetDiosModeRequest::mutable_count()
    {
      return &count_;
    }

    bool GetDiosModeRequest::Deserialize(const rapidjson::Value &obj)
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
      if(obj.HasMember("count"))
      {
        count_ = obj["count"].GetUint();
      }
      return true;
    }
    bool GetDiosModeRequest::Serialize(rapidjson::Writer<rapidjson::StringBuffer> *writer) const
    {
      writer->StartObject();
      writer->Key("device");
      writer->Int(device_);
      writer->Key("pin");
      writer->Uint(pin_);
      writer->Key("count");
      writer->Uint(count_);
      writer->EndObject();
      return true;
    }
    bool GetDiosModeRequest::IsNullJSONData() const
    {
      return false;
    }

    void GetDiosModeResponse::set_values(std::vector<bool> values)
    {
      values_ = values;
    }
    std::vector<bool> GetDiosModeResponse::values() const
    {
      return values_;
    }
    std::vector<bool> *GetDiosModeResponse::mutable_values()
    {
      return &values_;
    }
    bool GetDiosModeResponse::Deserialize(const rapidjson::Value &obj)
    {
      if(obj.HasMember("values"))
      {
        std::vector<bool> values;
        for(auto i = obj["values"].GetArray().Begin();i != obj["values"].GetArray().End();i++)
        {
          values.push_back(i->GetBool());
        }
        values_ = values;
      }
      return true;
    }
    bool GetDiosModeResponse::Serialize(rapidjson::Writer<rapidjson::StringBuffer> *writer) const
    {
      writer->StartObject();
      writer->Key("values");
      writer->StartArray();
      for(auto i:values_)
      {
        writer->Bool(i);
      }
      writer->EndArray();
      writer->EndObject();
      return true;
    }
    bool GetDiosModeResponse::IsNullJSONData() const
    {
      return false;
    }
  }
}