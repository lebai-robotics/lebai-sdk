#pragma once

#include "jsonbase.hh"
#include <vector>
#include <memory>

namespace lebai
{
    namespace file
    {
        class File : public JSONBase
        {
        public:
            void set_is_dir(bool is_dir);
            bool is_dir();
            bool * mutable_is_dir();

            void set_data(std::string data);
            std::string data();
            std::string * mutable_data();

        protected:
            bool is_dir_;
            std::string data_;
        public:
            virtual bool Deserialize(const rapidjson::Value &obj);
			virtual bool Serialize(rapidjson::Writer<rapidjson::StringBuffer> *writer) const;
			virtual bool IsNullJSONData() const;
        };

        class FileIndex : public JSONBase
        {
        public:
            void set_dir(std::string dir);
            std::string dir();
            std::string * mutable_dir();

            void set_name(std::string name);
            std::string name();
            std::string * mutable_name();

        protected:
            std::string dir_;
            std::string name_;
        public:
            virtual bool Deserialize(const rapidjson::Value &obj);
			virtual bool Serialize(rapidjson::Writer<rapidjson::StringBuffer> *writer) const;
			virtual bool IsNullJSONData() const;
        };

        class FileName : public JSONBase
        {
        public:
            void set_is_dir(bool is_dir);
            bool is_dir();
            bool * mutable_is_dir();

            void set_name(std::string data);
            std::string name();
            std::string * mutable_name();

        protected:
            bool is_dir_;
            std::string name_;
        public:
            virtual bool Deserialize(const rapidjson::Value &obj);
			virtual bool Serialize(rapidjson::Writer<rapidjson::StringBuffer> *writer) const;
			virtual bool IsNullJSONData() const;
        };


        class LoadFileListRequest : public JSONBase
        {
        public:
            void set_dir(std::string dir);
            std::string dir();
            std::string * mutable_dir();

            void set_prefix(std::string prefix);
            std::string prefix();
            std::string * mutable_prefix();

            void set_suffix(std::string suffix);
            std::string suffix();
            std::string * mutable_suffix();

        protected:
            std::string dir_;
            std::string prefix_;
            std::string suffix_;
        public:
            virtual bool Deserialize(const rapidjson::Value &obj);
			virtual bool Serialize(rapidjson::Writer<rapidjson::StringBuffer> *writer) const;
			virtual bool IsNullJSONData() const;
        };

        class LoadFileListResponse : public JSONBase
        {
        public:
            void set_files(std::vector<FileName> dir);
            std::vector<FileName> files();
            std::vector<FileName> * mutable_files();

        protected:
            std::vector<FileName> files_;
        public:
            virtual bool Deserialize(const rapidjson::Value &obj);
			virtual bool Serialize(rapidjson::Writer<rapidjson::StringBuffer> *writer) const;
			virtual bool IsNullJSONData() const;
        };
    
        class SaveFileRequest : public JSONBase
        {
        public:
            void set_dir(std::string dir);
            std::string dir();
            std::string * mutable_dir();

            void set_name(std::string name);
            std::string name();
            std::string * mutable_name();

            void set_file(File file);
            File file();
            File * mutable_file();

        protected:
            std::string dir_;
            std::string name_;
            File file_;
        public:
            virtual bool Deserialize(const rapidjson::Value &obj);
			virtual bool Serialize(rapidjson::Writer<rapidjson::StringBuffer> *writer) const;
			virtual bool IsNullJSONData() const;
        };

        class RenameFileRequest : public JSONBase
        {
        public:
            void set_from(FileIndex from);
            FileIndex from();
            FileIndex * mutable_from();

            void set_to(FileIndex to);
            FileIndex to();
            FileIndex * mutable_to();
        protected:
            FileIndex from_;
            FileIndex to_;
        public:
            virtual bool Deserialize(const rapidjson::Value &obj);
			virtual bool Serialize(rapidjson::Writer<rapidjson::StringBuffer> *writer) const;
			virtual bool IsNullJSONData() const;
        };
       
        class ZipRequest : public JSONBase
        {
        public:
            void set_zip(FileIndex zip);
            FileIndex zip();
            FileIndex * mutable_zip();

            void set_files(std::vector<std::string> files);
            std::vector<std::string> files();
            std::vector<std::string> * mutable_files();

            void set_dir(std::string dir);
            std::string dir();
            std::string * mutable_dir();

        protected:
            FileIndex zip_;
            std::vector<std::string> files_;
            std::string dir_;
        public:
            virtual bool Deserialize(const rapidjson::Value &obj);
			virtual bool Serialize(rapidjson::Writer<rapidjson::StringBuffer> *writer) const;
			virtual bool IsNullJSONData() const;
        };

        class UnzipRequest : public JSONBase
        {
        public:
            void set_zip(FileIndex zip);
            FileIndex zip();
            FileIndex * mutable_zip();

            void set_files(std::vector<std::string> files);
            std::vector<std::string> files();
            std::vector<std::string> * mutable_files();

            void set_dir(std::string dir);
            std::string dir();
            std::string * mutable_dir();

        protected:
            FileIndex zip_;
            std::vector<std::string> files_;
            std::string dir_;
        public:
            virtual bool Deserialize(const rapidjson::Value &obj);
			virtual bool Serialize(rapidjson::Writer<rapidjson::StringBuffer> *writer) const;
			virtual bool IsNullJSONData() const;
        };

        class LoadZipListRequest : public JSONBase
        {
        public:
            void set_zip(FileIndex zip);
            FileIndex zip();
            FileIndex * mutable_zip();

            void set_dir(std::string dir);
            std::string dir();
            std::string *mutable_dir();

            void set_prefix(std::string prefix);
            std::string prefix();
            std::string *mutable_prefix();

            void set_suffix(std::string suffix);
            std::string suffix();
            std::string *mutable_suffix();

        protected:
            FileIndex zip_;
            std::string dir_;
            std::string prefix_;
            std::string suffix_;

        public:
            virtual bool Deserialize(const rapidjson::Value &obj);
			virtual bool Serialize(rapidjson::Writer<rapidjson::StringBuffer> *writer) const;
			virtual bool IsNullJSONData() const;
        };

        class LoadZipListResponse : public JSONBase
        {
        public:
            void set_files(std::vector<FileName> dir);
            std::vector<FileName> files();
            std::vector<FileName> * mutable_files();

        protected:
            std::vector<FileName> files_;

        public:
            virtual bool Deserialize(const rapidjson::Value &obj);
			virtual bool Serialize(rapidjson::Writer<rapidjson::StringBuffer> *writer) const;
			virtual bool IsNullJSONData() const;
        };
    }
}