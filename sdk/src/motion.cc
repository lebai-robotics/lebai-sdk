#include <string>
#include "lebai-sdk/motion.hh"

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
    v_.reset();
    if(other.v_)
    {
      v_ = std::make_unique<double>(*other.v_);
    }
    a_.reset();
    if(other.a_)
    {
      a_ = std::make_unique<double>(*other.a_);
    }
    t_.reset();
    if(other.t_)
    {
      t_ = std::make_unique<double>(*other.t_);
    }
    r_.reset();
    if(other.r_)
    {
      r_ = std::make_unique<double>(*other.r_);
    }
    return *this;
  }
  void MoveParam::set_v(double v)
  {
    v_ = std::make_unique<double>(v);
  } 
	// double v() const;
  double * MoveParam::v() const
  {
    return v_.get();
  }
  double * MoveParam::mutable_v()
  {
    if(v_ == nullptr)
    {
      v_ = std::make_unique<double>();
    }
    return v_.get();
  }
	// void set_a(double);
  void MoveParam::set_a(double a)
  {
    a_ = std::make_unique<double>(a);
  }
	// double a() const;
  double * MoveParam::a() const
  {
    return a_.get();
  }
  double * MoveParam::mutable_a()
  {
    if(a_ == nullptr)
    {
      a_ = std::make_unique<double>();
    }
    return a_.get();
  }
  // void set_t(double);
  void MoveParam::set_t(double t)
  {
    t_ = std::make_unique<double>(t);
  }
	// double t() const;
  double * MoveParam::t() const
  {
    return t_.get();
  }
  double * MoveParam::mutable_t()
  {
    if(t_ == nullptr)
    {
      t_ = std::make_unique<double>();
    }
    return t_.get();
  }
  // void set_r(double);
  void MoveParam::set_r(double r)
  {
    r_ = std::make_unique<double>(r);
  }
	// double r() const;	 
  double * MoveParam::r() const
  {
    return r_.get();
  }
  // double & mutable_r(); 
  double * MoveParam::mutable_r()
  {
    if(r_ == nullptr)
    {
      r_ = std::make_unique<double>();
    }
    return r_.get();
  }
  
  bool MoveParam::Deserialize(const rapidjson::Value& obj)
  {
    if(obj.HasMember("v"))
    {
      if(!v_)
      {
        v_ = std::make_unique<double>();
        *v_ = obj.GetDouble();
      }
    }
    else
    {
      v_.reset();
    }
    if(obj.HasMember("a"))
    {
      if(!a_)
      {
        a_ = std::make_unique<double>();
        *a_ = obj.GetDouble();
      }
    }
    else
    {
      a_.reset();
    }
    if(obj.HasMember("t"))
    {
      if(!t_)
      {
        t_ = std::make_unique<double>();
        *t_ = obj.GetDouble();
      }
    }
    else
    {
      t_.reset();
    }
    if(obj.HasMember("r"))
    {
      if(!r_)
      {
        r_ = std::make_unique<double>();
        *r_ = obj.GetDouble();
      }
    }
    else
    {
      r_.reset();
    }        
    return true;    
  }

  bool MoveParam::Serialize(rapidjson::Writer<rapidjson::StringBuffer>* writer) const
  {

    writer->StartObject();
    if(v_)
    {
      writer->String("v");
      writer->Double(*v_);
    }
    if(a_)
    {
      writer->String("a");
      writer->Double(*a_);
    }
    if(t_)
    {
      writer->String("t");
      writer->Double(*t_);
    }
    if(r_)
    {
      writer->String("r");
      writer->Double(*r_);
    }
    writer->EndObject();
    return true;
  }
  bool MoveParam::IsNullJSONData() const
  {
    if(v_ || a_ || t_ || r_)
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

  void MoveRequest::set_pose(const Pose & pose)
  {
    pose_ = pose;
  }
  const Pose & MoveRequest::pose() const
  {
    return pose_;
  }
  Pose & MoveRequest::mutable_pose()
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
}