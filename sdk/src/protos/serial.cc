#include <string>
#include "serial.hh"

namespace lebai
{
    namespace serial
    {
        void SetSerialBaudRateRequest::set_device(std::string device)
        {
            device_ = device;
        }
        std::string SetSerialBaudRateRequest::device() const
        {
            return device_;
        }
        std::string *SetSerialBaudRateRequest::mutable_device()
        {
            return &device_;
        }

        void SetSerialBaudRateRequest::set_baud_rate(unsigned int baud_rate)
        {
            baud_rate_ = baud_rate;
        }
        unsigned int SetSerialBaudRateRequest::baud_rate() const
        {
            return baud_rate_;
        }
        unsigned int *SetSerialBaudRateRequest::mutable_baud_rate()
        {
            return &baud_rate_;
        }

        bool SetSerialBaudRateRequest::Deserialize(const rapidjson::Value &obj)
        {
            if (obj.HasMember("device"))
            {
                std::string device_str = std::string(obj["device"].GetString());
                device_ = device_str;
            }
            if (obj.HasMember("baud_rate"))
            {
                baud_rate_ = obj["baud_rate"].GetUint();
            }
            return true;
        }
        bool SetSerialBaudRateRequest::Serialize(rapidjson::Writer<rapidjson::StringBuffer> *writer) const
        {
            writer->StartObject();
            writer->Key("device");
            writer->String(device_.c_str());
            writer->Key("baud_rate");
            writer->Uint(baud_rate_);
            writer->EndObject();
            return true;
        }
        bool SetSerialBaudRateRequest::IsNullJSONData() const
        {
            return false;
        }

        void SetSerialParityRequest::set_device(std::string device)
        {
            device_ = device;
        }
        std::string SetSerialParityRequest::device() const
        {
            return device_;
        }
        std::string *SetSerialParityRequest::mutable_device()
        {
            return &device_;
        }

        void SetSerialParityRequest::set_parity(Parity parity)
        {
            parity_ = parity;
        }
        Parity SetSerialParityRequest::parity() const
        {
            return parity_;
        }
        Parity *SetSerialParityRequest::mutable_parity()
        {
            return &parity_;
        }

        bool SetSerialParityRequest::Deserialize(const rapidjson::Value &obj)
        {
            if (obj.HasMember("device"))
            {
                std::string device_str = std::string(obj["device"].GetString());
                device_ = device_str;
            }
            if (obj.HasMember("parity"))
            {
                parity_ = static_cast<Parity>(obj["parity"].GetUint());
            }
            return true;
        }
        bool SetSerialParityRequest::Serialize(rapidjson::Writer<rapidjson::StringBuffer> *writer) const
        {
            writer->StartObject();
            writer->Key("device");
            writer->String(device_.c_str());
            writer->Key("parity");
            writer->Uint(static_cast<unsigned int>(parity_));
            writer->EndObject();
            return true;
        }
        bool SetSerialParityRequest::IsNullJSONData() const
        {
            return false;
        }
    }
}