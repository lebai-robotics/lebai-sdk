#pragma once

#include "jsonbase.hh"
#include <vector>
#include <memory>
#include <string>
#include "posture.hh"

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

            void set_cog(posture::Position cog);
            posture::Position cog() const;
            posture::Position * mutable_cog();

        protected:
            double mass_;
            posture::Position cog_;
            // These methods are used to serialize and deserialize the class.
            // They will not be wrapped in the SDK.
        public:
            virtual bool Deserialize(const rapidjson::Value &obj);
            virtual bool Serialize(rapidjson::Writer<rapidjson::StringBuffer> *writer) const;
            virtual bool IsNullJSONData() const;
        };

        class SetMassRequest : public JSONBase
        {
        public:
            void set_mass(double mass);
            double mass() const;
            double *mutable_mass();

        protected:
            double mass_;
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

            void set_cog(posture::Position cog);
            posture::Position cog() const;
            posture::Position * mutable_cog();

        protected:
            double mass_;
            posture::Position cog_;
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