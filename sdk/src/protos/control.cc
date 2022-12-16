#include <iostream>
#include "control.hh"

namespace lebai
{
    namespace control
    {
        void StartTaskRequest::set_name(std::string name)
        {
            name_ = name;
        }
        std::string StartTaskRequest::name()
        {
            return name_;
        }
        std::string * StartTaskRequest::mutable_name()
        {
            return &name_;
        }

        void StartTaskRequest::set_is_parallel(bool is_parallel)
        {
            is_parallel_ = is_parallel;
        }
        bool StartTaskRequest::is_parallel()
        {
            return is_parallel_;
        }
        bool * StartTaskRequest::mutable_is_parallel()
        {
            return &is_parallel_;
        }

        void StartTaskRequest::set_loop_to(unsigned int loop_to)
        {
            loop_to_ = loop_to;
        }
        unsigned int StartTaskRequest::loop_to()
        {
            return loop_to_;
        }
        unsigned int * StartTaskRequest::mutable_loop_to()
        {
            return &loop_to_;
        }

        void StartTaskRequest::set_dir(std::string dir)
        {
            dir_ = dir;
        }
        std::string StartTaskRequest::dir()
        {
            return dir_;
        }
        std::string * StartTaskRequest::mutable_dir()
        {
            return &dir_;
        }
        
        void StartTaskRequest::set_params(const std::vector<std::string> & params)
        {
            params_ = params;
        }
        std::vector<std::string> StartTaskRequest::params()
        {
            return params_;
        }
        std::vector<std::string> * StartTaskRequest::mutable_params()
        {
            return &params_;
        }

        bool StartTaskRequest::Deserialize(const rapidjson::Value &obj)
        {
            if(obj.HasMember("name"))
            {
                std::string name_str = std::string(obj["name"].GetString());
                name_ = name_str;
            }
            if(obj.HasMember("is_parallel"))
            {
                bool is_parallel_bool = bool(obj["is_parallel"].GetBool());
                is_parallel_ = is_parallel_bool;
            }
            if(obj.HasMember("loop_to"))
            {
                unsigned int loop_to_int = (unsigned int)(obj["loop_to"].GetUint());
                loop_to_ = loop_to_int;
            }
            if(obj.HasMember("dir"))
            {
                std::string dir_str = std::string(obj["dir"].GetString());
                dir_ = dir_str;
            }
            if(obj.HasMember("params"))
            {
                const rapidjson::Value& params = obj["params"];
                for (rapidjson::SizeType i = 0; i < params.Size(); i++)
                {
                    std::string param_str = std::string(obj[i].GetString());
                    params_.push_back(param_str);
                }
            }
            return true;
        }
		bool StartTaskRequest::Serialize(rapidjson::Writer<rapidjson::StringBuffer> *writer) const
        {
            writer->StartObject();
            writer->Key("name");
            writer->String(name_.c_str());
            writer->Key("is_parallel");
            writer->Bool(is_parallel_);
            writer->Key("loop_to");
            writer->Uint(loop_to_);
            writer->Key("dir");
            writer->String(dir_.c_str());
            writer->Key("kind");
            writer->Int(kind_);
            writer->Key("params");
            writer->StartArray();
            for (auto& param : params_)
            {
                writer->String(param.c_str());
            }
            writer->EndArray();
            writer->EndObject();
            return true;
        }
		bool StartTaskRequest::IsNullJSONData() const
        {
            return false;
        }

        void TaskIds::set_ids(std::vector<unsigned int> ids)
        {
            ids_ = ids;
        }
        std::vector<unsigned int> TaskIds::ids()
        {
            return ids_;
        }
        std::vector<unsigned int> * TaskIds::mutable_ids()
        {
            return &ids_;
        }
        bool TaskIds::Deserialize(const rapidjson::Value &obj)
        {
            if(obj.HasMember("ids"))
            {
                const rapidjson::Value& ids = obj["ids"];
                for (rapidjson::SizeType i = 0; i < ids.Size(); i++)
                {
                    unsigned int id_int = (unsigned int)(obj[i].GetUint());
                    ids_.push_back(id_int);
                }
            }
            return true;
        }
		bool TaskIds::Serialize(rapidjson::Writer<rapidjson::StringBuffer> *writer) const
        {
            writer->StartObject();
            writer->Key("ids");
            writer->StartArray();
            for (auto& id : ids_)
            {
                writer->Uint(id);
            }
            writer->EndArray();
            writer->EndObject();
            return true;
        }
		bool TaskIds::IsNullJSONData() const
        {
            return false;
        }

        void TaskIndex::set_id(unsigned int id)
        {
            id_ = id;
        }
        unsigned TaskIndex::id()
        {
            return id_;
        }
        unsigned int * TaskIndex::mutable_id()
        {
            return &id_;
        }
        bool TaskIndex::Deserialize(const rapidjson::Value &obj)
        {
            if(obj.HasMember("id"))
            {
                unsigned int id_int = (unsigned int)(obj["id"].GetUint());
                id_ = id_int; 
            }
            return true;
        }
		bool TaskIndex::Serialize(rapidjson::Writer<rapidjson::StringBuffer> *writer) const
        {
            writer->StartObject();
            writer->Key("ids");
            writer->Uint(id_);
            writer->EndObject();
            return true;
        }
		bool TaskIndex::IsNullJSONData() const
        {
            return false;
        }

