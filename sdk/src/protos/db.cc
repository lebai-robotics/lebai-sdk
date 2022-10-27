#include "db.hh"

namespace lebai
{
    namespace db
    {
        void DeleteRequest::set_name(std::string name)
        {
            name_ = name;
        }
        std::string DeleteRequest::name()
        {
            return name_;
        }
        std::string *DeleteRequest::mutable_name()
        {
            return &name_;
        }

        void DeleteRequest::set_dir(std::string dir)
        {
            dir_ = dir;
        }
        std::string DeleteRequest::dir()
        {
            return dir_;
        }
        std::string *DeleteRequest::mutable_dir()
        {
            return &dir_;
        }
        bool DeleteRequest::Deserialize(const rapidjson::Value &obj)
        {
            if(obj.HasMember("name"))
            {
                std::string name_str = (std::string)(obj["name"].GetString());
                name_ = name_str;
            }
            if(obj.HasMember("dir"))
            {
                std::string dir_str = (std::string)(obj["dir"].GetString());
                dir_ = dir_str;
            }
            return true;
        }
		bool DeleteRequest::Serialize(rapidjson::Writer<rapidjson::StringBuffer> *writer) const
        {
            writer->StartObject();
            writer->Key("name");
            writer->String(name_.c_str());
            writer->Key("dir");
            writer->String(dir_.c_str());
            writer->EndObject();
            return true;
        }
		bool DeleteRequest::IsNullJSONData() const
        {
            return false;
        }

        void Dir::set_name(std::string name)
        {
            name_ = name;
        }
        std::string Dir::name()
        {
            return name_;
        }
        std::string *Dir::mutable_name()
        {
            return &name_;
        }

        void Dir::set_id(unsigned int id)
        {
            id_ = id;
        }
        unsigned int Dir::id()
        {
            return id_;
        }
        unsigned int *Dir::mutable_id()
        {
            return &id_;
        }

        bool Dir::Deserialize(const rapidjson::Value &obj)
        {
            if(obj.HasMember("name"))
            {
                std::string name_str = (std::string)(obj["name"].GetString());
                name_ = name_str;
            }
            if(obj.HasMember("id"))
            {
                unsigned int id_int = (unsigned int)(obj["id"].GetUint());
                id_ = id_int;
            }
            return true;
        }
		bool Dir::Serialize(rapidjson::Writer<rapidjson::StringBuffer> *writer) const
        {
            writer->StartObject();
            writer->Key("name");
            writer->String(name_.c_str());
            writer->Key("id");
            writer->Uint(id_);
            writer->EndObject();
            return true;
        }
		bool Dir::IsNullJSONData() const
        {
            return false;
        }
        void Dirs::set_dirs(std::vector<Dir> dirs)
        {
            dirs_ = dirs;
        }
        std::vector<Dir> Dirs::dirs()
        {
            return dirs_;
        }
        std::vector<Dir> *Dirs::mutable_dirs()
        {
            return &dirs_;
        }

        bool Dirs::Deserialize(const rapidjson::Value &obj)
        {
           if(obj.HasMember("dir"))
            {
                std::vector<Dir> dirs;
                for(auto d = obj["dir"].Begin();d != obj["dir"].End();d++)
                {
                    Dir dir;
                    dir.FromJSONString(d->GetString());
                    dirs.push_back(dir);
                }
                dirs_ = dirs;
            }
            return true;
        }
		bool Dirs::Serialize(rapidjson::Writer<rapidjson::StringBuffer> *writer) const
        {
            writer->StartObject();
            writer->Key("dir");
            writer->StartArray();
            for(auto d:dirs_)
            {
                writer->String(d.ToJSONString().c_str());
            }
            writer->EndArray();
            writer->EndObject();
            return true;
        }
		bool Dirs::IsNullJSONData() const
        {
            return false;
        }

        void LoadListRequest::set_dir(std::string dir)
        {
            dir_ = dir;
        }
        std::string LoadListRequest::dir()
        {
            return dir_;
        }
        std::string *LoadListRequest::mutable_dir()
        {
            return &dir_;
        }
        bool LoadListRequest::Deserialize(const rapidjson::Value &obj)
        {
            if(obj.HasMember("dir"))
            {
                std::string dir_str = (std::string)(obj["dir"].GetString());
                dir_ = dir_str;
            }
            return true;
        }
		bool LoadListRequest::Serialize(rapidjson::Writer<rapidjson::StringBuffer> *writer) const
        {
            writer->StartObject();
            writer->Key("dir");
            writer->String(dir_.c_str());
            writer->EndObject();
            return true;
        }
		bool LoadListRequest::IsNullJSONData() const
        {
            return false;
        }
        
        void LoadListResponse::set_data(std::vector<std::string> data)
        {
            data_ = data;
        }
        std::vector<std::string> LoadListResponse::data()
        {
            return data_;
        }
        std::vector<std::string> *LoadListResponse::mutable_data()
        {
            return &data_;
        }

        bool LoadListResponse::Deserialize(const rapidjson::Value &obj)
        {
            if(obj.HasMember("data"))
            {
                std::vector<std::string> data;
                for(auto d = obj["data"].Begin();d != obj["data"].End();d++)
                {
                    std::string data_str = (std::string)(d->GetString());
                    data.push_back(data_str);
                }
                data_ = data;
            }
            return true;
        }
		bool LoadListResponse::Serialize(rapidjson::Writer<rapidjson::StringBuffer> *writer) const
        {
            writer->StartObject();
            writer->Key("data");
            writer->StartArray();
            for(auto d:data_){
                writer->String(d.c_str());
            }
            writer->EndArray();
            writer->EndObject();
            return true;
        }
		bool LoadListResponse::IsNullJSONData() const
        {
            return false;
        }

