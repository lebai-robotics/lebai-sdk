/**
 * Copyright 2022 lebai.ltd
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include <gtest/gtest.h>
#include <math.h>
#include <memory>
#include "protos/posture.hh"
#include "protos/motion.hh"

namespace lebai {
class ProtosTest : public ::testing::Test {
 public:
  ProtosTest() {}
  void SetUp() {}

 protected:
};
TEST_F(ProtosTest, TestPosture) {
  {
    lebai::posture::JointPose joint_pose;
    joint_pose.set_joint({0.1, 0.2});
    ASSERT_EQ(2, joint_pose.joint().size());
    EXPECT_NEAR(0.1, joint_pose.joint()[0], 1e-6);
    EXPECT_NEAR(0.2, joint_pose.joint()[1], 1e-6);
    auto joint_pose_json_str = joint_pose.ToJSONString();
    lebai::posture::JointPose joint_pose_check;
    joint_pose_check.FromJSONString(joint_pose_json_str);
    ASSERT_EQ(2, joint_pose_check.joint().size());
    EXPECT_NEAR(0.1, joint_pose_check.joint()[0], 1e-6);
    EXPECT_NEAR(0.2, joint_pose_check.joint()[1], 1e-6);
  }
  {
    lebai::posture::Position position, position_check;
    position.set_x(0.1);
    position.set_y(0.2);
    position.set_z(0.3);
    auto json_str = position.ToJSONString();
    position_check.FromJSONString(json_str);
    EXPECT_NEAR(0.1, position_check.x(), 1e-6);
    EXPECT_NEAR(0.2, *position_check.mutable_y(), 1e-6);
    EXPECT_NEAR(0.3, position_check.z(), 1e-6);
  }
  {
    lebai::posture::RotationMatrix m, m_check;
    EXPECT_NEAR(1, m_check.data()[0], 1e-6);
    EXPECT_NEAR(1, m_check.data()[4], 1e-6);
    EXPECT_NEAR(1, m_check.data()[8], 1e-6);
    m.set_data({{1, 2, 3, 4, 5, 6, 7, 8, 9}});
    auto json_str = m.ToJSONString();
    m_check.FromJSONString(json_str);
    EXPECT_NEAR(1, m_check.data()[0], 1e-6);
    EXPECT_NEAR(2, (*m_check.mutable_data())[1], 1e-6);
    EXPECT_NEAR(5, (*m_check.mutable_data())[4], 1e-6);
    EXPECT_NEAR(9, m_check.data()[8], 1e-6);
  }
  {
    lebai::posture::Rotation r, r_check;
    EXPECT_TRUE(r.mutable_rotation_matrix());
    auto json_str = r.ToJSONString();
    r_check.FromJSONString(json_str);
    EXPECT_TRUE(r_check.mutable_rotation_matrix());
    EXPECT_NEAR(1, r_check.rotation_matrix()->data()[4], 1e-6);
    lebai::posture::Quaternion quat;
    r.set_quaternion(quat);
    EXPECT_TRUE(r.mutable_quaternion());
    json_str = r.ToJSONString();
    r_check.FromJSONString(json_str);
    EXPECT_TRUE(r_check.mutable_rotation_matrix());
    lebai::posture::Position euler_zyx;
    r.set_euler_zyx(euler_zyx);
    EXPECT_TRUE(r.mutable_euler_zyx());
    json_str = r.ToJSONString();
    r_check.FromJSONString(json_str);
    EXPECT_TRUE(r_check.mutable_euler_zyx());
  }
  {
    lebai::posture::CartesianFrame cf, cf_check;
    cf.set_position_kind(lebai::posture::CartesianFrame::Kind::FLANGE);
    cf.mutable_position()->set_x(0.1);
    cf.mutable_position()->set_y(0.2);
    cf.mutable_position()->set_z(0.3);
    cf.set_rotation_kind(lebai::posture::CartesianFrame::Kind::TCP);
    cf.mutable_rotation()->mutable_euler_zyx()->set_x(0.4);
    cf.mutable_rotation()->mutable_euler_zyx()->set_y(0.5);
    cf.mutable_rotation()->mutable_euler_zyx()->set_z(0.6);
    auto json_str = cf.ToJSONString();
    // std::cout<<"json_str:"<<json_str<<std::endl;
    cf_check.FromJSONString(json_str);
    EXPECT_NEAR(0.1, cf_check.mutable_position()->x(), 1e-6);
    EXPECT_NEAR(0.2, cf_check.mutable_position()->y(), 1e-6);
    EXPECT_NEAR(0.3, cf_check.mutable_position()->z(), 1e-6);
    EXPECT_EQ(1, cf_check.position_kind());
    EXPECT_EQ(2, cf_check.rotation_kind());
    EXPECT_NEAR(0.4, cf_check.rotation().euler_zyx()->x(), 1e-6);
    EXPECT_NEAR(0.5, cf_check.rotation().euler_zyx()->y(), 1e-6);
    EXPECT_NEAR(0.6, cf_check.rotation().euler_zyx()->z(), 1e-6);
  }
  {
    lebai::posture::CartesianPose cp, cp_check;
    cp.mutable_position()->set_x(0.1);
    cp.mutable_position()->set_y(0.2);
    cp.mutable_position()->set_z(0.3);
    cp.mutable_rotation()->mutable_euler_zyx()->set_x(0.4);
    cp.mutable_rotation()->mutable_euler_zyx()->set_y(0.5);
    cp.mutable_rotation()->mutable_euler_zyx()->set_z(0.6);
    auto json_str = cp.ToJSONString();
    // std::cout<<"json_str:"<<json_str<<std::endl;
    cp_check.FromJSONString(json_str);
    EXPECT_NEAR(0.1, cp_check.mutable_position()->x(), 1e-6);
    EXPECT_NEAR(0.2, cp_check.mutable_position()->y(), 1e-6);
    EXPECT_NEAR(0.3, cp_check.mutable_position()->z(), 1e-6);
    EXPECT_NEAR(0.4, cp_check.rotation().euler_zyx()->x(), 1e-6);
    EXPECT_NEAR(0.5, cp_check.rotation().euler_zyx()->y(), 1e-6);
    EXPECT_NEAR(0.6, cp_check.rotation().euler_zyx()->z(), 1e-6);
  }
  // {
  //   lebai::posture::CartesianTargetPose ctp, ctp_check;
  //   ctp.mutable_base()->mutable_position()->set_x(0.1);
  //   ctp.mutable_base()->mutable_position()->set_y(0.2);
  //   ctp.mutable_base()->mutable_position()->set_z(0.3);
  //   ctp.mutable_base()->mutable_rotation()->mutable_euler_zyx()->set_x(0.4);
  //   ctp.mutable_base()->mutable_rotation()->mutable_euler_zyx()->set_y(0.5);
  //   ctp.mutable_base()->mutable_rotation()->mutable_euler_zyx()->set_z(0.6);
  //   auto json_str = ctp.ToJSONString();
  //   // std::cout<<"json_str:"<<json_str<<std::endl;
  //   ctp_check.FromJSONString(json_str);
  //   EXPECT_NEAR(0.1, ctp_check.mutable_base()->mutable_position()->x(),
  //   1e-6); EXPECT_NEAR(0.2,
  //   ctp_check.mutable_base()->mutable_position()->y(), 1e-6);
  //   EXPECT_NEAR(0.3, ctp_check.mutable_base()->mutable_position()->z(),
  //   1e-6); EXPECT_NEAR(0.4,
  //   ctp_check.mutable_base()->mutable_rotation()->mutable_euler_zyx()->x(),
  //   1e-6); EXPECT_NEAR(0.5,
  //   ctp_check.mutable_base()->mutable_rotation()->mutable_euler_zyx()->y(),
  //   1e-6); EXPECT_NEAR(0.6,
  //   ctp_check.mutable_base()->mutable_rotation()->mutable_euler_zyx()->z(),
  //   1e-6);
  // }
  {
    lebai::posture::Pose pose, pose_check;
    pose.mutable_joint()->set_joint({1.0, 2.0, 3.0, 4.0, 5.0, 6.0});
    auto json_str = pose.ToJSONString();
    pose_check.FromJSONString(json_str);
    json_str = pose_check.ToJSONString();
    // ASSERT_FALSE(pose_check.mutable_cart());
    ASSERT_EQ(6, pose_check.mutable_joint()->joint().size());
    EXPECT_NEAR(6.0, pose_check.mutable_joint()->joint()[5], 1e-6);
    pose.mutable_cart()->mutable_position()->set_x(0.1);
    pose.mutable_cart()->mutable_position()->set_y(0.2);
    pose.mutable_cart()->mutable_position()->set_z(0.3);
    json_str = pose.ToJSONString();
    pose_check.FromJSONString(json_str);
    // ASSERT_FALSE(pose_check.joint());
    EXPECT_NEAR(0.1, pose_check.mutable_cart()->position().x(), 1e-6);
    EXPECT_NEAR(0.2, pose_check.mutable_cart()->position().y(), 1e-6);
    EXPECT_NEAR(0.3, pose_check.mutable_cart()->position().z(), 1e-6);

    // EXPECT_NEAR(0.2, ctp_check.mutable_base()->mutable_position()->y(),
    // 1e-6); EXPECT_NEAR(0.3,
    // ctp_check.mutable_base()->mutable_position()->z(), 1e-6);
    // EXPECT_NEAR(0.4,
    // ctp.mutable_base()->mutable_rotation()->mutable_euler_zyx()->x(), 1e-6);
    // EXPECT_NEAR(0.5,
    // ctp.mutable_base()->mutable_rotation()->mutable_euler_zyx()->y(), 1e-6);
    // EXPECT_NEAR(0.6,
    // ctp.mutable_base()->mutable_rotation()->mutable_euler_zyx()->z(), 1e-6);
  }
  {
    lebai::posture::Rotation rot, rot_check;
    rot.mutable_euler_zyx()->set_x(0.1);
    rot.mutable_euler_zyx()->set_y(0.2);
    rot.mutable_euler_zyx()->set_z(0.3);
    auto json_str = rot.ToJSONString();
    rot_check.FromJSONString(json_str);
    EXPECT_NEAR(0.1, rot_check.euler_zyx()->x(), 1e-6);
    EXPECT_NEAR(0.2, rot_check.euler_zyx()->y(), 1e-6);
    EXPECT_NEAR(0.3, rot_check.euler_zyx()->z(), 1e-6);
    // std::cout<<"json_str:"<<json_str<<std::endl;
  }
  {
    lebai::posture::GetPoseTransRequest req, req_check;
    req.mutable_from()->mutable_cart()->mutable_position()->set_x(0.1);
    req.mutable_from()->mutable_cart()->mutable_position()->set_y(0.2);
    req.mutable_from()->mutable_cart()->mutable_position()->set_z(0.3);
    req.mutable_from()
        ->mutable_cart()
        ->mutable_rotation()
        ->mutable_euler_zyx()
        ->set_x(0.4);
    req.mutable_from()
        ->mutable_cart()
        ->mutable_rotation()
        ->mutable_euler_zyx()
        ->set_y(0.5);
    req.mutable_from()
        ->mutable_cart()
        ->mutable_rotation()
        ->mutable_euler_zyx()
        ->set_z(0.6);

    req.mutable_from_to()->mutable_cart()->mutable_position()->set_x(0.1);
    req.mutable_from_to()->mutable_cart()->mutable_position()->set_y(0.2);
    req.mutable_from_to()->mutable_cart()->mutable_position()->set_z(0.3);
    req.mutable_from_to()
        ->mutable_cart()
        ->mutable_rotation()
        ->mutable_euler_zyx()
        ->set_x(0.4);
    req.mutable_from_to()
        ->mutable_cart()
        ->mutable_rotation()
        ->mutable_euler_zyx()
        ->set_y(0.5);
    req.mutable_from_to()
        ->mutable_cart()
        ->mutable_rotation()
        ->mutable_euler_zyx()
        ->set_z(0.6);
    auto json_str = req.ToJSONString();
    req_check.FromJSONString(json_str);
    EXPECT_NEAR(
        0.1, req_check.mutable_from()->mutable_cart()->mutable_position()->x(),
        1e-6);
    EXPECT_NEAR(
        0.2, req_check.mutable_from()->mutable_cart()->mutable_position()->y(),
        1e-6);
    EXPECT_NEAR(
        0.3, req_check.mutable_from()->mutable_cart()->mutable_position()->z(),
        1e-6);
    EXPECT_NEAR(0.4,
                req_check.mutable_from()
                    ->mutable_cart()
                    ->mutable_rotation()
                    ->mutable_euler_zyx()
                    ->x(),
                1e-6);
    EXPECT_NEAR(0.5,
                req_check.mutable_from()
                    ->mutable_cart()
                    ->mutable_rotation()
                    ->mutable_euler_zyx()
                    ->y(),
                1e-6);
    EXPECT_NEAR(0.6,
                req_check.mutable_from()
                    ->mutable_cart()
                    ->mutable_rotation()
                    ->mutable_euler_zyx()
                    ->z(),
                1e-6);

    EXPECT_NEAR(
        0.1,
        req_check.mutable_from_to()->mutable_cart()->mutable_position()->x(),
        1e-6);
    EXPECT_NEAR(
        0.2,
        req_check.mutable_from_to()->mutable_cart()->mutable_position()->y(),
        1e-6);
    EXPECT_NEAR(
        0.3,
        req_check.mutable_from_to()->mutable_cart()->mutable_position()->z(),
        1e-6);
    EXPECT_NEAR(0.4,
                req_check.mutable_from_to()
                    ->mutable_cart()
                    ->mutable_rotation()
                    ->mutable_euler_zyx()
                    ->x(),
                1e-6);
    EXPECT_NEAR(0.5,
                req_check.mutable_from_to()
                    ->mutable_cart()
                    ->mutable_rotation()
                    ->mutable_euler_zyx()
                    ->y(),
                1e-6);
    EXPECT_NEAR(0.6,
                req_check.mutable_from_to()
                    ->mutable_cart()
                    ->mutable_rotation()
                    ->mutable_euler_zyx()
                    ->z(),
                1e-6);
  }
  {
    lebai::motion::SpeedParam data, data_check;
    *data.mutable_acc() = 0.1;
    *data.mutable_time() = 0.2;
    *data.mutable_constrained() = true;
    auto json_str = data.ToJSONString();
    data_check.FromJSONString(json_str);
    EXPECT_NEAR(0.1, *data_check.acc(), 1e-6);
    EXPECT_NEAR(0.2, *data_check.time(), 1e-6);
    EXPECT_TRUE(*data_check.constrained());

    lebai::motion::SpeedJRequest req, req_check;
    *req.mutable_param()->mutable_acc() = 0.1;
    *req.mutable_param()->mutable_time() = 0.2;
    *req.mutable_param()->mutable_constrained() = true;
    req.mutable_speed()->mutable_joint()->push_back(0.1);
    req.mutable_speed()->mutable_joint()->push_back(0.2);
    req.mutable_speed()->mutable_joint()->push_back(0.3);
    req.mutable_speed()->mutable_joint()->push_back(0.4);
    req.mutable_speed()->mutable_joint()->push_back(0.5);
    req.mutable_speed()->mutable_joint()->push_back(0.6);

    json_str = req.ToJSONString();
    req_check.FromJSONString(json_str);
    EXPECT_NEAR(0.1, *req_check.mutable_param()->mutable_acc(), 1e-6);
    EXPECT_NEAR(0.2, *req_check.mutable_param()->mutable_time(), 1e-6);
    EXPECT_TRUE(*req_check.mutable_param()->mutable_constrained());
    EXPECT_NEAR(0.1, req_check.mutable_speed()->mutable_joint()->at(0), 1e-6);
    EXPECT_NEAR(0.2, req_check.mutable_speed()->mutable_joint()->at(1), 1e-6);
    EXPECT_NEAR(0.3, req_check.mutable_speed()->mutable_joint()->at(2), 1e-6);
    EXPECT_NEAR(0.4, req_check.mutable_speed()->mutable_joint()->at(3), 1e-6);
    EXPECT_NEAR(0.5, req_check.mutable_speed()->mutable_joint()->at(4), 1e-6);
    EXPECT_NEAR(0.6, req_check.mutable_speed()->mutable_joint()->at(5), 1e-6);
  }
}
TEST_F(ProtosTest, TestMotion) {
  {
    lebai::motion::MovecRequest req, req_check;
    req.mutable_pose()->mutable_joint()->set_joint(
        {1.0, 2.0, 3.0, 4.0, 5.0, 6.0});
    req.mutable_pose_via()->mutable_joint()->set_joint(
        {0.1, 0.2, 0.3, 0.4, 0.5, 0.6});
    req.mutable_param()->set_acc(0.1);
    req.mutable_param()->set_time(0.2);
    req.mutable_param()->set_velocity(0.3);
    req.set_rad(0.5);
    auto json_str = req.ToJSONString();
    req_check.FromJSONString(json_str);
    EXPECT_NEAR(1.0, req_check.mutable_pose()->mutable_joint()->joint().at(0),
                1e-6);
    EXPECT_NEAR(2.0, req_check.mutable_pose()->mutable_joint()->joint().at(1),
                1e-6);
    EXPECT_NEAR(3.0, req_check.mutable_pose()->mutable_joint()->joint().at(2),
                1e-6);
    EXPECT_NEAR(4.0, req_check.mutable_pose()->mutable_joint()->joint().at(3),
                1e-6);
    EXPECT_NEAR(5.0, req_check.mutable_pose()->mutable_joint()->joint().at(4),
                1e-6);
    EXPECT_NEAR(6.0, req_check.mutable_pose()->mutable_joint()->joint().at(5),
                1e-6);
    EXPECT_NEAR(0.1,
                req_check.mutable_pose_via()->mutable_joint()->joint().at(0),
                1e-6);
    EXPECT_NEAR(0.2,
                req_check.mutable_pose_via()->mutable_joint()->joint().at(1),
                1e-6);
    EXPECT_NEAR(0.3,
                req_check.mutable_pose_via()->mutable_joint()->joint().at(2),
                1e-6);
    EXPECT_NEAR(0.4,
                req_check.mutable_pose_via()->mutable_joint()->joint().at(3),
                1e-6);
    EXPECT_NEAR(0.5,
                req_check.mutable_pose_via()->mutable_joint()->joint().at(4),
                1e-6);
    EXPECT_NEAR(0.6,
                req_check.mutable_pose_via()->mutable_joint()->joint().at(5),
                1e-6);
    EXPECT_NEAR(0.1, *req_check.mutable_param()->mutable_acc(), 1e-6);
    EXPECT_NEAR(0.2, *req_check.mutable_param()->mutable_time(), 1e-6);
    EXPECT_NEAR(0.3, *req_check.mutable_param()->mutable_velocity(), 1e-6);
    EXPECT_NEAR(0.5, *req_check.mutable_rad(), 1e-6);
  }
  {
    lebai::motion::JointMove joint_move;
    joint_move.set_pose(0.1);
    joint_move.set_velocity(0.2);
    joint_move.set_acc(0.3);
    auto json_str = joint_move.ToJSONString();
    // "{"pose":0.1,"velocity":0.2,"acc":0.3}"
    EXPECT_EQ("{\"pose\":0.1,\"velocity\":0.2,\"acc\":0.3}", json_str);
    lebai::motion::MovePvatRequest req;
    req.set_duration(0.4);
    req.set_joints({joint_move});
    json_str = req.ToJSONString();
    // {"duration":0.4,"joints":[{"pose":0.1,"velocity":0.2,"acc":0.3}]}
    EXPECT_EQ(
        "{\"duration\":0.4,\"joints\":[{\"pose\":0.1,\"velocity\":0.2,\"acc\":"
        "0.3}]}",
        json_str);
  }
}
}  // namespace lebai

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
