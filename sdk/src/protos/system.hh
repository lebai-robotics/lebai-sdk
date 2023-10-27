#pragma once

// #include <iostream>
#include "jsonbase.hh"
#include <memory>
#include <string>
#include <vector>

namespace lebai
{
  namespace system
  {
    enum RobotState 
    {
      ERROR = -1,
      DISCONNECTED = 0,
      ESTOP = 1,
      BOOTING = 2,
      ROBOT_OFF = 3,
      ROBOT_ON = 4,
      IDLE = 5,
      PAUSED = 6,
      MOVING = 7,
      UPDATING = 8,
      STARTING = 9,
      STOPPING = 10,
      TEACHING = 11,
      STOP = 12
    };
    class GetRobotStateResponse : public JSONBase
    {
    public:      
      void set_state(RobotState);
      const RobotState & state() const;
      RobotState * mutable_state();
    protected:
      RobotState state_;
    public:
      virtual bool Deserialize(const rapidjson::Value& obj);
      virtual bool Serialize(rapidjson::Writer<rapidjson::StringBuffer>* writer) const;		
      virtual bool IsNullJSONData() const;      
    };


    class SystemInfo : public JSONBase
    {
    public:
      // class SystemInfoImpl;
      // name	string
      void set_name(const std::string & name);
      const std::string & name() const;
      std::string * mutable_name();           
      // kernel_version	string
      void set_kernel_version(const std::string & kernel_version);
      const std::string & kernel_version() const;
      std::string * mutable_kernel_version();
      // name	string
      // kernel_version	string
      // os_version	string
      // host_name	string		
      // total_memory	uint32		
      // used_memory	uint32		
      // total_swap	uint32		
      // used_swap	uint32		
      // disks	string	repeated	
      // networks	string	repeated	
      // components	string	repeated	
      // processes	string	repeated	


    protected:
      std::string name_;
      std::string kernel_version_;
    public:
      virtual bool Deserialize(const rapidjson::Value& obj);
      virtual bool Serialize(rapidjson::Writer<rapidjson::StringBuffer>* writer) const;		
      virtual bool IsNullJSONData() const;
    };
    class PhyData : public JSONBase
    {
    public:
      // joint_temp_
      void set_joint_temp(const std::vector<double> & joint_temp);
      const std::vector<double> & joint_temp() const;
      std::vector<double> * mutable_joint_temp();
      // joint_voltage
      void set_joint_voltage(const std::vector<double> & joint_temp);
      const std::vector<double> & joint_voltage() const;
      std::vector<double> * mutable_joint_voltage();
      // flange_voltage
      void set_flange_voltage(double flange_voltage);
      double flange_voltage() const;
      double * mutable_flange_voltage();

    protected:
      std::vector<double> joint_temp_;
      std::vector<double> joint_voltage_;
      double flange_voltage_;
    public:
      virtual bool Deserialize(const rapidjson::Value& obj);
      virtual bool Serialize(rapidjson::Writer<rapidjson::StringBuffer>* writer) const;		
      virtual bool IsNullJSONData() const;
    };

    enum EstopReason 
    {
      NONE = 0,
      SYSTEM = 2,
      MANUAL = 3,
      HARD_ESTOP = 4,
      COLLISION = 5,
      JOINT_LIMIT = 6,
      EXCEED = 7,
      TRAJECTORY_ERROR = 8,
      COMM_ERROR = 11,
      CAN_ERROR = 12,
      JOINT_ERROR = 13,
    };
    class GetEstopReasonResponse : public JSONBase
    {
    public:      
      void set_reason(EstopReason);
      const EstopReason & reason() const;
      EstopReason * mutable_reason();
    protected:
      EstopReason reason_;
    public:
      virtual bool Deserialize(const rapidjson::Value& obj);
      virtual bool Serialize(rapidjson::Writer<rapidjson::StringBuffer>* writer) const;		
      virtual bool IsNullJSONData() const;      
    };

  }
} // namespace lebai
