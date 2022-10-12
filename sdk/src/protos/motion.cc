#include <string>
#include <cmath>
#include "motion.hh"

namespace lebai
{
  // MoveParam begin
  MoveParam::MoveParam()
  {
    
  }
  MoveParam::MoveParam(const MoveParam & other)
  {
    *this = other;
    // if(other.v_)
    // {
    //   v_ = std::make_unique<double>(other.v_);
    // }
    // if(other.a_)
    // {
    //   a_ = std::make_unique<double>(other.a_);
    // }
    // if(other.t_)
    // {
    //   t_ = std::make_unique<double>(other.t_);
    // }
    // if(other.r_)
    // {
    //   r_ = std::make_unique<double>(other.r_);
    // }
  }
  MoveParam & MoveParam::operator = (const MoveParam & other)
  {
    velocity_.reset();
    if(other.velocity_)
    {
      velocity_ = std::make_unique<double>(*other.velocity_);
    }
    acc_.reset();
    if(other.acc_)
    {
      acc_ = std::make_unique<double>(*other.acc_);
    }
    time_.reset();
    if(other.time_)
    {
      time_ = std::make_unique<double>(*other.time_);
    }
    radius_.reset();
    if(other.radius_)
    {
      radius_ = std::make_unique<double>(*other.radius_);
    }
    return *this;
  }
  void MoveParam::set_velocity(double v)
  {
    velocity_ = std::make_unique<double>(v);
  } 
	// double v() const;
  double * MoveParam::velocity() const
  {
    return velocity_.get();
  }
  double * MoveParam::mutable_velocity()
  {
    if(velocity_ == nullptr)
    {
      velocity_ = std::make_unique<double>();
    }
    return velocity_.get();
  }
	// void set_a(double);
  void MoveParam::set_acc(double a)
  {
    acc_ = std::make_unique<double>(a);
  }
	// double a() const;
  double * MoveParam::acc() const
  {
    return acc_.get();
  }
  double * MoveParam::mutable_acc()
  {
    if(acc_ == nullptr)
    {
      acc_ = std::make_unique<double>();
    }
    return acc_.get();
  }
  // void set_t(double);
  void MoveParam::set_time(double time)
  {
    time_ = std::make_unique<double>(time);
  }
	// double t() const;
  double * MoveParam::t() const
  {
    return time_.get();
  }
  double * MoveParam::mutable_time()
  {
    if(time_ == nullptr)
    {
      time_ = std::make_unique<double>();
    }
    return time_.get();
  }
  // void set_r(double);
  void MoveParam::set_radius(double r)
  {
    radius_ = std::make_unique<double>(r);
  }
	// double r() const;	 
  double * MoveParam::radius() const
  {
    return radius_.get();
  }
  // double & mutable_r(); 
  double * MoveParam::mutable_radius()
  {
    if(radius_ == nullptr)
    {
      radius_ = std::make_unique<double>();
    }
    return radius_.get();
  }
  
  bool MoveParam::Deserialize(const rapidjson::Value& obj)
  {
    if(obj.HasMember("velocity"))
    {
      if(!velocity_)
      {
        velocity_ = std::make_unique<double>();
        *velocity_ = obj["velocity"].GetDouble();
      }
    }
    else
    {
      velocity_.reset();
    }
    if(obj.HasMember("acc"))
    {
      if(!acc_)
      {
        acc_ = std::make_unique<double>();
        *acc_ = obj["acc"].GetDouble();
      }
    }
    else
    {
      acc_.reset();
    }
    if(obj.HasMember("time"))
    {
      if(!time_)
      {
        time_ = std::make_unique<double>();
        *time_ = obj["time"].GetDouble();
      }
    }
    else
    {
      time_.reset();
    }
    if(obj.HasMember("radius"))
    {
      if(!radius_)
      {
        radius_ = std::make_unique<double>();
        *radius_ = obj["radius"].GetDouble();
      }
    }
    else
    {
      radius_.reset();
    }        
    return true;    
  }

  bool MoveParam::Serialize(rapidjson::Writer<rapidjson::StringBuffer>* writer) const
  {

    writer->StartObject();
    if(velocity_)
    {
      writer->String("velocity");
      writer->Double(*velocity_);
    }
    if(acc_)
    {
      writer->String("acc");
      writer->Double(*acc_);
    }
    if(time_)
    {
      writer->String("time");
      writer->Double(*time_);
    }
    if(radius_)
    {
      writer->String("radius");
      writer->Double(*radius_);
    }
    writer->EndObject();
    return true;
  }
  bool MoveParam::IsNullJSONData() const
  {
    if(velocity_ || acc_ || time_ || radius_)
    {
      return false;
    }
    return true;
  }
  // MoveParam end