        void PauseRequest::set_id(unsigned int id)
        {
            id_ = id;
        }
        unsigned int PauseRequest::id()
        {
            return id_;
        }
        unsigned int * PauseRequest::mutable_id()
        {
            return &id_;
        }

        void PauseRequest::set_time(unsigned long time)
        {
            time_ = time;
        }
        unsigned long PauseRequest::time()
        {
            return time_;
        }
        unsigned long * PauseRequest::mutable_time()
        {
            return &time_;
        }

        void PauseRequest::set_wait(bool wait)
        {
            wait_ = wait;
        }
        bool PauseRequest::wait()
        {
            return wait_;
        }
        bool * PauseRequest::mutable_wait()
        {
            return &wait_;
        }

        bool PauseRequest::Deserialize(const rapidjson::Value &obj)
        {
            if(obj.HasMember("id"))
            {
                unsigned int id_int = (unsigned int)(obj["id"].GetUint());
                id_ = id_int; 
            }
            if(obj.HasMember("time"))
            {
                unsigned long time_u64 = (unsigned long)(obj["time"].GetUint64());
                time_ = time_u64; 
            }
            if(obj.HasMember("wait"))
            {
                bool wait_bool = (bool)(obj["wait"].GetBool());
                wait_ = wait_bool; 
            }
            return true;
        }
		bool PauseRequest::Serialize(rapidjson::Writer<rapidjson::StringBuffer> *writer) const
        {
            writer->StartObject();
            writer->Key("id");
            writer->Uint(id_);
            writer->Key("time");
            writer->Uint64(time_);
            writer->Key("wait");
            writer->Bool(wait_);
            writer->EndObject();
            return true;
        }
		bool PauseRequest::IsNullJSONData() const
        {
            return false;
        }
        void Task::set_id(unsigned int id)
        {
            id_ = id;
        }
        unsigned int Task::id()
        {
            return id_;
        }
        unsigned int *Task::mutable_id()
        {
            return &id_;
        }

        void Task::set_block_id(std::string block_id)
        {
            block_id_ = block_id;
        }
        std::string Task::block_id()
        {
            return block_id_;
        }
        std::string *Task::mutable_block_id()
        {
            return &block_id_;
        }

        void Task::set_state(TaskState state)
        {
            state_ = state;
        }
        TaskState Task::state()
        {
            return state_;
        }
        TaskState *Task::mutable_state()
        {
            return &state_;
        }

        void Task::set_loopc(unsigned int loopc)
        {
            loopc_ = loopc;
        }
        unsigned int Task::loopc()
        {
            return loopc_;
        }
        unsigned int *Task::mutable_loopc()
        {
            return &loopc_;
        }

        void Task::set_loopt(unsigned int loopt)
        {
            loopt_ = loopt;
        }
        unsigned int Task::loopt()
        {
            return loopt_;
        }
        unsigned int *Task::mutable_loopt()
        {
            return &loopt_;
        }

        void Task::set_is_parallel(bool is_parallel)
        {
            is_parallel_ = is_parallel;
        }
        bool Task::is_parallel()
        {
            return is_parallel_;
        }
        bool *Task::mutable_is_parallel()
        {
            return &is_parallel_;
        }

        void Task::set_is_simu(bool is_simu)
        {
            is_simu_ = is_simu;
        }
        bool Task::is_simu()
        {
            return is_simu_;
        }
        bool *Task::mutable_is_simu()
        {
            return &is_simu_;
        }

        void Task::set_out(std::string out)
        {
            out_ = out;
        }
        std::string Task::out()
        {
            return out_;
        }
        std::string *Task::mutable_out()
        {
            return &out_;
        }

        void Task::set_started_at(std::string started_at)
        {
            started_at_ = started_at;
        }
        std::string Task::started_at()
        {
            return started_at_;
        }
        std::string *Task::mutable_started_at()
        {
            return &started_at_;
        }

        void Task::set_ended_at(std::string ended_at)
        {
            ended_at_ = ended_at;
        }
        std::string Task::ended_at()
        {
            return ended_at_;
        }
        std::string *Task::mutable_ended_at()
        {
            return &ended_at_;
        }

        void Task::set_pause_at(std::string pause_at)
        {
            pause_at_ = pause_at;
        }
        std::string Task::pause_at()
        {
            return pause_at_;
        }
        std::string *Task::mutable_pause_at()
        {
            return &pause_at_;
        }

        void Task::set_pre_pause(unsigned int pre_pause)
        {
            pre_pause_ = pre_pause;
        }
        unsigned int Task::pre_pause()
        {
            return pre_pause_;
        }
        unsigned int *Task::mutable_pre_pause()
        {
            return &pre_pause_;
        }

        void Task::set_kind(TaskKind kind)
        {
            kind_ = kind;
        }
        TaskKind Task::kind()
        {
            return kind_;
        }
        TaskKind *Task::mutable_kind()
        {
            return &kind_;
        }

        void Task::set_dir(std::string dir)
        {
            dir_ = dir;
        }
        std::string Task::dir()
        {
            return dir_;
        }
        std::string *Task::mutable_dir()
        {
            return &dir_;
        }

        void Task::set_name(std::string name)
        {
            name_ = name;
        }
        std::string Task::name()
        {
            return name_;
        }
        std::string *Task::mutable_name()
        {
            return &name_;
        }

        void Task::set_params(std::vector<std::string> params)
        {
            params_ = params;
        }
        std::vector<std::string> Task::params()
        {
            return params_;
        }
        std::vector<std::string> *Task::mutable_params()
        {
            return &params_;
        }
    }
}