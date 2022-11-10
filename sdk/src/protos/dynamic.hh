#pragma once

#include "jsonbase.hh"
#include <vector>
#include <memory>
#include <string>

namespace lebai
{
    namespace dynamic
    {
        class SetPayloadRequest : public JSONBase
        {
        public:
            void set_mass(double mass);
            double mass() const;
            double *mutable_mass();

            void set_cog(std::vector<double> cog);
            std::vector<double>  cog() const;
            std::vector<double> * mutable_cog();

        protected:
            double mass_;
            std::vector<double> cog_;
            // These methods are used to serialize and deserialize the class.
            // They will not be wrapped in the SDK.
        public:
            virtual bool Deserialize(const rapidjson::Value &obj);
            virtual bool Serialize(rapidjson::Writer<rapidjson::StringBuffer> *writer) const;
            virtual bool IsNullJSONData() const;
        };

        class Payload : public JSONBase
        {
        public:
            void set_mass(double mass);
            double mass() const;
            double *mutable_mass();

            void set_cog(std::vector<double> cog);
            std::vector<double>  cog() const;
            std::vector<double> * mutable_cog();

        protected:
            double mass_;
            std::vector<double> cog_;
            // These methods are used to serialize and deserialize the class.
            // They will not be wrapped in the SDK.
        public:
            virtual bool Deserialize(const rapidjson::Value &obj);
            virtual bool Serialize(rapidjson::Writer<rapidjson::StringBuffer> *writer) const;
            virtual bool IsNullJSONData() const;
        };

        class SavePayloadRequest : public JSONBase
        {
        public:
            void set_name(std::string name);
            std::string name() const;
            std::string *mutable_name();

            void set_data(Payload data);
            Payload  data() const;
            Payload * mutable_data();

            void set_dir(std::string dir);
            std::string dir() const;
            std::string *mutable_dir();
        protected:
            std::string name_;
            Payload data_;
            std::string dir_;
            // These methods are used to serialize and deserialize the class.
            // They will not be wrapped in the SDK.
        public:
            virtual bool Deserialize(const rapidjson::Value &obj);
            virtual bool Serialize(rapidjson::Writer<rapidjson::StringBuffer> *writer) const;
            virtual bool IsNullJSONData() const;
        };
    }
}