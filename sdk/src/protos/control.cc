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

        void StartTaskRequest::set_is_main(bool is_main)
        {
            is_main_ = is_main;
        }
        bool StartTaskRequest::is_main()
        {
            return is_main_;
        }
        bool * StartTaskRequest::mutable_is_main()
        {
            return &is_main_;
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
            if(obj.HasMember("is_main"))
            {
                bool is_main_bool = bool(obj["is_main"].GetBool());
                is_main_ = is_main_bool;
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
            writer->Key("is_main");
            writer->Bool(is_main_);
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
    }
}