#pragma once

#include "dynamic.hh"
#include <vector>
#include <string>

namespace lebai
{
    namespace dynamic
    {
        void SetPayloadRequest::set_mass(double mass)
        {
            mass_ = mass;
        }
        double SetPayloadRequest::mass() const
        {
            return mass_;
        }
        double *SetPayloadRequest::mutable_mass()
        {
            return &mass_;
        }

        void SetPayloadRequest::set_cog(std::vector<double> cog)
        {
            cog_ = cog;
        }
        std::vector<double> SetPayloadRequest::cog() const
        {
            return cog_;
        }
        std::vector<double> *SetPayloadRequest::mutable_cog()
        {
            return &cog_;
        }

        bool SetPayloadRequest::Deserialize(const rapidjson::Value &obj)
        {
            if(obj.HasMember("mass"))
            {
                double mass = (double)(obj["mass"].GetDouble());
                mass_ = mass;
            }
            if(obj.HasMember("cog"))
            {
                std::vector<double> cog;
                for(auto c = obj["cog"].GetArray().Begin();c != obj["cog"].GetArray().End();c++)
                {
                    cog.push_back(c->GetDouble());
                }
                cog_ = cog;
            }
            return true;
        }
        bool SetPayloadRequest::Serialize(rapidjson::Writer<rapidjson::StringBuffer> *writer) const
        {
            writer->StartObject();
            writer->Key("mass");
            writer->Double(mass_);
            writer->Key("cog");
            writer->StartArray();
            for(auto c:cog_)
            {
                writer->Double(c);
            }
            writer->EndArray();
            writer->EndObject();
            return true;
        }
        bool SetPayloadRequest::IsNullJSONData() const
        {
            return false;
        }
        void Payload::set_mass(double mass)
        {
            mass_ = mass;
        }
        double Payload::mass() const
        {
            return mass_;
        }
        double *Payload::mutable_mass()
        {
            return &mass_;
        }

        void Payload::set_cog(std::vector<double> cog)
        {
            cog_ = cog;
        }
        std::vector<double> Payload::cog() const
        {
            return cog_;
        }
        std::vector<double> *Payload::mutable_cog()
        {
            return &cog_;
        }

        bool Payload::Deserialize(const rapidjson::Value &obj)
        {
            if(obj.HasMember("mass"))
            {
                double mass = (double)(obj["mass"].GetDouble());
                mass_ = mass;
            }
            if(obj.HasMember("cog"))
            {
                std::vector<double> cog;
                for(auto c = obj["cog"].GetArray().Begin();c != obj["cog"].GetArray().End();c++)
                {
                    cog.push_back(c->GetDouble());
                }
                cog_ = cog;
            }
            return true;
        }
        bool Payload::Serialize(rapidjson::Writer<rapidjson::StringBuffer> *writer) const
        {
            writer->StartObject();
            writer->Key("mass");
            writer->Double(mass_);
            writer->Key("cog");
            writer->StartArray();
            for(auto c:cog_)
            {
                writer->Double(c);
            }
            writer->EndArray();
            writer->EndObject();
            return true;
        }
        bool Payload::IsNullJSONData() const
        {
            return false;
        }

        void SavePayloadRequest::set_name(std::string name)
        {
            name_ = name;
        }
        std::string SavePayloadRequest::name() const
        {
            return name_;
        }
        std::string *SavePayloadRequest::mutable_name()
        {
            return &name_;
        }

        void SavePayloadRequest::set_data(Payload data)
        {
            data_ = data;
        }
        Payload SavePayloadRequest::data() const
        {
            return data_;
        }
        Payload *SavePayloadRequest::mutable_data()
        {
            return &data_;
        }

        void SavePayloadRequest::set_dir(std::string dir)
        {
            dir_ = dir;
        }
        std::string SavePayloadRequest::dir() const
        {
            return dir_;
        }
        std::string *SavePayloadRequest::mutable_dir()
        {
            return &dir_;
        }
        bool SavePayloadRequest::Deserialize(const rapidjson::Value &obj)
        {
            if(obj.HasMember("name"))
            {
                std::string name = (std::string)(obj["name"].GetString());
                name_ = name;
            }
            if(obj.HasMember("data"))
            {
                Payload data;
                data.FromJSONString(obj["data"].GetString());
                data_ = data;
            }
            if(obj.HasMember("dir"))
            {
                std::string dir = (std::string)(obj["dir"].GetString());
                dir_ = dir;
            }
            return true;
        }
        bool SavePayloadRequest::Serialize(rapidjson::Writer<rapidjson::StringBuffer> *writer) const
        {
            writer->StartObject();
            writer->Key("name");
            writer->String(name_.c_str());
            writer->Key("data");
            writer->String(data_.ToJSONString().c_str());
            writer->Key("dir");
            writer->String(dir_.c_str());
            writer->EndObject();
            return true;
        }
        bool SavePayloadRequest::IsNullJSONData() const
        {
            return false;
        }
    }
}