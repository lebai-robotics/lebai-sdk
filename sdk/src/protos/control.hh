
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

        enum TaskState
        {
            WAIT = 0,
            RUNNING = 1,
            PAUSE = 2,
            SUCCESS = 3,
            INTERRUPT = 4,
            FAIL = 5,
        };

        class StartTaskRequest : public JSONBase
        {
        public:
            void set_name(std::string name);
            std::string name();
            std::string * mutable_name();

            void set_is_parallel(bool is_main);
            bool is_parallel();
            bool * mutable_is_parallel();

            void set_loop_to(unsigned int loop_to);
            unsigned int loop_to();
            unsigned int * mutable_loop_to();

            void set_dir(std::string name);
            std::string dir();
            std::string * mutable_dir();

            void set_params(const std::vector<std::string> & name);
            std::vector<std::string> params();
            std::vector<std::string> * mutable_params();


        protected:
            std::string name_;
            bool is_parallel_;
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
            void set_id(unsigned int id);
            unsigned int id();
            unsigned int * mutable_id();

        protected:
            unsigned int id_;

        public:
            virtual bool Deserialize(const rapidjson::Value &obj);
			virtual bool Serialize(rapidjson::Writer<rapidjson::StringBuffer> *writer) const;
			virtual bool IsNullJSONData() const;
        };

        /*
        class Task : public JSONBase
        {
            void set_id(unsigned int id);
            unsigned int id();
            unsigned int * mutable_id();

            void set_block_id(std::string block_id);
            std::string block_id();
            std::string * mutable_block_id();

            void set_state(TaskState state);
            TaskState state();
            TaskState * mutable_state();

            void set_loopc(unsigned int loopc);
            unsigned int loopc();
            unsigned int * mutable_loopc();

            void set_loopt(unsigned int loopt);
            unsigned int loopt();
            unsigned int * mutable_loopt();

            void set_is_main(bool is_main);
            bool is_main();
            bool * mutable_is_main();

            void set_is_simu(bool is_simu);
            bool is_simu();
            bool * mutable_is_simu();

            void set_out(std::string out);
            std::string out();
            std::string * mutable_out();

        protected:
            unsigned int id_;
            std::string block_id_;
            TaskState state_;
            unsigned int loopc_;
            unsigned int loopt_;
            bool is_main_;
            bool is_simu_;
            std::string out_;

        public:
            virtual bool Deserialize(const rapidjson::Value &obj);
			virtual bool Serialize(rapidjson::Writer<rapidjson::StringBuffer> *writer) const;
			virtual bool IsNullJSONData() const;
        };
        */

        class PauseRequest : public JSONBase
        {
        public:
            void set_id(unsigned int id);
            unsigned int id();
            unsigned int * mutable_id();

            void set_time(unsigned long time);
            unsigned long time();
            unsigned long * mutable_time();

            void set_wait(bool wait);
            bool wait();
            bool * mutable_wait();

        protected:
            unsigned int id_;
            unsigned long time_;
            bool wait_;
        public:
            virtual bool Deserialize(const rapidjson::Value &obj);
			virtual bool Serialize(rapidjson::Writer<rapidjson::StringBuffer> *writer) const;
			virtual bool IsNullJSONData() const;
        };
    }
}