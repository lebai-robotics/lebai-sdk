#pragma once

#include "jsonbase.hh"
#include <string>
#include <vector>

namespace lebai
{
    namespace db
    {
        class DeleteRequest : public JSONBase
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

        class Dir : public JSONBase
        {
        public:
            void set_name(std::string name);
            std::string name();
            std::string * mutable_name();

            void set_id(unsigned int id);
            unsigned int id();
            unsigned int * mutable_id();

        protected:
            std::string name_;
            unsigned int id_;

        public:
            virtual bool Deserialize(const rapidjson::Value &obj);
			virtual bool Serialize(rapidjson::Writer<rapidjson::StringBuffer> *writer) const;
			virtual bool IsNullJSONData() const;
        };

        class Dirs : public JSONBase
        {
        public:
            void set_dirs(std::vector<Dir> dirs);
            std::vector<Dir> dirs();
            std::vector<Dir> * mutable_dirs();

        protected:
            std::vector<Dir> dirs_;

        public:
            virtual bool Deserialize(const rapidjson::Value &obj);
			virtual bool Serialize(rapidjson::Writer<rapidjson::StringBuffer> *writer) const;
			virtual bool IsNullJSONData() const;
        };

        class LoadListRequest : public JSONBase
        {
        public:
            void set_dir(std::string dir);
            std::string dir();
            std::string * mutable_dir();

        protected:
            std::string dir_;

        public:
            virtual bool Deserialize(const rapidjson::Value &obj);
			virtual bool Serialize(rapidjson::Writer<rapidjson::StringBuffer> *writer) const;
			virtual bool IsNullJSONData() const;
        };

        class LoadListResponse : public JSONBase
        {
        public:
            void set_data(std::vector<std::string> data);
            std::vector<std::string> data();
            std::vector<std::string> * mutable_data();

        protected:
            std::vector<std::string> data_;

        public:
            virtual bool Deserialize(const rapidjson::Value &obj);
			virtual bool Serialize(rapidjson::Writer<rapidjson::StringBuffer> *writer) const;
			virtual bool IsNullJSONData() const;
        };

        class LoadRequest : public JSONBase
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

        class LoadResponse : public JSONBase
        {
        public:
            void set_data(std::string data);
            std::string data();
            std::string * mutable_data();

        protected:
            std::string data_;

        public:
            virtual bool Deserialize(const rapidjson::Value &obj);
			virtual bool Serialize(rapidjson::Writer<rapidjson::StringBuffer> *writer) const;
			virtual bool IsNullJSONData() const;
        };

        class SaveRequest : public JSONBase
        {
        public:
            void set_name(std::string name);
            std::string name();
            std::string * mutable_name();

            void set_data(std::string data);
            std::string data();
            std::string * mutable_data();

            void set_dir(std::string dir);
            std::string dir();
            std::string * mutable_dir();

        protected:
            std::string name_;
            std::string data_;
            std::string dir_;

        public:
            virtual bool Deserialize(const rapidjson::Value &obj);
			virtual bool Serialize(rapidjson::Writer<rapidjson::StringBuffer> *writer) const;
			virtual bool IsNullJSONData() const;
        };

        class UpdateDirRequest : public JSONBase
        {
        public:
            void set_from(std::string from);
            std::string from();
            std::string * mutable_from();

            void set_to(std::string to);
            std::string to();
            std::string * mutable_to();

        protected:
            std::string from_;
            std::string to_;

        public:
            virtual bool Deserialize(const rapidjson::Value &obj);
			virtual bool Serialize(rapidjson::Writer<rapidjson::StringBuffer> *writer) const;
			virtual bool IsNullJSONData() const;
        };
    }
}