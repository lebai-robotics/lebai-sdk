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
  }
}