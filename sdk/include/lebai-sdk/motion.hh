
#pragma once

#include <lebai-sdk/jsonbase.hh>
#include <lebai-sdk/posture.hh>
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
		void set_v(double);
		double * v() const;
		double * mutable_v();
		void set_a(double);
		double * a() const;
		double * mutable_a();
		void set_t(double);
		double * t() const;
		double * mutable_t();
		void set_r(double);
		double * r() const;
		double * mutable_r();
	protected:
		std::unique_ptr<double> v_ = nullptr;
		std::unique_ptr<double> a_ = nullptr;
		std::unique_ptr<double> t_ = nullptr;
		std::unique_ptr<double> r_ = nullptr;

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
		void set_param(const MoveParam & param) ;
		const MoveParam & param() const;
		MoveParam & mutable_param();
		void set_pose(const Pose & pose);
		const Pose & pose() const;
		Pose & mutable_pose();
	protected:
		MoveParam param_;
		Pose pose_;
	// These methods are used to serialize and deserialize the class.
	// They will not be wrapped in the SDK.		
	public:		
		virtual bool Deserialize(const rapidjson::Value& obj);
		virtual bool Serialize(rapidjson::Writer<rapidjson::StringBuffer>* writer) const;		
		virtual bool IsNullJSONData() const;
	};

		


}