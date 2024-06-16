#include <string>
#include "file.hh"

namespace lebai {
namespace file {
void File::set_is_dir(bool is_dir) { is_dir_ = is_dir; }
bool File::is_dir() { return is_dir_; }
bool *File::mutable_is_dir() { return &is_dir_; }
void File::set_data(std::string data) { data_ = data; }
std::string File::data() { return data_; }
std::string *File::mutable_data() { return &data_; }
bool File::Deserialize(const rapidjson::Value &obj) {
  if (obj.HasMember("is_dir")) {
    bool dir_bool = (unsigned int)(obj["is_dir"].GetBool());
    is_dir_ = dir_bool;
  }
  if (obj.HasMember("data")) {
    std::string data_str = (std::string)(obj["data"].GetString());
    data_ = data_str;
  }
  return true;
}
bool File::Serialize(rapidjson::Writer<rapidjson::StringBuffer> *writer) const {
  writer->StartObject();
  writer->Key("is_dir");
  writer->Bool(is_dir_);
  writer->Key("data");
  writer->String(data_.c_str());
  writer->EndObject();
  return true;
}
bool File::IsNullJSONData() const { return false; }

void FileIndex::set_dir(std::string dir) { dir_ = dir; }
std::string FileIndex::dir() { return dir_; }
std::string *FileIndex::mutable_dir() { return &dir_; }
void FileIndex::set_name(std::string name) { name_ = name; }
std::string FileIndex::name() { return name_; }
std::string *FileIndex::mutable_name() { return &name_; }

bool FileIndex::Deserialize(const rapidjson::Value &obj) {
  if (obj.HasMember("dir")) {
    std::string dir_str = (std::string)(obj["dir"].GetString());
    dir_ = dir_str;
  }
  if (obj.HasMember("name")) {
    std::string name_str = (std::string)(obj["name"].GetString());
    name_ = name_str;
  }
  return true;
}
bool FileIndex::Serialize(
    rapidjson::Writer<rapidjson::StringBuffer> *writer) const {
  writer->StartObject();
  writer->Key("dir");
  writer->String(dir_.c_str());
  writer->Key("name");
  writer->String(name_.c_str());
  writer->EndObject();
  return true;
}
bool FileIndex::IsNullJSONData() const { return false; }

void FileName::set_name(std::string name) { name_ = name; }
std::string FileName::name() { return name_; }
std::string *FileName::mutable_name() { return &name_; }
void FileName::set_is_dir(bool is_dir) { is_dir_ = is_dir; }
bool FileName::is_dir() { return is_dir_; }
bool *FileName::mutable_is_dir() { return &is_dir_; }

bool FileName::Deserialize(const rapidjson::Value &obj) {
  if (obj.HasMember("is_dir")) {
    bool is_dir_bool = (bool)(obj["is_dir"].GetBool());
    is_dir_ = is_dir_bool;
  }
  if (obj.HasMember("name")) {
    std::string name_str = (std::string)(obj["name"].GetString());
    name_ = name_str;
  }
  return true;
}
bool FileName::Serialize(
    rapidjson::Writer<rapidjson::StringBuffer> *writer) const {
  writer->StartObject();
  writer->Key("name");
  writer->String(name_.c_str());
  writer->Key("is_dir");
  writer->Bool(is_dir_);
  writer->EndObject();
  return true;
}
bool FileName::IsNullJSONData() const { return false; }

void LoadFileListRequest::set_dir(std::string dir) { dir_ = dir; }
std::string LoadFileListRequest::dir() { return dir_; }
std::string *LoadFileListRequest::mutable_dir() { return &dir_; }
void LoadFileListRequest::set_prefix(std::string prefix) { prefix_ = prefix; }
std::string LoadFileListRequest::prefix() { return prefix_; }
std::string *LoadFileListRequest::mutable_prefix() { return &prefix_; }
void LoadFileListRequest::set_suffix(std::string suffix) { suffix_ = suffix; }
std::string LoadFileListRequest::suffix() { return suffix_; }
std::string *LoadFileListRequest::mutable_suffix() { return &suffix_; }

bool LoadFileListRequest::Deserialize(const rapidjson::Value &obj) {
  if (obj.HasMember("dir")) {
    std::string dir_str = (std::string)(obj["dir"].GetString());
    dir_ = dir_str;
  }
  if (obj.HasMember("prefix")) {
    std::string prefix_str = (std::string)(obj["prefix"].GetString());
    prefix_ = prefix_str;
  }
  if (obj.HasMember("suffix")) {
    std::string suffix_str = (std::string)(obj["suffix"].GetString());
    suffix_ = suffix_str;
  }
  return true;
}
bool LoadFileListRequest::Serialize(
    rapidjson::Writer<rapidjson::StringBuffer> *writer) const {
  writer->StartObject();
  writer->Key("dir");
  writer->String(dir_.c_str());
  writer->Key("prefix");
  writer->String(prefix_.c_str());
  writer->Key("suffix");
  writer->String(suffix_.c_str());
  writer->EndObject();
  return true;
}
bool LoadFileListRequest::IsNullJSONData() const { return false; }

void LoadFileListResponse::set_files(std::vector<FileName> files) {
  files_ = files;
}
std::vector<FileName> LoadFileListResponse::files() { return files_; }
std::vector<FileName> *LoadFileListResponse::mutable_files() { return &files_; }
bool LoadFileListResponse::Deserialize(const rapidjson::Value &obj) {
  if (obj.HasMember("files")) {
    for (auto file = obj["files"].GetArray().Begin();
         file != obj["files"].GetArray().End(); file++) {
      FileName temp;
      temp.FromJSONString((std::string)(file->GetString()));
      files_.push_back(temp);
    }
  }
  return true;
}
bool LoadFileListResponse::Serialize(
    rapidjson::Writer<rapidjson::StringBuffer> *writer) const {
  writer->StartObject();
  writer->Key("files");
  writer->StartArray();
  for (auto file : files_) {
    writer->String(file.ToJSONString().c_str());
  }
  writer->EndArray();
  writer->EndObject();
  return true;
}
bool LoadFileListResponse::IsNullJSONData() const { return false; }

void SaveFileRequest::set_dir(std::string dir) { dir_ = dir; }
std::string SaveFileRequest::dir() { return dir_; }
std::string *SaveFileRequest::mutable_dir() { return &dir_; }
void SaveFileRequest::set_name(std::string name) { name_ = name; }
std::string SaveFileRequest::name() { return name_; }
std::string *SaveFileRequest::mutable_name() { return &name_; }
void SaveFileRequest::set_file(File file) { file_ = file; }
File SaveFileRequest::file() { return file_; }
File *SaveFileRequest::mutable_file() { return &file_; }

bool SaveFileRequest::Deserialize(const rapidjson::Value &obj) {
  if (obj.HasMember("dir")) {
    std::string dir_str = (std::string)(obj["dir"].GetString());
    dir_ = dir_str;
  }
  if (obj.HasMember("name")) {
    std::string name_str = (std::string)(obj["name"].GetString());
    name_ = name_str;
  }
  if (obj.HasMember("file")) {
    File temp;
    temp.FromJSONString((std::string)(obj["name"].GetString()));
    file_ = temp;
  }
  return true;
}
bool SaveFileRequest::Serialize(
    rapidjson::Writer<rapidjson::StringBuffer> *writer) const {
  writer->StartObject();
  writer->Key("dir");
  writer->String(dir_.c_str());
  writer->Key("name");
  writer->String(name_.c_str());
  writer->Key("file");
  writer->String(file_.ToJSONString().c_str());
  writer->EndObject();
  return true;
}
bool SaveFileRequest::IsNullJSONData() const { return false; }

void RenameFileRequest::set_from(FileIndex from) { from_ = from; }
FileIndex RenameFileRequest::from() { return from_; }
FileIndex *RenameFileRequest::mutable_from() { return &from_; }
void RenameFileRequest::set_to(FileIndex to) { to_ = to; }
FileIndex RenameFileRequest::to() { return to_; }
FileIndex *RenameFileRequest::mutable_to() { return &to_; }
bool RenameFileRequest::Deserialize(const rapidjson::Value &obj) {
  if (obj.HasMember("from")) {
    FileIndex from_str;
    from_str.FromJSONString((std::string)(obj["from"].GetString()));
    from_ = from_str;
  }
  if (obj.HasMember("to")) {
    FileIndex to_str;
    to_str.FromJSONString((std::string)(obj["to"].GetString()));
    to_ = to_str;
  }
  return true;
}
bool RenameFileRequest::Serialize(
    rapidjson::Writer<rapidjson::StringBuffer> *writer) const {
  writer->StartObject();
  writer->Key("from");
  writer->String(from_.ToJSONString().c_str());
  writer->Key("to");
  writer->String(to_.ToJSONString().c_str());
  writer->EndObject();
  return true;
}
bool RenameFileRequest::IsNullJSONData() const { return false; }

void ZipRequest::set_zip(file::FileIndex zip) { zip_ = zip; }
file::FileIndex ZipRequest::zip() { return zip_; }
file::FileIndex *ZipRequest::mutable_zip() { return &zip_; }

void ZipRequest::set_files(std::vector<std::string> files) { files_ = files; }
std::vector<std::string> ZipRequest::files() { return files_; }
std::vector<std::string> *ZipRequest::mutable_files() { return &files_; }

void ZipRequest::set_dir(std::string dir) { dir_ = dir; }
std::string ZipRequest::dir() { return dir_; }
std::string *ZipRequest::mutable_dir() { return &dir_; }
bool ZipRequest::Deserialize(const rapidjson::Value &obj) {
  if (obj.HasMember("zip")) {
    FileIndex zip_str;
    zip_str.FromJSONString((std::string)(obj["zip"].GetString()));
    zip_ = zip_str;
  }
  if (obj.HasMember("files")) {
    std::vector<std::string> files;
    for (auto i = obj["files"].GetArray().Begin();
         i != obj["files"].GetArray().End(); i++) {
      files.push_back(i->GetString());
    }
    files_ = files;
  }
  if (obj.HasMember("dir")) {
    std::string dir_str = (std::string)(obj["dir"].GetString());
    dir_ = dir_str;
  }
  return true;
}
bool ZipRequest::Serialize(
    rapidjson::Writer<rapidjson::StringBuffer> *writer) const {
  writer->StartObject();
  writer->Key("zip");
  writer->String(zip_.ToJSONString().c_str());
  writer->Key("files");
  writer->StartArray();
  for (auto f : files_) {
    writer->String(f.c_str());
  }
  writer->EndArray();
  writer->Key("dir");
  writer->String(dir_.c_str());
  writer->EndObject();
  return true;
}
bool ZipRequest::IsNullJSONData() const { return false; }

void UnzipRequest::set_zip(file::FileIndex zip) { zip_ = zip; }
file::FileIndex UnzipRequest::zip() { return zip_; }
file::FileIndex *UnzipRequest::mutable_zip() { return &zip_; }

void UnzipRequest::set_files(std::vector<std::string> files) { files_ = files; }
std::vector<std::string> UnzipRequest::files() { return files_; }
std::vector<std::string> *UnzipRequest::mutable_files() { return &files_; }

void UnzipRequest::set_dir(std::string dir) { dir_ = dir; }
std::string UnzipRequest::dir() { return dir_; }
std::string *UnzipRequest::mutable_dir() { return &dir_; }
bool UnzipRequest::Deserialize(const rapidjson::Value &obj) {
  if (obj.HasMember("zip")) {
    FileIndex zip_str;
    zip_str.FromJSONString((std::string)(obj["zip"].GetString()));
    zip_ = zip_str;
  }
  if (obj.HasMember("files")) {
    std::vector<std::string> files;
    for (auto i = obj["files"].GetArray().Begin();
         i != obj["files"].GetArray().End(); i++) {
      files.push_back(i->GetString());
    }
    files_ = files;
  }
  if (obj.HasMember("dir")) {
    std::string dir_str = (std::string)(obj["dir"].GetString());
    dir_ = dir_str;
  }
  return true;
}
bool UnzipRequest::Serialize(
    rapidjson::Writer<rapidjson::StringBuffer> *writer) const {
  writer->StartObject();
  writer->Key("zip");
  writer->String(zip_.ToJSONString().c_str());
  writer->Key("files");
  writer->StartArray();
  for (auto f : files_) {
    writer->String(f.c_str());
  }
  writer->EndArray();
  writer->Key("dir");
  writer->String(dir_.c_str());
  writer->EndObject();
  return true;
}
bool UnzipRequest::IsNullJSONData() const { return false; }

void LoadZipListRequest::set_zip(FileIndex zip) { zip_ = zip; }
FileIndex LoadZipListRequest::zip() { return zip_; }
FileIndex *LoadZipListRequest::mutable_zip() { return &zip_; }

void LoadZipListRequest::set_dir(std::string dir) { dir_ = dir; }
std::string LoadZipListRequest::dir() { return dir_; }
std::string *LoadZipListRequest::mutable_dir() { return &dir_; }

void LoadZipListRequest::set_prefix(std::string prefix) { prefix_ = prefix; }
std::string LoadZipListRequest::prefix() { return prefix_; }
std::string *LoadZipListRequest::mutable_prefix() { return &prefix_; }

void LoadZipListRequest::set_suffix(std::string suffix) { suffix_ = suffix; }
std::string LoadZipListRequest::suffix() { return suffix_; }
std::string *LoadZipListRequest::mutable_suffix() { return &suffix_; }

bool LoadZipListRequest::Deserialize(const rapidjson::Value &obj) {
  if (obj.HasMember("zip")) {
    FileIndex zip_str;
    zip_str.FromJSONString((std::string)(obj["zip"].GetString()));
    zip_ = zip_str;
  }
  if (obj.HasMember("dir")) {
    std::string dir_str = (std::string)(obj["dir"].GetString());
    dir_ = dir_str;
  }
  if (obj.HasMember("prefix")) {
    std::string prefix_str = (std::string)(obj["prefix"].GetString());
    prefix_ = prefix_str;
  }
  if (obj.HasMember("suffix")) {
    std::string suffix_str = (std::string)(obj["suffix"].GetString());
    suffix_ = suffix_str;
  }
  return true;
}
bool LoadZipListRequest::Serialize(
    rapidjson::Writer<rapidjson::StringBuffer> *writer) const {
  writer->StartObject();
  writer->Key("zip");
  writer->String(zip_.ToJSONString().c_str());
  writer->Key("dir");
  writer->String(dir_.c_str());
  writer->Key("prefix");
  writer->String(prefix_.c_str());
  writer->Key("suffix");
  writer->String(suffix_.c_str());
  writer->EndObject();
  return true;
}
bool LoadZipListRequest::IsNullJSONData() const { return false; }

void LoadZipListResponse::set_files(std::vector<FileName> files) {
  files_ = files;
}
std::vector<FileName> LoadZipListResponse::files() { return files_; }
std::vector<FileName> *LoadZipListResponse::mutable_files() { return &files_; }
bool LoadZipListResponse::Deserialize(const rapidjson::Value &obj) {
  if (obj.HasMember("files")) {
    for (auto file = obj["files"].GetArray().Begin();
         file != obj["files"].GetArray().End(); file++) {
      FileName temp;
      temp.FromJSONString((std::string)(file->GetString()));
      files_.push_back(temp);
    }
  }
  return true;
}
bool LoadZipListResponse::Serialize(
    rapidjson::Writer<rapidjson::StringBuffer> *writer) const {
  writer->StartObject();
  writer->Key("files");
  writer->StartArray();
  for (auto file : files_) {
    writer->String(file.ToJSONString().c_str());
  }
  writer->EndArray();
  writer->EndObject();
  return true;
}
bool LoadZipListResponse::IsNullJSONData() const { return false; }
}  // namespace file
}  // namespace lebai