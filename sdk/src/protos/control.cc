#include "control.hh"

namespace lebai {
namespace control {
void StartTaskRequest::set_name(std::string name) { name_ = name; }
std::string StartTaskRequest::name() { return name_; }
std::string *StartTaskRequest::mutable_name() { return &name_; }

void StartTaskRequest::set_is_parallel(bool is_parallel) {
  is_parallel_ = is_parallel;
}
bool StartTaskRequest::is_parallel() { return is_parallel_; }
bool *StartTaskRequest::mutable_is_parallel() { return &is_parallel_; }

void StartTaskRequest::set_loop_to(unsigned int loop_to) { loop_to_ = loop_to; }
unsigned int StartTaskRequest::loop_to() { return loop_to_; }
unsigned int *StartTaskRequest::mutable_loop_to() { return &loop_to_; }

void StartTaskRequest::set_dir(std::string dir) { dir_ = dir; }
std::string StartTaskRequest::dir() { return dir_; }
std::string *StartTaskRequest::mutable_dir() { return &dir_; }

void StartTaskRequest::set_params(const std::vector<std::string> &params) {
  params_ = params;
}
std::vector<std::string> StartTaskRequest::params() { return params_; }
std::vector<std::string> *StartTaskRequest::mutable_params() {
  return &params_;
}

bool StartTaskRequest::Deserialize(const rapidjson::Value &obj) {
  if (obj.HasMember("name")) {
    std::string name_str = std::string(obj["name"].GetString());
    name_ = name_str;
  }
  if (obj.HasMember("is_parallel")) {
    bool is_parallel_bool = bool(obj["is_parallel"].GetBool());
    is_parallel_ = is_parallel_bool;
  }
  if (obj.HasMember("loop_to")) {
    unsigned int loop_to_int = (unsigned int)(obj["loop_to"].GetUint());
    loop_to_ = loop_to_int;
  }
  if (obj.HasMember("dir")) {
    std::string dir_str = std::string(obj["dir"].GetString());
    dir_ = dir_str;
  }
  if (obj.HasMember("params")) {
    const rapidjson::Value &params = obj["params"];
    for (rapidjson::SizeType i = 0; i < params.Size(); i++) {
      std::string param_str = std::string(obj[i].GetString());
      params_.push_back(param_str);
    }
  }
  return true;
}
bool StartTaskRequest::Serialize(
    rapidjson::Writer<rapidjson::StringBuffer> *writer) const {
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
  for (auto &param : params_) {
    writer->String(param.c_str());
  }
  writer->EndArray();
  writer->EndObject();
  return true;
}
bool StartTaskRequest::IsNullJSONData() const { return false; }

void TaskIds::set_ids(std::vector<unsigned int> ids) { ids_ = ids; }
std::vector<unsigned int> TaskIds::ids() { return ids_; }
std::vector<unsigned int> *TaskIds::mutable_ids() { return &ids_; }
bool TaskIds::Deserialize(const rapidjson::Value &obj) {
  if (obj.HasMember("ids")) {
    const rapidjson::Value &ids = obj["ids"];
    for (rapidjson::SizeType i = 0; i < ids.Size(); i++) {
      unsigned int id_int = (unsigned int)(obj[i].GetUint());
      ids_.push_back(id_int);
    }
  }
  return true;
}
bool TaskIds::Serialize(
    rapidjson::Writer<rapidjson::StringBuffer> *writer) const {
  writer->StartObject();
  writer->Key("ids");
  writer->StartArray();
  for (auto &id : ids_) {
    writer->Uint(id);
  }
  writer->EndArray();
  writer->EndObject();
  return true;
}
bool TaskIds::IsNullJSONData() const { return false; }

void TaskIndex::set_id(unsigned int id) { id_ = id; }
unsigned TaskIndex::id() { return id_; }
unsigned int *TaskIndex::mutable_id() { return &id_; }
bool TaskIndex::Deserialize(const rapidjson::Value &obj) {
  if (obj.HasMember("id")) {
    unsigned int id_int = (unsigned int)(obj["id"].GetUint());
    id_ = id_int;
  }
  return true;
}
bool TaskIndex::Serialize(
    rapidjson::Writer<rapidjson::StringBuffer> *writer) const {
  writer->StartObject();
  writer->Key("id");
  writer->Uint(id_);
  writer->EndObject();
  return true;
}
bool TaskIndex::IsNullJSONData() const { return false; }

void TaskStdout::set_id(unsigned int id) { id_ = id; }
unsigned int TaskStdout::id() { return id_; }
unsigned int *TaskStdout::mutable_id() { return &id_; }

void TaskStdout::set_done(bool done) { done_ = done; }
bool TaskStdout::done() { return done_; }
bool *TaskStdout::mutable_done() { return &done_; }

void TaskStdout::set_stdout(std::string data) { stdout_ = data; }
std::string TaskStdout::get_stdout() { return stdout_; }
std::string *TaskStdout::mutable_stdout() { return &stdout_; }

bool TaskStdout::Deserialize(const rapidjson::Value &obj) {
  if (obj.HasMember("id")) {
    unsigned int id_int = (unsigned int)(obj["id"].GetUint());
    id_ = id_int;
  }
  if (obj.HasMember("done")) {
    bool done_bool = (bool)(obj["done"].GetBool());
    done_ = done_bool;
  }
  if (obj.HasMember("stdout")) {
    std::string stdout_str = (std::string)(obj["stdout"].GetString());
    stdout_ = stdout_str;
  }
  return true;
}
bool TaskStdout::Serialize(
    rapidjson::Writer<rapidjson::StringBuffer> *writer) const {
  writer->StartObject();
  writer->Key("id");
  writer->Uint(id_);
  writer->Key("done");
  writer->Bool(done_);
  writer->Key("stdout");
  writer->String(stdout_.c_str());
  writer->EndObject();
  return true;
}
bool TaskStdout::IsNullJSONData() const { return false; }

void PauseRequest::set_id(unsigned int id) { id_ = id; }
unsigned int PauseRequest::id() { return id_; }
unsigned int *PauseRequest::mutable_id() { return &id_; }

void PauseRequest::set_time(unsigned long time) { time_ = time; }
unsigned long PauseRequest::time() { return time_; }
unsigned long *PauseRequest::mutable_time() { return &time_; }

void PauseRequest::set_wait(bool wait) { wait_ = wait; }
bool PauseRequest::wait() { return wait_; }
bool *PauseRequest::mutable_wait() { return &wait_; }

bool PauseRequest::Deserialize(const rapidjson::Value &obj) {
  if (obj.HasMember("id")) {
    unsigned int id_int = (unsigned int)(obj["id"].GetUint());
    id_ = id_int;
  }
  if (obj.HasMember("time")) {
    unsigned long time_u64 = (unsigned long)(obj["time"].GetUint64());
    time_ = time_u64;
  }
  if (obj.HasMember("wait")) {
    bool wait_bool = (bool)(obj["wait"].GetBool());
    wait_ = wait_bool;
  }
  return true;
}
bool PauseRequest::Serialize(
    rapidjson::Writer<rapidjson::StringBuffer> *writer) const {
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
bool PauseRequest::IsNullJSONData() const { return false; }
void Task::set_id(unsigned int id) { id_ = id; }
unsigned int Task::id() { return id_; }
unsigned int *Task::mutable_id() { return &id_; }

void Task::set_block_id(std::string block_id) { block_id_ = block_id; }
std::string Task::block_id() { return block_id_; }
std::string *Task::mutable_block_id() { return &block_id_; }

void Task::set_state(TaskState state) { state_ = state; }
TaskState Task::state() { return state_; }
TaskState *Task::mutable_state() { return &state_; }

void Task::set_loopc(unsigned int loopc) { loopc_ = loopc; }
unsigned int Task::loopc() { return loopc_; }
unsigned int *Task::mutable_loopc() { return &loopc_; }

void Task::set_loopt(unsigned int loopt) { loopt_ = loopt; }
unsigned int Task::loopt() { return loopt_; }
unsigned int *Task::mutable_loopt() { return &loopt_; }

void Task::set_is_parallel(bool is_parallel) { is_parallel_ = is_parallel; }
bool Task::is_parallel() { return is_parallel_; }
bool *Task::mutable_is_parallel() { return &is_parallel_; }

void Task::set_is_simu(bool is_simu) { is_simu_ = is_simu; }
bool Task::is_simu() { return is_simu_; }
bool *Task::mutable_is_simu() { return &is_simu_; }

void Task::set_out(std::string out) { out_ = out; }
std::string Task::out() { return out_; }
std::string *Task::mutable_out() { return &out_; }

void Task::set_started_at(std::string started_at) { started_at_ = started_at; }
std::string Task::started_at() { return started_at_; }
std::string *Task::mutable_started_at() { return &started_at_; }

void Task::set_ended_at(std::string ended_at) { ended_at_ = ended_at; }
std::string Task::ended_at() { return ended_at_; }
std::string *Task::mutable_ended_at() { return &ended_at_; }

void Task::set_pause_at(std::string pause_at) { pause_at_ = pause_at; }
std::string Task::pause_at() { return pause_at_; }
std::string *Task::mutable_pause_at() { return &pause_at_; }

void Task::set_pre_pause(unsigned int pre_pause) { pre_pause_ = pre_pause; }
unsigned int Task::pre_pause() { return pre_pause_; }
unsigned int *Task::mutable_pre_pause() { return &pre_pause_; }

void Task::set_kind(TaskKind kind) { kind_ = kind; }
TaskKind Task::kind() { return kind_; }
TaskKind *Task::mutable_kind() { return &kind_; }

void Task::set_dir(std::string dir) { dir_ = dir; }
std::string Task::dir() { return dir_; }
std::string *Task::mutable_dir() { return &dir_; }

void Task::set_name(std::string name) { name_ = name; }
std::string Task::name() { return name_; }
std::string *Task::mutable_name() { return &name_; }

void Task::set_params(std::vector<std::string> params) { params_ = params; }
std::vector<std::string> Task::params() { return params_; }
std::vector<std::string> *Task::mutable_params() { return &params_; }
bool Task::Deserialize(const rapidjson::Value &obj) {
  if (obj.HasMember("id")) {
    id_ = (unsigned int)(obj["id"].GetUint());
  }
  if (obj.HasMember("block_id")) {
    block_id_ = (std::string)(obj["block_id"].GetString());
  }
  if (obj.HasMember("state")) {
    std::string state = (std::string)(obj["state"].GetString());
    if (state == "WAIT") {
      state_ = TaskState::WAIT;
    } else if (state == "NONE") {
      state_ = TaskState::NONE;
    } else if (state == "RUNNING") {
      state_ = TaskState::RUNNING;
    } else if (state == "PAUSE") {
      state_ = TaskState::PAUSE;
    } else if (state == "SUCCESS") {
      state_ = TaskState::SUCCESS;
    } else if (state == "INTERRUPT") {
      state_ = TaskState::INTERRUPT;
    } else if (state == "FAIL") {
      state_ = TaskState::FAIL;
    } else if (state == "INTERRUPTING") {
      state_ = TaskState::INTERRUPTING;
    }
  }
  if (obj.HasMember("loop_count")) {
    loopc_ = (unsigned int)(obj["loop_count"].GetUint());
  }
  if (obj.HasMember("loop_to")) {
    loopt_ = (unsigned int)(obj["loop_to"].GetUint());
  }
  if (obj.HasMember("is_parallel")) {
    is_parallel_ = bool(obj["is_parallel"].GetBool());
  }
  if (obj.HasMember("is_simu")) {
    is_simu_ = bool(obj["is_simu"].GetBool());
  }
  if (obj.HasMember("stdout")) {
    out_ = (std::string)(obj["stdout"].GetString());
  }
  if (obj.HasMember("started_at")) {
    started_at_ = (std::string)(obj["started_at"].GetString());
  }
  if (obj.HasMember("ended_at")) {
    ended_at_ = (std::string)(obj["ended_at"].GetString());
  }
  if (obj.HasMember("pause_at")) {
    pause_at_ = (std::string)(obj["pause_at"].GetString());
  }
  if (obj.HasMember("pre_pause")) {
    pre_pause_ = (unsigned int)(obj["pre_pause"].GetUint());
  }
  if (obj.HasMember("kind")) {
    std::string kind = (std::string)(obj["kind"].GetString());
    if (kind == "LUA") {
      kind_ = TaskKind::LUA;
    } else if (kind == "APP") {
      kind_ = TaskKind::APP;
    }
  }
  if (obj.HasMember("dir")) {
    dir_ = (std::string)(obj["dir"].GetString());
  }
  if (obj.HasMember("name")) {
    name_ = (std::string)(obj["name"].GetString());
  }
  if (obj.HasMember("params")) {
    std::vector<std::string> params;
    for (auto i = obj["params"].GetArray().Begin();
         i != obj["params"].GetArray().End(); i++) {
      params.push_back(i->GetString());
    }
    params_ = params;
  }
  return true;
}
bool Task::Serialize(rapidjson::Writer<rapidjson::StringBuffer> *writer) const {
  writer->StartObject();
  writer->Key("id");
  writer->Uint(id_);
  writer->Key("block_id");
  writer->String(block_id_.c_str());
  writer->Key("state");
  switch (state_) {
    case TaskState::NONE:
      writer->String("NONE");
      break;
    case TaskState::WAIT:
      writer->String("WAIT");
      break;
    case TaskState::RUNNING:
      writer->String("RUNNING");
      break;
    case TaskState::PAUSE:
      writer->String("PAUSE");
      break;
    case TaskState::SUCCESS:
      writer->String("SUCCESS");
      break;
    case TaskState::INTERRUPT:
      writer->String("INTERRUPT");
      break;
    case TaskState::FAIL:
      writer->String("FAIL");
      break;
    case TaskState::INTERRUPTING:
      writer->String("INTERRUPTING");
      break;
    default:
      writer->String("nil");
      break;
  };
  writer->Key("loop_count");
  writer->Uint(loopc_);
  writer->Key("loop_to");
  writer->Uint(loopt_);
  writer->Key("is_parallel");
  writer->Bool(is_parallel_);
  writer->Key("is_simu");
  writer->Bool(is_simu_);
  writer->Key("stdout");
  writer->String(out_.c_str());
  writer->Key("started_at");
  writer->String(started_at_.c_str());
  writer->Key("ended_at");
  writer->String(ended_at_.c_str());
  writer->Key("pause_at");
  writer->String(pause_at_.c_str());
  writer->Key("kind");
  switch (kind_) {
    case TaskKind::APP:
      writer->String("APP");
      break;
    case TaskKind::LUA:
      writer->String("LUA");
      break;
  }
  writer->Key("dir");
  writer->String(dir_.c_str());
  writer->Key("name");
  writer->String(name_.c_str());
  writer->Key("params");
  writer->StartArray();
  for (auto i : params_) {
    writer->String(i.c_str());
  }
  writer->EndArray();
  writer->EndObject();
  return true;
}
bool Task::IsNullJSONData() const { return false; }

void Exec::set_id(unsigned int id) { id_ = id; }
unsigned int Exec::id() { return id_; }
unsigned int *Exec::mutable_id() { return &id_; }
bool Exec::Deserialize(const rapidjson::Value &obj) {
  if (obj.HasMember("id")) {
    id_ = (unsigned int)(obj["id"].GetUint());
  }
  return true;
}
bool Exec::Serialize(rapidjson::Writer<rapidjson::StringBuffer> *writer) const {
  writer->StartObject();
  writer->Key("id");
  writer->Uint(id_);
  writer->EndObject();
  return true;
}
bool Exec::IsNullJSONData() const { return false; }

void HookResponse::set_success(unsigned int success) { success_ = success; }
unsigned int HookResponse::success() { return success_; }
unsigned int *HookResponse::mutable_success() { return &success_; }

void HookResponse::set_error(std::string error) { error_ = error; }
std::string HookResponse::error() { return error_; }
std::string *HookResponse::mutable_error() { return &error_; }
bool HookResponse::Deserialize(const rapidjson::Value &obj) {
  if (obj.HasMember("success")) {
    success_ = bool(obj["success"].GetBool());
  }
  if (obj.HasMember("error")) {
    error_ = (std::string)(obj["error"].GetString());
  }
  return true;
}
bool HookResponse::Serialize(
    rapidjson::Writer<rapidjson::StringBuffer> *writer) const {
  writer->StartObject();
  writer->Key("success");
  writer->Bool(success_);
  writer->Key("error");
  writer->String(error_.c_str());
  writer->EndObject();
  return true;
}
bool HookResponse::IsNullJSONData() const { return false; }
}  // namespace control
}  // namespace lebai