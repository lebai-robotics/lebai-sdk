#pragma once

#include <nlohmann/json.hpp>

#include <string>
#include <vector>

namespace protos_json::file_proto {

struct File {
  bool is_dir{};
  std::string data;
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(File, is_dir, data)
};

struct FileIndex {
  std::string dir;
  std::string name;
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(FileIndex, dir, name)
};

struct FileName {
  bool is_dir{};
  std::string name;
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(FileName, is_dir, name)
};

struct LoadFileListRequest {
  std::string dir;
  std::string prefix;
  std::string suffix;
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(LoadFileListRequest, dir, prefix, suffix)
};

struct LoadFileListResponse {
  std::vector<FileName> files;
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(LoadFileListResponse, files)
};

struct SaveFileRequest {
  std::string dir;
  std::string name;
  File file;
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(SaveFileRequest, dir, name, file)
};

struct RenameFileRequest {
  FileIndex from;
  FileIndex to;
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(RenameFileRequest, from, to)
};

}  // namespace protos_json::file_proto
