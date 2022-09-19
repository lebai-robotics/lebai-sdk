
#pragma once

#include <lebai-sdk/jsonbase.hh>
#include <vector>
#include <memory>
#include <iostream>

namespace lebai
{
	/**
	 * @brief Data to represent a joint array data.
	 * 
	 */
  class JointPose : public JSONBase
	{
	public:
		/**
		 * @brief Set the joint array data.
		 * @param joints The joint array data.
		 */
		void set_joint(const std::vector<double> & joints);
		/**
		 * @brief Return the const joint array object.
		 * @return Immutable joint array data.
		 */
		const std::vector<double> & joint() const;
		/**
		 * @brief Return the joint array reference.
		 * 
		 * @return Mutable joint array data.
		 */
		std::vector<double> & mutable_joint();
	protected:
		std::vector<double> joint_;  /*!< Joint array data */

	// These methods are used to serialize and deserialize the class.
	// They will not be wrapped in the SDK.		
	public:		
		virtual bool Deserialize(const rapidjson::Value& obj);
		virtual bool Serialize(rapidjson::Writer<rapidjson::StringBuffer>* writer) const;		
		virtual bool IsNullJSONData() const;

	};

  class JointFrame : public JSONBase
	{
	public:
		enum Kind
		{
			BASE = 0,
			CURRENT	= 1,
			LAST = 11,
			CUSTOM = 99,
		};
    // Data
		void set_kind(Kind kind);
		Kind kind() const;
		Kind & mutable_kind();
		void set_joints(const JointPose & joints) ;
		const JointPose & joints() const;
		JointPose & mutable_joints();
		
	protected:
		Kind kind_;
		JointPose joints_;
	public:
		// These methods are used to serialize and deserialize the class.
		// They will not be wrapped in the SDK.	
		virtual bool Deserialize(const rapidjson::Value& obj);
		virtual bool Serialize(rapidjson::Writer<rapidjson::StringBuffer>* writer) const;
		virtual bool IsNullJSONData() const;
	};
	// JointTargetPose
	class JointTargetPose : public JSONBase
	{
	public:
    // Data
		void set_base(const JointFrame & base);
		const JointFrame & base() const;
		JointFrame & mutable_base();
		void set_delta(const JointPose & joints) ;
		const JointPose & delta() const;
		JointPose & mutable_delta();
	protected:
		JointFrame base_;
		JointPose delta_;	
	public:	
		// These methods are used to serialize and deserialize the class.
		// They will not be wrapped in the SDK.	
		virtual bool Deserialize(const rapidjson::Value& obj);
		virtual bool Serialize(rapidjson::Writer<rapidjson::StringBuffer>* writer) const;
		virtual bool IsNullJSONData() const;
	};
	// Pose
	class Pose : public JSONBase	
	{
	public:
    // Data
		// Pose(const Pose &;
		Pose & operator = (const Pose & other);
		void set_joint(const JointTargetPose & joint);
		JointTargetPose * joint() const;
		JointTargetPose * mutable_joint();
	protected:
		std::unique_ptr<JointTargetPose> joint_ = nullptr;
	public:
		// These methods are used to serialize and deserialize the class.
		// They will not be wrapped in the SDK.	
		virtual bool Deserialize(const rapidjson::Value& obj);
		virtual bool Serialize(rapidjson::Writer<rapidjson::StringBuffer>* writer) const;
		virtual bool IsNullJSONData() const;
	};	
}