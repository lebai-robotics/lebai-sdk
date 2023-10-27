// #include "system_impl.hh"
#include <iostream>
#include "system.hh"

namespace lebai
{
  namespace system
  {
    void GetRobotStateResponse::set_state(RobotState state)
    {
      state_ = state;
    }
    const RobotState &GetRobotStateResponse::state() const
    {
      return state_;
    }
    RobotState *GetRobotStateResponse::mutable_state()
    {
      return &state_;
    }
    bool GetRobotStateResponse::Deserialize(const rapidjson::Value &obj)
    {
      if (obj.HasMember("state"))
      {
        std::string state_str = std::string(obj["state"].GetString());
        if (state_str == "ERROR")
        {
          state_ = ERROR;
        }
        else if (state_str == "DISCONNECTED")
        {
          state_ = DISCONNECTED;
        }
        else if (state_str == "ESTOP")
        {
          state_ = ESTOP;
        }
        else if (state_str == "BOOTING")
        {
          state_ = BOOTING;
        }
        else if (state_str == "ROBOT_OFF")
        {
          state_ = ROBOT_OFF;
        }
        else if (state_str == "ROBOT_ON")
        {
          state_ = ROBOT_ON;
        }
        else if (state_str == "IDLE")
        {
          state_ = IDLE;
        }
        else if (state_str == "PAUSED")
        {
          state_ = PAUSED;
        }
        else if (state_str == "MOVING")
        {
          state_ = MOVING;
        }
        else if (state_str == "UPDATING")
        {
          state_ = UPDATING;
        }
        else if (state_str == "STOPPING")
        {
          state_ = STOPPING;
        }
        else if (state_str == "TEACHING")
        {
          state_ = TEACHING;
        }
        else if (state_str == "STOP")
        {
          state_ = STOP;
        }
        return true;
      }
      return false;
    }
    bool GetRobotStateResponse::Serialize(rapidjson::Writer<rapidjson::StringBuffer> *writer) const
    {
      writer->StartObject();
      writer->Key("state");
      writer->Int(state_);
      writer->EndObject();
      return true;
    }
    bool GetRobotStateResponse::IsNullJSONData() const
    {
      return false;
    }

    void SystemInfo::set_name(const std::string &name)
    {
      name_ = name;
    }
    const std::string &SystemInfo::name() const
    {
      return name_;
    }

    std::string *SystemInfo::mutable_name()
    {
      return &name_;
    }
    // kernel_version	string
    void SystemInfo::set_kernel_version(const std::string &kernel_version)
    {
      kernel_version_ = kernel_version;
    }
    const std::string &SystemInfo::kernel_version() const
    {
      return kernel_version_;
    }
    std::string *SystemInfo::mutable_kernel_version()
    {
      return &kernel_version_;
    }

    bool SystemInfo::Deserialize(const rapidjson::Value &obj)
    {
      if (obj.HasMember("name"))
      {
        name_ = obj["name"].GetString();
      }
      if (obj.HasMember("kernel_version"))
      {
        kernel_version_ = obj["kernel_version"].GetString();
      }
      return true;
    }

    bool SystemInfo::Serialize(rapidjson::Writer<rapidjson::StringBuffer> *writer) const
    {
      writer->StartObject();
      writer->String("kind");
      writer->String(name_.c_str());
      writer->String("kernel_version");
      writer->String(kernel_version_.c_str());
      writer->EndObject();
      return true;
    }
    bool SystemInfo::IsNullJSONData() const
    {
      return false;
    }

