
#pragma once

#include "jsonbase.hh"
#include <memory>
#include <string>
#include <vector>

namespace lebai
{
    namespace control
    {
        enum TaskKind
        {
            LUA = 0,
            APP = 10,
        };
        class StartTaskRequest : public JSONBase
        {
        public:
            void set_name(std::string name);
            std::string name();
            std::string * mutable_name();

            void set_is_main(bool is_main);
            bool is_main();
            bool * mutable_is_main();

            void set_loop_to(unsigned int loop_to);
            unsigned int loop_to();
            unsigned int * mutable_loop_to();

            void set_dir(std::string name);
            std::string dir();
            std::string * mutable_dir();

            void set_params(std::vector<std::string> name);
            std::vector<std::string> params();
            std::vector<std::string> * mutable_params();


        protected:
            std::string name_;
            bool is_main_;
            unsigned int loop_to_;
            std::string dir_;
            TaskKind kind_ = TaskKind::LUA;
            std::vector<std::string> params_;

        public:
            virtual bool Deserialize(const rapidjson::Value &obj);
			virtual bool Serialize(rapidjson::Writer<rapidjson::StringBuffer> *writer) const;
			virtual bool IsNullJSONData() const;
        };

        class TaskIds : public JSONBase
        {
        public:
            void set_ids(std::vector<unsigned int> ids);
            std::vector<unsigned int> ids();
            std::vector<unsigned int> * mutable_ids();

        protected:
            std::vector<unsigned int> ids_;

        public:
            virtual bool Deserialize(const rapidjson::Value &obj);
			virtual bool Serialize(rapidjson::Writer<rapidjson::StringBuffer> *writer) const;
			virtual bool IsNullJSONData() const;
        };

        class TaskIndex : public JSONBase
        {
        public:
            void set_id(unsigned int);
            unsigned int id();
            unsigned int * mutable_id();

        protected:
            unsigned int id_;

        public:
            virtual bool Deserialize(const rapidjson::Value &obj);
			virtual bool Serialize(rapidjson::Writer<rapidjson::StringBuffer> *writer) const;
			virtual bool IsNullJSONData() const;
        };
    }
}