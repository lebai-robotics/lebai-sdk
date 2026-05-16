#pragma once

#include <nlohmann/json.hpp>

#include <stdexcept>
#include <string>
#include <vector>

namespace protos_json::control_proto {

enum class TaskState {
  NONE = 0,
  RUNNING = 1,
  PAUSE = 2,
  SUCCESS = 3,
  INTERRUPT = 4,
  FAIL = 5,
  WAIT = 10,
  INTERRUPTING = 14,
};

inline void from_json(const nlohmann::json& json, TaskState& state) {
  const auto text = json.get<std::string>();
  if (text == "NONE") {
    state = TaskState::NONE;
  } else if (text == "RUNNING") {
    state = TaskState::RUNNING;
  } else if (text == "PAUSE") {
    state = TaskState::PAUSE;
  } else if (text == "SUCCESS") {
    state = TaskState::SUCCESS;
  } else if (text == "INTERRUPT") {
    state = TaskState::INTERRUPT;
  } else if (text == "FAIL") {
    state = TaskState::FAIL;
  } else if (text == "WAIT") {
    state = TaskState::WAIT;
  } else if (text == "INTERRUPTING") {
    state = TaskState::INTERRUPTING;
  } else {
    throw std::runtime_error("unknown task state: " + text);
  }
}

inline void to_json(nlohmann::json& json, const TaskState& state) {
  switch (state) {
    case TaskState::NONE:
      json = "NONE";
      return;
    case TaskState::RUNNING:
      json = "RUNNING";
      return;
    case TaskState::PAUSE:
      json = "PAUSE";
      return;
    case TaskState::SUCCESS:
      json = "SUCCESS";
      return;
    case TaskState::INTERRUPT:
      json = "INTERRUPT";
      return;
    case TaskState::FAIL:
      json = "FAIL";
      return;
    case TaskState::WAIT:
      json = "WAIT";
      return;
    case TaskState::INTERRUPTING:
      json = "INTERRUPTING";
      return;
  }
  throw std::runtime_error("invalid task state");
}

enum class TaskKind {
  LUA = 0,
  APP = 10,
};

inline void from_json(const nlohmann::json& json, TaskKind& kind) {
  const auto text = json.get<std::string>();
  if (text == "LUA") {
    kind = TaskKind::LUA;
  } else if (text == "APP") {
    kind = TaskKind::APP;
  } else {
    throw std::runtime_error("unknown task kind: " + text);
  }
}

inline void to_json(nlohmann::json& json, const TaskKind& kind) {
  switch (kind) {
    case TaskKind::LUA:
      json = "LUA";
      return;
    case TaskKind::APP:
      json = "APP";
      return;
  }
  throw std::runtime_error("invalid task kind");
}

struct TaskIds {
  std::vector<unsigned int> ids;
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(TaskIds, ids)
};

struct TaskIndex {
  unsigned int id{};
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(TaskIndex, id)
};

struct StartTaskRequest {
  std::string name;
  bool is_parallel{};
  unsigned int loop_to{1};
  std::string dir;
  unsigned int kind{};
  std::vector<std::string> params;
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(StartTaskRequest, name, is_parallel, loop_to,
                                 dir, kind, params)
};

struct TaskStdout {
  unsigned int id{};
  bool done{};
  std::string stdout_text;
};

inline void from_json(const nlohmann::json& json, TaskStdout& stdout_data) {
  json.at("id").get_to(stdout_data.id);
  json.at("done").get_to(stdout_data.done);
  if (json.contains("stdout")) {
    json.at("stdout").get_to(stdout_data.stdout_text);
  }
}

inline void to_json(nlohmann::json& json, const TaskStdout& stdout_data) {
  json = nlohmann::json{{"id", stdout_data.id},
                        {"done", stdout_data.done},
                        {"stdout", stdout_data.stdout_text}};
}

struct PauseRequest {
  unsigned int id{};
  unsigned long time{};
  bool wait{};
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(PauseRequest, id, time, wait)
};

struct HookResponse {
  bool success{};
  std::string error;
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(HookResponse, success, error)
};

struct Task {
  unsigned int id{};
  std::string block_id;
  unsigned int event_id{};
  TaskState state{TaskState::NONE};
  unsigned int loop_count{};
  unsigned int loop_to{};
  bool is_parallel{};
  bool is_simu{};
  std::string stdout_text;
  unsigned int pre_pause{};
  TaskKind kind{TaskKind::LUA};
  std::string dir;
  std::string name;
  std::vector<std::string> params;
};

inline void from_json(const nlohmann::json& json, Task& task) {
  json.at("id").get_to(task.id);
  json.at("block_id").get_to(task.block_id);
  if (json.contains("event_id")) {
    json.at("event_id").get_to(task.event_id);
  }
  json.at("state").get_to(task.state);
  if (json.contains("loop_count")) {
    json.at("loop_count").get_to(task.loop_count);
  }
  if (json.contains("loop_to")) {
    json.at("loop_to").get_to(task.loop_to);
  }
  json.at("is_parallel").get_to(task.is_parallel);
  json.at("is_simu").get_to(task.is_simu);
  if (json.contains("stdout")) {
    json.at("stdout").get_to(task.stdout_text);
  }
  if (json.contains("pre_pause")) {
    json.at("pre_pause").get_to(task.pre_pause);
  }
  json.at("kind").get_to(task.kind);
  json.at("dir").get_to(task.dir);
  json.at("name").get_to(task.name);
  json.at("params").get_to(task.params);
}

inline void to_json(nlohmann::json& json, const Task& task) {
  json = nlohmann::json{{"id", task.id},
                        {"block_id", task.block_id},
                        {"event_id", task.event_id},
                        {"state", task.state},
                        {"loop_count", task.loop_count},
                        {"loop_to", task.loop_to},
                        {"is_parallel", task.is_parallel},
                        {"is_simu", task.is_simu},
                        {"stdout", task.stdout_text},
                        {"pre_pause", task.pre_pause},
                        {"kind", task.kind},
                        {"dir", task.dir},
                        {"name", task.name},
                        {"params", task.params}};
}

struct Tasks {
  std::vector<Task> tasks;
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(Tasks, tasks)
};

}  // namespace protos_json::control_proto
