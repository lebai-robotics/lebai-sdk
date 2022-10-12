#pragma once

#include "jsonbase.hh"
#include <vector>
#include <memory>
#include <iostream>
#include "posture.hh"

namespace lebai
{
	namespace kinematic
	{

		/**
		 * @brief Data to represent a joint array data.
		 *
		 */
		class KinData : public JSONBase
		{
		public:
			// KinData() = default;		
			// KinData(const KinData & other) = default;
			KinData & operator = (const KinData & other);
			void set_actual_joint_pose(const std::vector<double> & actual_joint_pose);
			const std::vector<double> & actual_joint_pose() const;
			std::vector<double> * mutable_actual_joint_pose();
			void set_actual_joint_speed(const std::vector<double> & actual_joint_speed);
			const std::vector<double> & actual_joint_speed() const;
			std::vector<double> * mutable_actual_joint_speed();
			void set_actual_joint_acc(const std::vector<double> & actual_joint_acc);
			const std::vector<double> & actual_joint_acc() const;
			std::vector<double> * mutable_actual_joint_acc();
			void set_actual_joint_torque(const std::vector<double> & actual_joint_torque);
			const std::vector<double> & actual_joint_torque() const;
			std::vector<double> * mutable_actual_joint_torque();

			void set_target_joint_pose(const std::vector<double> & target_joint_pose);
			const std::vector<double> & target_joint_pose() const;
			std::vector<double> * mutable_target_joint_pose();
			void set_target_joint_speed(const std::vector<double> & target_joint_speed);
			const std::vector<double> & target_joint_speed() const;
			std::vector<double> * mutable_target_joint_speed();
			void set_target_joint_acc(const std::vector<double> & target_joint_acc);
			const std::vector<double> & target_joint_acc() const;
			std::vector<double> * mutable_target_joint_acc();
			void set_target_joint_torque(const std::vector<double> & target_joint_torque);
			const std::vector<double> & target_joint_torque() const;
			std::vector<double> * mutable_target_joint_torque();
			
			void set_actual_tcp_pose(const posture::CartesianPose & actual_tcp_pose);
			const posture::CartesianPose & actual_tcp_pose() const;
			posture::CartesianPose * mutable_actual_tcp_pose();
			void set_target_tcp_pose(const posture::CartesianPose & target_tcp_pose);
			const posture::CartesianPose & target_tcp_pose() const;
			posture::CartesianPose * mutable_target_tcp_pose();
			void set_actual_flange_pose(const posture::CartesianPose & actual_flange_pose);
			const posture::CartesianPose & actual_flange_pose() const;
			posture::CartesianPose * mutable_actual_flange_pose();

		protected:
			std::vector<double> actual_joint_pose_;
			std::vector<double> actual_joint_speed_;
			std::vector<double> actual_joint_acc_;
			std::vector<double> actual_joint_torque_;
			std::vector<double> target_joint_pose_;
			std::vector<double> target_joint_speed_;
			std::vector<double> target_joint_acc_;
			std::vector<double> target_joint_torque_;
			posture::CartesianPose actual_tcp_pose_;
			posture::CartesianPose target_tcp_pose_;
			posture::CartesianPose actual_flange_pose_;
			// These methods are used to serialize and deserialize the class.
			// They will not be wrapped in the SDK.
		public:
			virtual bool Deserialize(const rapidjson::Value &obj);
			virtual bool Serialize(rapidjson::Writer<rapidjson::StringBuffer> *writer) const;
			virtual bool IsNullJSONData() const;
		};

		// class KinData : public JSONBase
		// {
		// public:
		// 	// KinData() = default;		
		// 	// KinData(const KinData & other) = default;
		// 	KinData & operator = (const KinData & other);
		// 	void set_actual_joint_pose(const std::vector<double> & actual_joint_pose);
		// 	const std::vector<double> & actual_joint_pose() const;
		// 	std::vector<double> * mutable_actual_joint_pose();
		// 	void set_actual_joint_speed(const std::vector<double> & actual_joint_speed);
		// 	const std::vector<double> & actual_joint_speed() const;
		// 	std::vector<double> * mutable_actual_joint_speed();
		// 	void set_actual_joint_acc(const std::vector<double> & actual_joint_acc);
		// 	const std::vector<double> & actual_joint_acc() const;
		// 	std::vector<double> * mutable_actual_joint_acc();
		// 	void set_actual_joint_torque(const std::vector<double> & actual_joint_torque);
		// 	const std::vector<double> & actual_joint_torque() const;
		// 	std::vector<double> * mutable_actual_joint_torque();

		// 	void set_target_joint_pose(const std::vector<double> & target_joint_pose);
		// 	const std::vector<double> & target_joint_pose() const;
		// 	std::vector<double> * mutable_target_joint_pose();
		// 	void set_target_joint_speed(const std::vector<double> & target_joint_speed);
		// 	const std::vector<double> & target_joint_speed() const;
		// 	std::vector<double> * mutable_target_joint_speed();
		// 	void set_target_joint_acc(const std::vector<double> & target_joint_acc);
		// 	const std::vector<double> & target_joint_acc() const;
		// 	std::vector<double> * mutable_target_joint_acc();
		// 	void set_target_joint_torque(const std::vector<double> & target_joint_torque);
		// 	const std::vector<double> & target_joint_torque() const;
		// 	std::vector<double> * mutable_target_joint_torque();
			
		// 	void set_actual_tcp_pose(const posture::CartesianPose & actual_tcp_pose);
		// 	const posture::CartesianPose & actual_tcp_pose() const;
		// 	posture::CartesianPose * mutable_actual_tcp_pose();
		// 	void set_target_tcp_pose(const posture::CartesianPose & target_tcp_pose);
		// 	const posture::CartesianPose & target_tcp_pose() const;
		// 	posture::CartesianPose * mutable_target_tcp_pose();
		// 	void set_actual_flange_pose(const posture::CartesianPose & actual_flange_pose);
		// 	const posture::CartesianPose & actual_flange_pose() const;
		// 	posture::CartesianPose * mutable_actual_flange_pose();

		// protected:
		// 	std::vector<double> actual_joint_pose_;
		// 	std::vector<double> actual_joint_speed_;
		// 	std::vector<double> actual_joint_acc_;
		// 	std::vector<double> actual_joint_torque_;
		// 	std::vector<double> target_joint_pose_;
		// 	std::vector<double> target_joint_speed_;
		// 	std::vector<double> target_joint_acc_;
		// 	std::vector<double> target_joint_torque_;
		// 	posture::CartesianPose actual_tcp_pose_;
		// 	posture::CartesianPose target_tcp_pose_;
		// 	posture::CartesianPose actual_flange_pose_;
		// 	// These methods are used to serialize and deserialize the class.
		// 	// They will not be wrapped in the SDK.
		// public:
		// 	virtual bool Deserialize(const rapidjson::Value &obj);
		// 	virtual bool Serialize(rapidjson::Writer<rapidjson::StringBuffer> *writer) const;
		// 	virtual bool IsNullJSONData() const;
		// };		

	}
}