  // MoveRequest begin
  void MoveRequest::set_param(const MoveParam & param)
  {
    param_ = param;
  }
	// double v() const;
  const MoveParam & MoveRequest::param() const
  {
    return param_;
  }
  MoveParam & MoveRequest::mutable_param()
  {
    return param_;
  }

  void MoveRequest::set_pose(const posture::Pose & pose)
  {
    pose_ = pose;
  }
  const posture::Pose & MoveRequest::pose() const
  {
    return pose_;
  }
  posture::Pose & MoveRequest::mutable_pose()
  {
    return pose_;
  }
  
  bool MoveRequest::Deserialize(const rapidjson::Value& obj)
  {
    if(obj.HasMember("param"))
    {
      param_.Deserialize(obj["param"]);
    }
    if(obj.HasMember("pose"))
    {
      pose_.Deserialize(obj["pose"]);
    }    
    return true;    
  }

  bool MoveRequest::Serialize(rapidjson::Writer<rapidjson::StringBuffer>* writer) const
  {
    writer->StartObject();
    if(!param_.IsNullJSONData())
    {
      writer->String("param");
      param_.Serialize(writer);
    }
    if(!pose_.IsNullJSONData())
    {
      writer->String("pose");
      pose_.Serialize(writer);
    }
    writer->EndObject();
    return true;
  }
  bool MoveRequest::IsNullJSONData() const
  {
    if(param_.IsNullJSONData() && pose_.IsNullJSONData())
    {
      return true;
    }
    return false;
  }
  // MoveParam end

  // pose_via_
  void MovecRequest::set_pose_via(const posture::Pose & pose)
  {
    pose_via_ = pose;
  }
  const posture::Pose & MovecRequest::pose_via() const
  {
    return pose_via_;
  }
  posture::Pose * MovecRequest::mutable_pose_via()
  {
    return &pose_via_;
  }
  // pose_
  void MovecRequest::set_pose(const posture::Pose & pose)
  {
    pose_ = pose;
  }
  const posture::Pose & MovecRequest::pose() const
  {
    return pose_;
  }
  posture::Pose * MovecRequest::mutable_pose()
  {
    return &pose_;
  }
  // param_
  void MovecRequest::set_param(const MoveParam & param)
  {
    param_ = param;
  }
  const MoveParam & MovecRequest::param() const
  {
    return param_;
  }
  MoveParam * MovecRequest::mutable_param()
  {
    return &param_;
  }
  // rad_ 
  void MovecRequest::set_rad(double rad)
  {
    rad_ = rad;
  }
  double MovecRequest::rad() const
  {
    return rad_;
  }
  double * MovecRequest::mutable_rad()
  {
    return &rad_;
  }
  bool MovecRequest::Deserialize(const rapidjson::Value& obj)
  {
    if(obj.HasMember("pose_via"))
    {
      pose_via_.Deserialize(obj["pose_via"]);
    }
    if(obj.HasMember("pose"))
    {
      pose_.Deserialize(obj["pose"]);
    }
    if(obj.HasMember("param"))
    {
      param_.Deserialize(obj["param"]);
    }
    if(obj.HasMember("rad"))
    {
      rad_ = obj.GetDouble();
    }
    return true;
  }
  bool MovecRequest::Serialize(rapidjson::Writer<rapidjson::StringBuffer>* writer) const
  {
    writer->StartObject();
    if(!pose_via_.IsNullJSONData())
    {
      writer->String("pose_via");
      pose_via_.Serialize(writer);
    }
    if(!pose_.IsNullJSONData())
    {
      writer->String("pose");
      pose_.Serialize(writer);
    }
    if(!param_.IsNullJSONData())
    {
      writer->String("param");
      param_.Serialize(writer);
    }
    if(rad_ != 0)
    {
      writer->String("rad");
      writer->Double(rad_);
    }
    writer->EndObject();
    return true;    
  }
  bool MovecRequest::IsNullJSONData() const
  {
    if(pose_via_.IsNullJSONData() && pose_.IsNullJSONData() && param_.IsNullJSONData() && std::abs(rad_) < 1e-6)
    {
      return true;
    }
    {
      return true;
    }
    return false;
  }


}