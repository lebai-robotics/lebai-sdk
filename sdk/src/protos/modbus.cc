#include <string>
#include "modbus.hh"


namespace lebai
{
  namespace modbus{
      void SetCoilsRequest::set_device(std::string device)
      {
        device_ = device;
      }
      std::string SetCoilsRequest::device() const
      {
        return device_;
      }
      std::string *SetCoilsRequest::mutable_device()
      {
        return &device_;
      }

      void SetCoilsRequest::set_pin(std::string pin)
      {
        pin_ = pin;
      }
      std::string SetCoilsRequest::pin() const
      {
        return pin_;
      }
      std::string *SetCoilsRequest::mutable_pin()
      {
        return &pin_;
      }

      void SetCoilsRequest::set_values(std::vector<bool> values)
      {
        values = values_;
      }
      std::vector<bool> SetCoilsRequest::values() const
      {
        return values_;
      }
      std::vector<bool> *SetCoilsRequest::mutable_values()
      {
        return &values_;
      }

      bool SetCoilsRequest::Deserialize(const rapidjson::Value &obj)
      {
        if(obj.HasMember("device"))
        {
            std::string device_str = std::string(obj["device"].GetString());
            device_ = device_str;
        }
        if(obj.HasMember("pin"))
        {
            std::string pin_str = std::string(obj["pin"].GetString());
            pin_ = pin_str;
        }
        if(obj.HasMember("values"))
        {
            std::vector<bool> values;
            for(auto i = obj["values"].GetArray().Begin();i != obj["values"].GetArray().End();i++)
            {
                bool value_bool = bool(i->GetBool());
                values.push_back(value_bool);
            }
            values_ = values;
        }
        return true;
      }
      bool SetCoilsRequest::Serialize(rapidjson::Writer<rapidjson::StringBuffer> *writer) const
      {

            writer->StartObject();
            writer->Key("device");
            writer->String(device_.c_str());
            writer->Key("pin");
            writer->String(pin_.c_str());
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
    bool SetCoilsRequest::IsNullJSONData() const
    {
        return values_.size() == 0;
    }
    void SetCoilRequest::set_device(std::string device)
    {
        device_ = device;
    }
    std::string SetCoilRequest::device() const
    {
        return device_;
    }
    std::string *SetCoilRequest::mutable_device()
    {
        return &device_;
    }

    void SetCoilRequest::set_pin(std::string pin)
    {
        pin_ = pin;
    }
    std::string SetCoilRequest::pin() const
    {
        return pin_;
    }
    std::string *SetCoilRequest::mutable_pin()
    {
        return &pin_;
    }

    void SetCoilRequest::set_value(bool values)
    {
        values = value_;
    }
    bool SetCoilRequest::value() const
    {
        return value_;
    }
    bool *SetCoilRequest::mutable_value()
    {
        return &value_;
    }

    bool SetCoilRequest::Deserialize(const rapidjson::Value &obj)
    {
        if (obj.HasMember("device"))
        {
                std::string device_str = std::string(obj["device"].GetString());
                device_ = device_str;
        }
        if (obj.HasMember("pin"))
        {
                std::string pin_str = std::string(obj["pin"].GetString());
                pin_ = pin_str;
        }
        if (obj.HasMember("values"))
        {
                bool values = bool(obj["values"].GetBool());
                value_ = values;
        }
        return true;
    }
    bool SetCoilRequest::Serialize(rapidjson::Writer<rapidjson::StringBuffer> *writer) const
    {

        writer->StartObject();
        writer->Key("device");
        writer->String(device_.c_str());
        writer->Key("pin");
        writer->String(pin_.c_str());
        writer->Key("value");
        writer->Bool(value_);
        writer->EndObject();
        return true;
    }
    bool SetCoilRequest::IsNullJSONData() const
    {
        return false;
    }
    void GetCoilsRequest::set_device(std::string device)
    {
        device_ = device;
    }
    std::string GetCoilsRequest::device() const
    {
        return device_;
    }
    std::string *GetCoilsRequest::mutable_device()
    {
        return &device_;
    }

    void GetCoilsRequest::set_pin(std::string pin)
    {
        pin_ = pin;
    }
    std::string GetCoilsRequest::pin() const
    {
        return pin_;
    }
    std::string *GetCoilsRequest::mutable_pin()
    {
        return &pin_;
    }

    void GetCoilsRequest::set_count(unsigned int count)
    {
        count = count_;
    }
    unsigned int GetCoilsRequest::count() const
    {
        return count_;
    }
    unsigned int *GetCoilsRequest::mutable_count()
    {
        return &count_;
    }

    bool GetCoilsRequest::Deserialize(const rapidjson::Value &obj)
    {
        if (obj.HasMember("device"))
        {
                std::string device_str = std::string(obj["device"].GetString());
                device_ = device_str;
        }
        if (obj.HasMember("pin"))
        {
                std::string pin_str = std::string(obj["pin"].GetString());
                pin_ = pin_str;
        }
        if (obj.HasMember("count"))
        {
                bool count = bool(obj["count"].GetUint());
                count_ = count;
        }
        return true;
    }
    bool GetCoilsRequest::Serialize(rapidjson::Writer<rapidjson::StringBuffer> *writer) const
    {

        writer->StartObject();
        writer->Key("device");
        writer->String(device_.c_str());
        writer->Key("pin");
        writer->String(pin_.c_str());
        writer->Key("count");
        writer->Uint(count_);
        writer->EndObject();
        return true;
    }
    bool GetCoilsRequest::IsNullJSONData() const
    {
        return false;
    }

    void GetCoilsResponse::set_values(std::vector<bool> values)
    {
      values_ = values;
    }
    std::vector<bool> GetCoilsResponse::values() const
    {
      return values_;
    }
    std::vector<bool> *GetCoilsResponse::mutable_values()
    {
      return &values_;
    }
    bool GetCoilsResponse::Deserialize(const rapidjson::Value &obj)
    {
      if(obj.HasMember("values"))
      {
        std::vector<bool> values;
        for(auto i = obj["values"].GetArray().Begin();i != obj["values"].GetArray().End();i++)
        {
          values.push_back(bool(i->GetBool()));
        }
        values_ = values;
      }
      return true;
    }
    bool GetCoilsResponse::Serialize(rapidjson::Writer<rapidjson::StringBuffer> *writer) const
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
    bool GetCoilsResponse::IsNullJSONData() const
    {
      return values_.size() == 0;
    }

    void SetRegistersRequest::set_device(std::string device)
    {
        device_ = device;
    }
    std::string SetRegistersRequest::device() const
    {
        return device_;
    }
    std::string *SetRegistersRequest::mutable_device()
    {
        return &device_;
    }

    void SetRegistersRequest::set_pin(std::string pin)
    {
        pin_ = pin;
    }
    std::string SetRegistersRequest::pin() const
    {
        return pin_;
    }
    std::string *SetRegistersRequest::mutable_pin()
    {
        return &pin_;
    }

    void SetRegistersRequest::set_values(std::vector<unsigned int> values)
    {
        values = values_;
    }
    std::vector<unsigned int> SetRegistersRequest::values() const
    {
        return values_;
    }
    std::vector<unsigned int> *SetRegistersRequest::mutable_values()
    {
        return &values_;
    }

    bool SetRegistersRequest::Deserialize(const rapidjson::Value &obj)
    {
        if (obj.HasMember("device"))
        {
                std::string device_str = std::string(obj["device"].GetString());
                device_ = device_str;
        }
        if (obj.HasMember("pin"))
        {
                std::string pin_str = std::string(obj["pin"].GetString());
                pin_ = pin_str;
        }
        if (obj.HasMember("values"))
        {
                std::vector<unsigned int> values;
                for (auto i = obj["values"].GetArray().Begin(); i != obj["values"].GetArray().End(); i++)
                {
                unsigned int value_uint = (unsigned int)(i->GetUint());
                values.push_back(value_uint);
                }
                values_ = values;
        }
        return true;
    }
    bool SetRegistersRequest::Serialize(rapidjson::Writer<rapidjson::StringBuffer> *writer) const
    {

        writer->StartObject();
        writer->Key("device");
        writer->String(device_.c_str());
        writer->Key("pin");
        writer->String(pin_.c_str());
        writer->Key("values");
        writer->StartArray();
        for (auto i : values_)
        {
                writer->Bool(i);
        }
        writer->EndArray();
        writer->EndObject();
        return true;
    }
    bool SetRegistersRequest::IsNullJSONData() const
    {
        return values_.size() == 0;
    }
    void SetRegisterRequest::set_device(std::string device)
    {
        device_ = device;
    }
    std::string SetRegisterRequest::device() const
    {
        return device_;
    }
    std::string *SetRegisterRequest::mutable_device()
    {
        return &device_;
    }

    void SetRegisterRequest::set_pin(std::string pin)
    {
        pin_ = pin;
    }
    std::string SetRegisterRequest::pin() const
    {
        return pin_;
    }
    std::string *SetRegisterRequest::mutable_pin()
    {
        return &pin_;
    }

    void SetRegisterRequest::set_value(unsigned int values)
    {
        values = value_;
    }
    unsigned int SetRegisterRequest::value() const
    {
        return value_;
    }
    unsigned int *SetRegisterRequest::mutable_value()
    {
        return &value_;
    }

    bool SetRegisterRequest::Deserialize(const rapidjson::Value &obj)
    {
        if (obj.HasMember("device"))
        {
                std::string device_str = std::string(obj["device"].GetString());
                device_ = device_str;
        }
        if (obj.HasMember("pin"))
        {
                std::string pin_str = std::string(obj["pin"].GetString());
                pin_ = pin_str;
        }
        if (obj.HasMember("values"))
        {
                unsigned int values = (unsigned int)(obj["values"].GetUint());
                value_ = values;
        }
        return true;
    }
    bool SetRegisterRequest::Serialize(rapidjson::Writer<rapidjson::StringBuffer> *writer) const
    {

        writer->StartObject();
        writer->Key("device");
        writer->String(device_.c_str());
        writer->Key("pin");
        writer->String(pin_.c_str());
        writer->Key("value");
        writer->Bool(value_);
        writer->EndObject();
        return true;
    }
    bool SetRegisterRequest::IsNullJSONData() const
    {
        return false;
    }

    void GetRegistersRequest::set_device(std::string device)
    {
        device_ = device;
    }
    std::string GetRegistersRequest::device() const
    {
        return device_;
    }
    std::string *GetRegistersRequest::mutable_device()
    {
        return &device_;
    }

    void GetRegistersRequest::set_pin(std::string pin)
    {
        pin_ = pin;
    }
    std::string GetRegistersRequest::pin() const
    {
        return pin_;
    }
    std::string *GetRegistersRequest::mutable_pin()
    {
        return &pin_;
    }

    void GetRegistersRequest::set_count(unsigned int count)
    {
        count = count_;
    }
    unsigned int GetRegistersRequest::count() const
    {
        return count_;
    }
    unsigned int *GetRegistersRequest::mutable_count()
    {
        return &count_;
    }

    bool GetRegistersRequest::Deserialize(const rapidjson::Value &obj)
    {
        if (obj.HasMember("device"))
        {
                std::string device_str = std::string(obj["device"].GetString());
                device_ = device_str;
        }
        if (obj.HasMember("pin"))
        {
                std::string pin_str = std::string(obj["pin"].GetString());
                pin_ = pin_str;
        }
        if (obj.HasMember("count"))
        {
                bool count = bool(obj["count"].GetUint());
                count_ = count;
        }
        return true;
    }
    bool GetRegistersRequest::Serialize(rapidjson::Writer<rapidjson::StringBuffer> *writer) const
    {

        writer->StartObject();
        writer->Key("device");
        writer->String(device_.c_str());
        writer->Key("pin");
        writer->String(pin_.c_str());
        writer->Key("count");
        writer->Uint(count_);
        writer->EndObject();
        return true;
    }
    bool GetRegistersRequest::IsNullJSONData() const
    {
        return false;
    }

    void GetRegistersResponse::set_values(std::vector<unsigned int> values)
    {
      values_ = values;
    }
    std::vector<unsigned int> GetRegistersResponse::values() const
    {
      return values_;
    }
    std::vector<unsigned int> *GetRegistersResponse::mutable_values()
    {
      return &values_;
    }
    bool GetRegistersResponse::Deserialize(const rapidjson::Value &obj)
    {
      if(obj.HasMember("values"))
      {
        std::vector<unsigned int> values;
        for(auto i = obj["values"].GetArray().Begin();i != obj["values"].GetArray().End();i++)
        {
          values.push_back((unsigned int)(i->GetUint()));
        }
        values_ = values;
      }
      return true;
    }
    bool GetRegistersResponse::Serialize(rapidjson::Writer<rapidjson::StringBuffer> *writer) const
    {
      writer->StartObject();
      writer->Key("values");
      writer->StartArray();
      for(auto i:values_)
      {
        writer->Uint(i);
      }
      writer->EndArray();
      writer->EndObject();
      return true;
    }
    bool GetRegistersResponse::IsNullJSONData() const
    {
      return values_.size() == 0;
    }
  }
}