    void PhyData::set_joint_temp(const std::vector<double> &joint_temp)
    {
      joint_temp_ = joint_temp;
    }
    const std::vector<double> &PhyData::joint_temp() const
    {
      return joint_temp_;
    }
    std::vector<double> *PhyData::mutable_joint_temp()
    {
      return &joint_temp_;
    }
    void PhyData::set_joint_voltage(const std::vector<double> &joint_voltage)
    {
      joint_voltage_ = joint_voltage;
    }
    const std::vector<double> &PhyData::joint_voltage() const
    {
      return joint_voltage_;
    }
    std::vector<double> *PhyData::mutable_joint_voltage()
    {
      return &joint_voltage_;
    }
    // flange_voltage_
    void PhyData::set_flange_voltage(double flange_voltage)
    {
      flange_voltage_ = flange_voltage;
    }
    double PhyData::flange_voltage() const
    {
      return flange_voltage_;
    }
    double *PhyData::mutable_flange_voltage()
    {
      return &flange_voltage_;
    }
    bool PhyData::Deserialize(const rapidjson::Value &obj)
    {
      if (obj.HasMember("joint_temp"))
      {
        joint_temp_.clear();
        for (auto iter = obj["joint_temp"].GetArray().Begin(); iter != obj["joint_temp"].GetArray().End(); iter++)
        {
          joint_temp_.push_back(iter->GetDouble());
        }
      }
      if (obj.HasMember("joint_voltage"))
      {
        joint_voltage_.clear();
        for (auto iter = obj["joint_voltage"].GetArray().Begin(); iter != obj["joint_voltage"].GetArray().End(); iter++)
        {
          joint_voltage_.push_back(iter->GetDouble());
        }
      }
      if (obj.HasMember("flange_voltage"))
      {
        flange_voltage_ = obj["flange_voltage"].GetDouble();
      }
      return true;
    }

    bool PhyData::Serialize(rapidjson::Writer<rapidjson::StringBuffer> *writer) const
    {
      writer->StartObject();
      writer->String("joint_temp");
      writer->StartArray();
      for (auto iter = joint_temp_.begin(); iter != joint_temp_.end(); iter++)
      {
        writer->Double(*iter);
      }
      writer->EndArray();
      writer->String("joint_voltage");
      writer->StartArray();
      for (auto iter = joint_voltage_.begin(); iter != joint_voltage_.end(); iter++)
      {
        writer->Double(*iter);
      }
      writer->EndArray();
      writer->String("flange_voltage");
      writer->Double(flange_voltage_);
      writer->EndObject();
      return true;
    }
    bool PhyData::IsNullJSONData() const
    {
      return false;
    }

    void GetEstopReasonResponse::set_reason(EstopReason reason)
    {
      reason_ = reason;
    }
    const EstopReason &GetEstopReasonResponse::reason() const
    {
      return reason_;
    }
    EstopReason *GetEstopReasonResponse::mutable_reason()
    {
      return &reason_;
    }
    bool GetEstopReasonResponse::Deserialize(const rapidjson::Value &obj)
    {
      if (obj.HasMember("reason"))
      {
        std::string reason_str = std::string(obj["reason"].GetString());
        if (reason_str == "NONE")
        {
          reason_ = NONE;
        }
        else if (reason_str == "SYSTEM")
        {
          reason_ = SYSTEM;
        }
        else if (reason_str == "MANUAL")
        {
          reason_ = MANUAL;
        }
        else if (reason_str == "HARD_ESTOP")
        {
          reason_ = HARD_ESTOP;
        }
        else if (reason_str == "COLLISION")
        {
          reason_ = COLLISION;
        }
        else if (reason_str == "JOINT_LIMIT")
        {
          reason_ = JOINT_LIMIT;
        }
        else if (reason_str == "EXCEED")
        {
          reason_ = EXCEED;
        }
        else if (reason_str == "TRAJECTORY_ERROR")
        {
          reason_ = TRAJECTORY_ERROR;
        }
        else if (reason_str == "COMM_ERROR")
        {
          reason_ = COMM_ERROR;
        }
        else if (reason_str == "CAN_ERROR")
        {
          reason_ = CAN_ERROR;
        }
        else if (reason_str == "JOINT_ERROR")
        {
          reason_ = JOINT_ERROR;
        }
        return true;
      }
      return false;
    }
    bool GetEstopReasonResponse::Serialize(rapidjson::Writer<rapidjson::StringBuffer> *writer) const
    {
      writer->StartObject();
      writer->Key("reason");
      writer->Int(reason_);
      writer->EndObject();
      return true;
    }
    bool GetEstopReasonResponse::IsNullJSONData() const
    {
      return false;
    }
  }
}