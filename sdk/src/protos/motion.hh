
#pragma once

#include "jsonbase.hh"
#include "posture.hh"
#include <vector>
#include <memory>
#include <iostream>

namespace lebai
{
	namespace motion 
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

	class MotionIndex : public JSONBase
	{
	public:
		void set_id(unsigned int id);
		unsigned int id() const;
		unsigned int * mutable_id();
	protected:
		unsigned int id_;
	public:		
		virtual bool Deserialize(const rapidjson::Value& obj);
		virtual bool Serialize(rapidjson::Writer<rapidjson::StringBuffer>* writer) const;		
		virtual bool IsNullJSONData() const;
	};

	enum MotionState
	{
		WAIT = 0,
		RUNNING = 1,
		FINISHED = 2,
	};
	
	class GetMotionStateResponse : public JSONBase
	{
	public:
		void set_state(MotionState state);
		const MotionState state() const;
		MotionState * mutable_state();
	protected:
		MotionState state_;
	public:		
		virtual bool Deserialize(const rapidjson::Value& obj);
		virtual bool Serialize(rapidjson::Writer<rapidjson::StringBuffer>* writer) const;		
		virtual bool IsNullJSONData() const;
	};

	class JointMove : public JSONBase
	{
	public:
		void set_pose(double pose);
		const double pose() const;
		double * mutable_pose();
		void set_velocity(double velocity);
		const double velocity() const;
		double * mutable_velocity();
		void set_acc(double acc);
		const double acc() const;
		double * mutable_acc();
	protected:
		double pose_;
		double velocity_;
		double acc_;
	public:		
		virtual bool Deserialize(const rapidjson::Value& obj);
		virtual bool Serialize(rapidjson::Writer<rapidjson::StringBuffer>* writer) const;		
		virtual bool IsNullJSONData() const;
	};
	
	class MovePvatRequest : public JSONBase
	{
	public:
		void set_duration(double duration);
		const double duration() const;
		double * mutable_duration();
		void set_joints(std::vector<JointMove> joints);
		const std::vector<JointMove> joints() const;
		std::vector<JointMove> * mutable_joints();
	protected:
		double duration_;
		std::vector<JointMove> joints_;
	public:		
		virtual bool Deserialize(const rapidjson::Value& obj);
		virtual bool Serialize(rapidjson::Writer<rapidjson::StringBuffer>* writer) const;		
		virtual bool IsNullJSONData() const;
	};

	class SpeedParam : public JSONBase
	{
	public:
		SpeedParam();
		SpeedParam(const SpeedParam & other);
		SpeedParam & operator = (const SpeedParam & other);	
		void set_acc(double duration);
		double * acc() const;
		double * mutable_acc();
		void set_time(double time);
		double * time() const;
		double * mutable_time();
		void set_constrained(bool constrained);
		bool * constrained() const;
		bool * mutable_constrained();
		
	protected:
		std::unique_ptr<double> acc_ = nullptr;
		std::unique_ptr<double> time_ = nullptr;
		std::unique_ptr<bool> constrained_ = nullptr;
	public:		
		virtual bool Deserialize(const rapidjson::Value& obj);
		virtual bool Serialize(rapidjson::Writer<rapidjson::StringBuffer>* writer) const;		
		virtual bool IsNullJSONData() const;
	};

	class SpeedJRequest : public JSONBase
	{
	public:
		void set_speed(const posture::JointPose & speed);
		const posture::JointPose & speed() const;
		posture::JointPose * mutable_speed();
		void set_param(const SpeedParam & param);
		const SpeedParam & param() const;
		SpeedParam * mutable_param();
	protected:
		posture::JointPose speed_;
		SpeedParam param_;
	public:		
		virtual bool Deserialize(const rapidjson::Value& obj);
		virtual bool Serialize(rapidjson::Writer<rapidjson::StringBuffer>* writer) const;		
		virtual bool IsNullJSONData() const;
	};
	}
}