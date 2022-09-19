#include <string>
#include "lebai-sdk/posture.hh"

namespace lebai
{
  void JointPose::set_joint(const std::vector<double> & joint)
  {
    joint_ = joint;
  }
  const std::vector<double> & JointPose::joint() const
  {
    return joint_;
  }
  std::vector<double> & JointPose::mutable_joint()
  {
    return joint_;
  }
	bool JointPose::Deserialize(const rapidjson::Value & obj)
	{
    joint_.clear();
		for(auto iter = obj["joint"].GetArray().Begin(); iter != obj["joint"].GetArray().End(); iter++)
    {
      joint_.push_back(iter->GetDouble());
    }		
		return true;
	}

	bool JointPose::Serialize(rapidjson::Writer<rapidjson::StringBuffer> * writer) const
	{
		writer->StartObject();
		writer->String("joint");
		writer->StartArray();
    for (auto it = joint_.begin(); it != joint_.end(); ++it)
    {
      writer->Double(*it);
    }
    writer->EndArray();
    writer->EndObject();
		return true;
	}
  bool JointPose::IsNullJSONData() const
  {
    return joint_.empty();
  }
  
  
  void JointFrame::set_kind(Kind kind)
  {
    kind_ = kind;
  }
  JointFrame::Kind JointFrame::kind() const
  {
    return kind_;
  }
  JointFrame::Kind & JointFrame::mutable_kind()
  {
    return kind_;
  }
  void JointFrame::set_joints(const JointPose & joints)
  {
    joints_ = joints;
  }
  const JointPose & JointFrame::joints() const
  {
    return joints_;
  }
  JointPose & JointFrame::mutable_joints()
  {
    return joints_;
  }

	bool JointFrame::Deserialize(const rapidjson::Value & obj)
	{
    if(obj.HasMember("kind"))
    {
      kind_ = static_cast<Kind>(obj["kind"].GetInt());      
    }
    if(obj.HasMember("joints"))
    {
      joints_.Deserialize(obj["joints"]);
    }	
		return true;
	}

	bool JointFrame::Serialize(rapidjson::Writer<rapidjson::StringBuffer> * writer) const
	{
		writer->StartObject();
    if(kind_ != Kind::BASE)
    {
      writer->String("kind");
      writer->Int(static_cast<int>(kind_));
    }
    if(joints_.joint().size() > 0)
    {
      writer->String("joints");
      joints_.Serialize(writer);
    }
    writer->EndObject();
		return true;
	}
  bool JointFrame::IsNullJSONData() const
  {
    return joints_.IsNullJSONData() && kind_ == Kind::BASE;
  }


  // JointTargetPose begin
  void JointTargetPose::set_base(const JointFrame & base)
  {
    base_ = base;
  }
  const JointFrame & JointTargetPose::base() const
  {
    return base_;
  }
  JointFrame & JointTargetPose::mutable_base()
  {
    return base_;
  }
  void JointTargetPose::set_delta(const JointPose & joints)
  {
    delta_ = joints;
  }
  const JointPose & JointTargetPose::delta() const
  {
    return delta_;
  }
  JointPose & JointTargetPose::mutable_delta()
  {
    return delta_;
  }
  bool JointTargetPose::Deserialize(const rapidjson::Value& obj)
  {
    if(obj.HasMember("base"))
    {
      base_.Deserialize(obj["base"]);
    }
    if(obj.HasMember("delta"))
    {
      delta_.Deserialize(obj["delta"]);
    }
    return true;
  }
  bool JointTargetPose::Serialize(rapidjson::Writer<rapidjson::StringBuffer>* writer) const
  {
    writer->StartObject();
    if(!base_.IsNullJSONData())
    {
      writer->String("base");
      base_.Serialize(writer);
    }
    if(!delta_.IsNullJSONData())
    {
      writer->String("delta");
      delta_.Serialize(writer);
    }
    writer->EndObject();
    return true;
  }
  bool JointTargetPose::IsNullJSONData() const
  {
    return base_.IsNullJSONData() && delta_.IsNullJSONData();
  }
  // JointTargetPose end

  // Pose begin
  Pose & Pose::operator = (const Pose & other)
  {
    if(other.joint_)
    {
      joint_ = std::make_unique<JointTargetPose>(*other.joint_);
    }
    else
    {
      joint_.reset();                  
    }
  }
  void Pose::set_joint(const JointTargetPose & joint)
  {
    if(!joint_)
    {
      joint_ = std::make_unique<JointTargetPose>();
    }
    *joint_ = joint;
    // joint_->set_base(joint.base());
    // joint_->set_delta(joint.delta());
  }
  JointTargetPose * Pose::joint() const
  {
    return joint_.get();
  }
  JointTargetPose * Pose::mutable_joint()
  {
    if(!joint_)
    {
      joint_ = std::make_unique<JointTargetPose>();
    }
    return joint_.get();
  }

  bool Pose::Deserialize(const rapidjson::Value& obj)
  {
    if(obj.HasMember("joint"))
    {
      if(!joint_)
      {
        joint_ = std::make_unique<JointTargetPose>();
      }
      joint_->Deserialize(obj["joint"]);
    }
        
    return true;    
  }

  bool Pose::Serialize(rapidjson::Writer<rapidjson::StringBuffer>* writer) const
  {
    writer->StartObject();
    if(joint_ && !joint_->IsNullJSONData())
    {
      writer->String("joint");
      joint_->Serialize(writer);
    }
    writer->EndObject();
    return true;
  }
  bool Pose::IsNullJSONData() const
  {
    if(!joint_)
    {
      return true;
    }
    return joint_->IsNullJSONData();
  }

}