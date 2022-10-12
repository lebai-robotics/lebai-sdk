
#pragma once

#include "jsonbase.hh"
#include "posture.hh"
#include <vector>
#include <memory>
#include <iostream>

namespace lebai
{  
  class MoveParam : public JSONBase
	{
	public:
		MoveParam();
		MoveParam(const MoveParam & other);
		MoveParam & operator = (const MoveParam & other);
		void set_velocity(double);
		double * velocity() const;
		double * mutable_velocity();
		void set_acc(double);
		double * acc() const;
		double * mutable_acc();
		void set_time(double);
		double * t() const;
		double * mutable_time();
		void set_radius(double);
		double * radius() const;
		double * mutable_radius();
	protected:
		std::unique_ptr<double> velocity_ = nullptr;
		std::unique_ptr<double> acc_ = nullptr;
		std::unique_ptr<double> time_ = nullptr;
		std::unique_ptr<double> radius_ = nullptr;

	// These methods are used to serialize and deserialize the class.
	// They will not be wrapped in the SDK.		
	public:		
		virtual bool Deserialize(const rapidjson::Value& obj);
		virtual bool Serialize(rapidjson::Writer<rapidjson::StringBuffer>* writer) const;		
		virtual bool IsNullJSONData() const;
	};

  class MoveRequest : public JSONBase
	{
	public:
		// MoveRequest(const MoveRequest & other);
		void set_param(const MoveParam & param);
		const MoveParam & param() const;
		MoveParam & mutable_param();
		void set_pose(const posture::Pose & pose);
		const posture::Pose & pose() const;
		posture::Pose & mutable_pose();
	protected:
		MoveParam param_;
		posture::Pose pose_;
	// These methods are used to serialize and deserialize the class.
	// They will not be wrapped in the SDK.		
	public:		
		virtual bool Deserialize(const rapidjson::Value& obj);
		virtual bool Serialize(rapidjson::Writer<rapidjson::StringBuffer>* writer) const;		
		virtual bool IsNullJSONData() const;
	};

	class MovecRequest : public JSONBase
	{
	public:
		// pose_via_
		void set_pose_via(const posture::Pose & pose);
		const posture::Pose & pose_via() const;
		posture::Pose * mutable_pose_via();
		// pose_
		void set_pose(const posture::Pose & pose);
		const posture::Pose & pose() const;
		posture::Pose * mutable_pose();
		// param_
		void set_param(const MoveParam & param);
		const MoveParam & param() const;
		MoveParam * mutable_param();
		// rad_ 
		void set_rad(double rad);
		double rad() const;
		double * mutable_rad();

	protected:
		posture::Pose pose_via_;
		posture::Pose pose_;
		double rad_;
		MoveParam param_;
	// These methods are used to serialize and deserialize the class.
	// They will not be wrapped in the SDK.		
	public:		
		virtual bool Deserialize(const rapidjson::Value& obj);
		virtual bool Serialize(rapidjson::Writer<rapidjson::StringBuffer>* writer) const;		
		virtual bool IsNullJSONData() const;		
	};

		


}