#pragma once

#include "jsonbase.hh"
#include "db.hh"
#include <string>
#include <vector>
#include <memory>

namespace lebai
{
    namespace block
    {

        class Block : public JSONBase
        {
        public:
            void set_method(std::string method);
            std::string method();
            std::string * mutable_method();

            void set_param(std::string param);
            std::string param();
            std::string * mutable_param();

        protected:
            std::string method_;
            std::string param_;

        public:
            virtual bool Deserialize(const rapidjson::Value &obj);
			virtual bool Serialize(rapidjson::Writer<rapidjson::StringBuffer> *writer) const;
			virtual bool IsNullJSONData() const;
        };
        
        class RunBlockRequest : public JSONBase
        {
        public:
            void set_name(std::string name);
            std::string name();
            std::string * mutable_name();

            void set_dir(std::string dir);
            std::string dir();
            std::string * mutable_dir();

        protected:
            std::string name_;
            std::string dir_;

        public:
            virtual bool Deserialize(const rapidjson::Value &obj);
			virtual bool Serialize(rapidjson::Writer<rapidjson::StringBuffer> *writer) const;
			virtual bool IsNullJSONData() const;
        };

        class SaveBlockRequest : public JSONBase
        {
        public:
            void set_name(std::string name);
            std::string name();
            std::string * mutable_name();

            void set_data(Block data);
            Block data();
            Block * mutable_data();

            void set_dir(std::string dir);
            std::string dir();
            std::string * mutable_dir();

        protected:
            std::string name_;
            Block data_;
            std::string dir_;

        public:
            virtual bool Deserialize(const rapidjson::Value &obj);
			virtual bool Serialize(rapidjson::Writer<rapidjson::StringBuffer> *writer) const;
			virtual bool IsNullJSONData() const;
        };

    }
}