        void LoadRequest::set_name(std::string name)
        {
            name_ = name;
        }
        std::string LoadRequest::name()
        {
            return name_;
        }
        std::string *LoadRequest::mutable_name()
        {
            return &name_;
        }

        void LoadRequest::set_dir(std::string dir)
        {
            dir_ = dir;
        }
        std::string LoadRequest::dir()
        {
            return dir_;
        }
        std::string *LoadRequest::mutable_dir()
        {
            return &dir_;
        }

        bool LoadRequest::Deserialize(const rapidjson::Value &obj)
        {
            if(obj.HasMember("name"))
            {
                std::string name_str = (std::string)(obj["name"].GetString());
                name_ = name_str;
            }
            if(obj.HasMember("dir"))
            {
                std::string dir_str = (std::string)(obj["dir"].GetString());
                dir_ = dir_str;
            }
            return true;
        }
		bool LoadRequest::Serialize(rapidjson::Writer<rapidjson::StringBuffer> *writer) const
        {
            writer->StartObject();
            writer->Key("name");
            writer->String(name_.c_str());
            writer->Key("dir");
            writer->String(dir_.c_str());
            writer->EndObject();
            return true;
        }
		bool LoadRequest::IsNullJSONData() const
        {
            return false;
        }
        
        void LoadResponse::set_data(std::string data)
        {
            data_ = data;
        }
        std::string LoadResponse::data()
        {
            return data_;
        }
        std::string * LoadResponse::mutable_data()
        {
            return &data_;
        }

        bool LoadResponse::Deserialize(const rapidjson::Value &obj)
        {
            if(obj.HasMember("data"))
            {
                std::string data_str = (std::string)(obj["data"].GetString());
                data_ = data_str;
            }
            return true;
        }
		bool LoadResponse::Serialize(rapidjson::Writer<rapidjson::StringBuffer> *writer) const
        {
            writer->StartObject();
            writer->Key("data");
            writer->String(data_.c_str());
            writer->EndObject();
            return true;
        }
		bool LoadResponse::IsNullJSONData() const
        {
            return false;
        }

        void SaveRequest::set_name(std::string name)
        {
            name_ = name;
        }
        std::string SaveRequest::name()
        {
            return name_;
        }
        std::string *SaveRequest::mutable_name()
        {
            return &name_;
        }

        void SaveRequest::set_data(std::string data)
        {
            data_ = data;
        }
        std::string SaveRequest::data()
        {
            return data_;
        }
        std::string *SaveRequest::mutable_data()
        {
            return &data_;
        }

        void SaveRequest::set_dir(std::string dir)
        {
            dir_ = dir;
        }
        std::string SaveRequest::dir()
        {
            return dir_;
        }
        std::string *SaveRequest::mutable_dir()
        {
            return &dir_;
        }
        
        bool SaveRequest::Deserialize(const rapidjson::Value &obj)
        {
            if(obj.HasMember("name"))
            {
                std::string name_str = (std::string)(obj["name"].GetString());
                name_ = name_str;
            }
            if(obj.HasMember("data"))
            {
                std::string data_str = (std::string)(obj["data"].GetString());
                data_ = data_str;
            }
            if(obj.HasMember("dir"))
            {
                std::string dir_str = (std::string)(obj["dir"].GetString());
                dir_ = dir_str;
            }
            return true;
        }
		bool SaveRequest::Serialize(rapidjson::Writer<rapidjson::StringBuffer> *writer) const
        {
            writer->StartObject();
            writer->Key("name");
            writer->String(name_.c_str());
            writer->Key("data");
            writer->String(data_.c_str());
            writer->Key("dir");
            writer->String(dir_.c_str());
            writer->EndObject();
            return true;
        }
		bool SaveRequest::IsNullJSONData() const
        {
            return false;
        }

        void UpdateDirRequest::set_from(std::string from)
        {
            from_ = from;
        }
        std::string UpdateDirRequest::from()
        {
            return from_;
        }
        std::string *UpdateDirRequest::mutable_from()
        {
            return &from_;
        }

        void UpdateDirRequest::set_to(std::string to)
        {
            to_ = to;
        }
        std::string UpdateDirRequest::to()
        {
            return to_;
        }
        std::string *UpdateDirRequest::mutable_to()
        {
            return &to_;
        }

        bool UpdateDirRequest::Deserialize(const rapidjson::Value &obj)
        {
            if(obj.HasMember("from"))
            {
                std::string from_str = (std::string)(obj["from"].GetString());
                from_ = from_str;
            }
            if(obj.HasMember("to"))
            {
                std::string to_str = (std::string)(obj["to"].GetString());
                to_ = to_str;
            }
            return true;
        }
		bool UpdateDirRequest::Serialize(rapidjson::Writer<rapidjson::StringBuffer> *writer) const
        {
            writer->StartObject();
            writer->Key("from");
            writer->String(from_.c_str());
            writer->Key("to");
            writer->String(to_.c_str());
            writer->EndObject();
            return true;
        }
		bool UpdateDirRequest::IsNullJSONData() const
        {
            return false;
        }
    }
}