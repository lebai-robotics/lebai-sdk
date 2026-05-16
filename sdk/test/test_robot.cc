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
#include <thread>
#include <math.h>
#include <memory>
#include "lebai/robot.hh"

namespace lebai {
class RobotTest : public ::testing::Test {
 public:
  RobotTest() : robot_(TEST_L_MASTER_IP, true) {
    // std::cout<<"TEST_L_MASTER_IP "<<TEST_L_MASTER_IP<<"\n";
  }
  void SetUp() {}

 protected:
  l_master::Robot robot_;
};
TEST_F(RobotTest, TestStartStop) {
  robot_.stop_sys();
  std::this_thread::sleep_for(std::chrono::seconds(3));
  robot_.start_sys();
  std::this_thread::sleep_for(std::chrono::seconds(1));
  EXPECT_EQ(5, robot_.get_robot_state());
  robot_.stop_sys();
  std::this_thread::sleep_for(std::chrono::seconds(5));
  EXPECT_EQ(12, robot_.get_robot_state());
  robot_.estop();
  std::this_thread::sleep_for(std::chrono::seconds(1));
  EXPECT_EQ(1, robot_.get_robot_state());
  robot_.start_sys();
  std::this_thread::sleep_for(std::chrono::seconds(1));
  EXPECT_EQ(5, robot_.get_robot_state());
  robot_.teach_mode();
  std::this_thread::sleep_for(std::chrono::seconds(1));
  EXPECT_EQ(11, robot_.get_robot_state());
  robot_.end_teach_mode();
  std::this_thread::sleep_for(std::chrono::seconds(1));
  EXPECT_EQ(5, robot_.get_robot_state());
}

TEST_F(RobotTest, TestPhase1HttpRpcSmoke) {
  EXPECT_NO_THROW(robot_.stop_sys());
  std::this_thread::sleep_for(std::chrono::seconds(2));
  EXPECT_NO_THROW(robot_.start_sys());
  std::this_thread::sleep_for(std::chrono::seconds(1));
  EXPECT_EQ(5, robot_.get_robot_state());
  EXPECT_NO_THROW(robot_.powerdown());
  std::this_thread::sleep_for(std::chrono::seconds(2));
  EXPECT_EQ(3, robot_.get_robot_state());
}

TEST_F(RobotTest, TestSystemInfoSmoke) {
  const auto info = robot_.get_system_info();
  EXPECT_FALSE(info.name.empty());
  EXPECT_FALSE(info.kernel_version.empty());
  EXPECT_FALSE(info.host_name.empty());
  EXPECT_GT(info.total_memory, 0ULL);
  EXPECT_GE(info.total_memory, info.used_memory);
}

TEST_F(RobotTest, TestEstopReasonSmoke) {
  EXPECT_EQ(robot_.get_estop_reason(), 0);
}

TEST_F(RobotTest, TestPhyDataSmoke) {
  const auto data = robot_.get_phy_data();
  EXPECT_EQ(data.joint_temperature.size(), 6U);
  EXPECT_EQ(data.joint_voltage.size(), 6U);
  EXPECT_GE(data.flange_voltage, 0.0);
}

TEST_F(RobotTest, TestKinDataSmoke) {
  const auto data = robot_.get_kin_data();
  EXPECT_EQ(data.actual_joint_pose.size(), 6U);
  EXPECT_EQ(data.target_joint_pose.size(), 6U);
  EXPECT_EQ(data.actual_tcp_pose.size(), 6U);
  EXPECT_EQ(data.target_tcp_pose.size(), 6U);
  EXPECT_EQ(data.actual_flange_pose.size(), 6U);
}

TEST_F(RobotTest, TestStopSmoke) {
  EXPECT_NO_THROW(robot_.start_sys());
  std::this_thread::sleep_for(std::chrono::seconds(1));
  EXPECT_NO_THROW(robot_.stop());
  std::this_thread::sleep_for(std::chrono::seconds(1));
  EXPECT_EQ(5, robot_.get_robot_state());
}

TEST_F(RobotTest, TestPauseResumeSmoke) {
  EXPECT_NO_THROW(robot_.start_sys());
  std::this_thread::sleep_for(std::chrono::seconds(1));
  EXPECT_NO_THROW(robot_.pause());
  EXPECT_NO_THROW(robot_.resume());
  EXPECT_EQ(5, robot_.get_robot_state());
}

TEST_F(RobotTest, TestEstopSmoke) {
  EXPECT_NO_THROW(robot_.start_sys());
  std::this_thread::sleep_for(std::chrono::seconds(1));
  EXPECT_NO_THROW(robot_.estop());
  std::this_thread::sleep_for(std::chrono::seconds(1));
  EXPECT_EQ(1, robot_.get_robot_state());
}

TEST_F(RobotTest, TestTeachModeSmoke) {
  EXPECT_NO_THROW(robot_.start_sys());
  std::this_thread::sleep_for(std::chrono::seconds(1));
  EXPECT_EQ(5, robot_.get_robot_state());
  EXPECT_NO_THROW(robot_.teach_mode());
  std::this_thread::sleep_for(std::chrono::seconds(1));
  EXPECT_EQ(11, robot_.get_robot_state());
  EXPECT_NO_THROW(robot_.end_teach_mode());
  std::this_thread::sleep_for(std::chrono::seconds(1));
  EXPECT_EQ(5, robot_.get_robot_state());
}

TEST_F(RobotTest, TestDigitalIoSmoke) {
  EXPECT_NO_THROW(robot_.start_sys());
  std::this_thread::sleep_for(std::chrono::seconds(1));
  EXPECT_NO_THROW(robot_.set_do("ROBOT", 0, static_cast<unsigned int>(1)));
  EXPECT_GE(robot_.get_do("ROBOT", 0), 0U);
  EXPECT_GE(robot_.get_di("ROBOT", 0), 0U);
  EXPECT_EQ(robot_.get_dos("ROBOT", 0, 2).size(), 2U);
  EXPECT_EQ(robot_.get_dis("ROBOT", 0, 2).size(), 2U);
}

TEST_F(RobotTest, TestAnalogIoSmoke) {
  EXPECT_NO_THROW(robot_.start_sys());
  std::this_thread::sleep_for(std::chrono::seconds(1));
  EXPECT_NO_THROW(robot_.set_ao("ROBOT", 0, 0.2));
  EXPECT_GE(robot_.get_ao("ROBOT", 0), 0.0);
  EXPECT_GE(robot_.get_ai("ROBOT", 0), 0.0);
  EXPECT_EQ(robot_.get_aos("ROBOT", 0, 2).size(), 2U);
  EXPECT_EQ(robot_.get_ais("ROBOT", 0, 2).size(), 2U);
}

TEST_F(RobotTest, TestDioModeSmoke) {
  EXPECT_NO_THROW(robot_.start_sys());
  std::this_thread::sleep_for(std::chrono::seconds(1));
  EXPECT_NO_THROW(robot_.set_dio_mode("EXTRA", 0, true));
  EXPECT_NO_THROW(robot_.get_dios_mode("EXTRA", 0, 2));
}

TEST_F(RobotTest, TestStopMoveSmoke) {
  EXPECT_NO_THROW(robot_.start_sys());
  std::this_thread::sleep_for(std::chrono::seconds(1));
  EXPECT_NO_THROW(robot_.stop_move());
}

TEST_F(RobotTest, TestMovejSmoke) {
  EXPECT_NO_THROW(robot_.start_sys());
  std::this_thread::sleep_for(std::chrono::seconds(1));
  const auto motion_id =
      robot_.movej({0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 1.0, 0.5, 0.0, 0.0);
  EXPECT_GE(motion_id, 0);
}

TEST_F(RobotTest, TestMovelSmoke) {
  EXPECT_NO_THROW(robot_.start_sys());
  std::this_thread::sleep_for(std::chrono::seconds(1));
  const auto motion_id =
      robot_.movel({0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 1.0, 0.5, 0.0, 0.0);
  EXPECT_GE(motion_id, 0);
}

TEST_F(RobotTest, TestMovejCartesianSmoke) {
  EXPECT_NO_THROW(robot_.start_sys());
  std::this_thread::sleep_for(std::chrono::seconds(1));
  const auto motion_id = robot_.movej(
      {{"x", -0.296},
       {"y", -0.295},
       {"z", 0.285},
       {"rx", 60.0 / 180.0 * M_PI},
       {"ry", -5.0 / 180.0 * M_PI},
       {"rz", 81.0 / 180.0 * M_PI}},
      3.0, 1.0, 0.0, 0.0);
  EXPECT_GE(motion_id, 0);
}

TEST_F(RobotTest, TestMovelCartesianSmoke) {
  EXPECT_NO_THROW(robot_.start_sys());
  std::this_thread::sleep_for(std::chrono::seconds(1));
  const auto motion_id = robot_.movel(
      {{"x", -0.306},
       {"y", -0.295},
       {"z", 0.285},
       {"rx", 60.0 / 180.0 * M_PI},
       {"ry", -5.0 / 180.0 * M_PI},
       {"rz", 81.0 / 180.0 * M_PI}},
      1.0, 0.5, 0.0, 0.0);
  EXPECT_GE(motion_id, 0);
}

TEST_F(RobotTest, TestTowardjSmoke) {
  EXPECT_NO_THROW(robot_.start_sys());
  std::this_thread::sleep_for(std::chrono::seconds(1));
  const auto motion_id =
      robot_.towardj({0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 1.0, 0.5, 0.0, 0.0);
  EXPECT_GE(motion_id, 0);
}

TEST_F(RobotTest, TestSpeedjSmoke) {
  EXPECT_NO_THROW(robot_.start_sys());
  std::this_thread::sleep_for(std::chrono::seconds(1));
  const auto motion_id = robot_.speedj(1.0, {0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
                                       0.1);
  EXPECT_GE(motion_id, 0);
}

TEST_F(RobotTest, TestSpeedlSmoke) {
  EXPECT_NO_THROW(robot_.start_sys());
  std::this_thread::sleep_for(std::chrono::seconds(1));
  const auto motion_id = robot_.speedl(
      1.0,
      {{"x", 0.0}, {"y", 0.0}, {"z", 0.0}, {"rx", 0.0}, {"ry", 0.0},
       {"rz", 0.0}},
      0.1,
      {{"x", 0.0}, {"y", 0.0}, {"z", 0.0}, {"rx", 0.0}, {"ry", 0.0},
       {"rz", 0.0}});
  EXPECT_GE(motion_id, 0);
}

TEST_F(RobotTest, TestMovePvatSmoke) {
  EXPECT_NO_THROW(robot_.start_sys());
  std::this_thread::sleep_for(std::chrono::seconds(1));
  EXPECT_NO_THROW(robot_.move_pvat(std::vector<double>(6, 0.0),
                                   std::vector<double>(6, 0.0),
                                   std::vector<double>(6, 0.0), 0.1));
}

TEST_F(RobotTest, TestMovecSmoke) {
  EXPECT_NO_THROW(robot_.start_sys());
  std::this_thread::sleep_for(std::chrono::seconds(1));
  EXPECT_NO_THROW(robot_.movej(
      {13.0 / 180.0 * M_PI, -52.0 / 180.0 * M_PI, 86.0 / 180.0 * M_PI,
       8.0 / 180.0 * M_PI, -59.0 / 180.0 * M_PI, -11.0 / 180.0 * M_PI},
      1.0, 0.5, 0.0, 0.0));
  const auto motion_id = robot_.movec(
      {3.0 / 180.0 * M_PI, -48.0 / 180.0 * M_PI, 78.0 / 180.0 * M_PI,
       9.0 / 180.0 * M_PI, -67.0 / 180.0 * M_PI, -3.0 / 180.0 * M_PI},
      {-28.0 / 180.0 * M_PI, -59.0 / 180.0 * M_PI, 96.0 / 180.0 * M_PI,
       -2.0 / 180.0 * M_PI, -92.0 / 180.0 * M_PI, 16.0 / 180.0 * M_PI},
      0.0, 1.0, 0.5, 0.0, 0.0);
  EXPECT_GE(motion_id, 0);
}

TEST_F(RobotTest, TestMovecCartesianSmoke) {
  EXPECT_NO_THROW(robot_.start_sys());
  std::this_thread::sleep_for(std::chrono::seconds(1));
  EXPECT_NO_THROW(robot_.movej(
      {13.0 / 180.0 * M_PI, -52.0 / 180.0 * M_PI, 86.0 / 180.0 * M_PI,
       8.0 / 180.0 * M_PI, -59.0 / 180.0 * M_PI, -11.0 / 180.0 * M_PI},
      1.0, 0.5, 0.0, 0.0));
  EXPECT_NO_THROW(robot_.wait_move());
  const auto motion_id = robot_.movec(
      {{"x", -0.282541},
       {"y", -0.168246},
       {"z", 0.265824},
       {"rx", 1.27256},
       {"ry", -0.648918},
       {"rz", 1.54549}},
      {{"x", -0.312822},
       {"y", -0.188102},
       {"z", 0.284325},
       {"rx", 1.59059},
       {"ry", -0.304173},
       {"rz", 1.39193}},
      0.0, 1.0, 0.5, 0.0, 0.0);
  EXPECT_GE(motion_id, 0);
}

TEST_F(RobotTest, TestMotionTrackingSmoke) {
  EXPECT_NO_THROW(robot_.start_sys());
  std::this_thread::sleep_for(std::chrono::seconds(1));
  const auto motion_id = robot_.movej(
      {0.0, -60.0 / 180.0 * M_PI, 80.0 / 180.0 * M_PI, -10.0 / 180.0 * M_PI,
       -60.0 / 180.0 * M_PI, 0.0},
      1.0, 0.5, 0.0, 0.0);
  EXPECT_GE(motion_id, 0);
  EXPECT_GE(robot_.get_running_motion(), 0U);
  const auto state = robot_.get_motion_state(motion_id);
  EXPECT_TRUE(state == "WAIT" || state == "RUNNING" || state == "FINISHED");
  EXPECT_NO_THROW(robot_.wait_move(motion_id));
  EXPECT_EQ("FINISHED", robot_.get_motion_state(motion_id));
}

TEST_F(RobotTest, TestRawCallSmoke) {
  const auto [code, payload] = robot_.call("hello", R"({"data":"world"})");
  EXPECT_EQ(code, 0);
  EXPECT_NE(payload.find("\"data\":\"hello, world\""), std::string::npos);
}

TEST_F(RobotTest, TestRawCallErrorContract) {
  const auto [code, payload] = robot_.call("missing_method_for_test", "{}");
  EXPECT_NE(code, 0);
  EXPECT_FALSE(payload.empty());
}
TEST_F(RobotTest, TestMove) {
  robot_.start_sys();
  std::this_thread::sleep_for(std::chrono::seconds(1));
  ASSERT_EQ(5, robot_.get_robot_state());
  {
    std::vector<double> joint_positions(6);
    joint_positions[0] = 0.0;
    joint_positions[1] = 0.0;
    joint_positions[2] = 0.0;
    joint_positions[3] = 0.0;
    joint_positions[4] = 0.0;
    joint_positions[5] = 0.0;
    robot_.movej(joint_positions, 1.0, 0.5, 0.0, 0.0);
    joint_positions[0] = 0.0;
    joint_positions[1] = -60.0 / 180.0 * M_PI;
    joint_positions[2] = 80.0 / 180.0 * M_PI;
    joint_positions[3] = -10.0 / 180.0 * M_PI;
    joint_positions[4] = -60.0 / 180.0 * M_PI;
    joint_positions[5] = 0.0;
    robot_.movej(joint_positions, 1.0, 0.5, 0.0, 0.0);
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    EXPECT_EQ(7, robot_.get_robot_state());
    robot_.wait_move();
    auto tcp = robot_.get_target_tcp_pose();
    EXPECT_NEAR(tcp["x"], -0.296, 1e-3);
    EXPECT_NEAR(tcp["y"], -0.162, 1e-3);
    EXPECT_NEAR(tcp["z"], 0.285, 1e-3);
    EXPECT_NEAR(tcp["rz"], 60.0 / 180.0 * M_PI, 1e-2);
    EXPECT_NEAR(tcp["ry"], -5.0 / 180.0 * M_PI, 1e-2);
    EXPECT_NEAR(tcp["rx"], 81.0 / 180.0 * M_PI, 1e-2);
    auto jp = robot_.get_target_joint_positions();
    ASSERT_EQ(6, jp.size());
    EXPECT_NEAR(jp[0], joint_positions[0], 1e-3);
    EXPECT_NEAR(jp[1], joint_positions[1], 1e-3);
    EXPECT_NEAR(jp[2], joint_positions[2], 1e-3);
    EXPECT_NEAR(jp[3], joint_positions[3], 1e-3);
    EXPECT_NEAR(jp[4], joint_positions[4], 1e-3);
    EXPECT_NEAR(jp[5], joint_positions[5], 1e-3);
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    EXPECT_EQ(5, robot_.get_robot_state());
    robot_.movej({{"x", -0.296},
                  {"y", -0.295},
                  {"z", 0.285},
                  {"rx", 60.0 / 180.0 * M_PI},
                  {"ry", -5.0 / 180.0 * M_PI},
                  {"rz", 81.0 / 180.0 * M_PI}},
                 3.0, 1.0, 0.0, 0.0);
    // std::this_thread::sleep_for(std::chrono::milliseconds(100));
    EXPECT_EQ(7, robot_.get_robot_state());
    robot_.wait_move(0);
    tcp = robot_.get_target_tcp_pose();
    EXPECT_NEAR(tcp["x"], -0.296, 1e-3);
    EXPECT_NEAR(tcp["y"], -0.295, 1e-3);
    EXPECT_NEAR(tcp["z"], 0.285, 1e-3);
    EXPECT_NEAR(tcp["rx"], 60.0 / 180.0 * M_PI, 1e-2);
    EXPECT_NEAR(tcp["ry"], -5.0 / 180.0 * M_PI, 1e-2);
    EXPECT_NEAR(tcp["rz"], 81.0 / 180.0 * M_PI, 1e-2);
    joint_positions[0] = 0.0;
    joint_positions[1] = -60.0 / 180.0 * M_PI;
    joint_positions[2] = 80.0 / 180.0 * M_PI;
    joint_positions[3] = -10.0 / 180.0 * M_PI;
    joint_positions[4] = -60.0 / 180.0 * M_PI;
    joint_positions[5] = 0.0;
    robot_.movel(joint_positions, 0.3, 1.0, 0.0, 0.0);
    // std::this_thread::sleep_for(std::chrono::milliseconds(100));
    // EXPECT_EQ(7, robot_.get_robot_mode());
    robot_.wait_move();
    ASSERT_EQ(6, jp.size());
    EXPECT_NEAR(jp[0], joint_positions[0], 1e-3);
    EXPECT_NEAR(jp[1], joint_positions[1], 1e-3);
    EXPECT_NEAR(jp[2], joint_positions[2], 1e-3);
    EXPECT_NEAR(jp[3], joint_positions[3], 1e-3);
    EXPECT_NEAR(jp[4], joint_positions[4], 1e-3);
    EXPECT_NEAR(jp[5], joint_positions[5], 1e-3);
    robot_.movel({{"x", -0.306},
                  {"y", -0.295},
                  {"z", 0.285},
                  {"rx", 60.0 / 180.0 * M_PI},
                  {"ry", -5.0 / 180.0 * M_PI},
                  {"rz", 81.0 / 180.0 * M_PI}},
                 1.0, 0.5, 0.0, 0.0);
    // std::this_thread::sleep_for(std::chrono::milliseconds(100));
    EXPECT_EQ(7, robot_.get_robot_state());
    robot_.wait_move();
    tcp = robot_.get_target_tcp_pose();
    EXPECT_NEAR(tcp["x"], -0.306, 1e-3);
    EXPECT_NEAR(tcp["y"], -0.295, 1e-3);
    EXPECT_NEAR(tcp["z"], 0.285, 1e-3);
    EXPECT_NEAR(tcp["rx"], 60.0 / 180.0 * M_PI, 1e-2);
    EXPECT_NEAR(tcp["ry"], -5.0 / 180.0 * M_PI, 1e-2);
    EXPECT_NEAR(tcp["rz"], 81.0 / 180.0 * M_PI, 1e-2);
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    EXPECT_EQ(5, robot_.get_robot_state());
    robot_.movej(
        {13.0 / 180.0 * M_PI, -52.0 / 180.0 * M_PI, 86.0 / 180.0 * M_PI,
         8.0 / 180.0 * M_PI, -59.0 / 180.0 * M_PI, -11.0 / 180.0 * M_PI},
        1.0, 0.5, 0.0, 0.0);
    robot_.movec(
        {3.0 / 180.0 * M_PI, -48.0 / 180.0 * M_PI, 78.0 / 180.0 * M_PI,
         9.0 / 180.0 * M_PI, -67.0 / 180.0 * M_PI, -3.0 / 180.0 * M_PI},
        {-28 / 180.0 * M_PI, -59.0 / 180.0 * M_PI, 96.0 / 180.0 * M_PI,
         -2.0 / 180.0 * M_PI, -92.0 / 180.0 * M_PI, 16.0 / 180.0 * M_PI},
        0.0, 1.0, 0.5, 0.0, 0.0);
    robot_.wait_move();
    joint_positions = robot_.get_target_joint_positions();
    ASSERT_EQ(6, joint_positions.size());
    // -28/ 180.0 * M_PI, -59.0/ 180.0 * M_PI, 96.0/ 180.0 * M_PI, -2.0/ 180.0 *
    // M_PI, -92.0/ 180.0 * M_PI, 16.0/ 180.0 * M_PI
    EXPECT_NEAR(joint_positions[0], -28.0 / 180.0 * M_PI, 1e-3);
    EXPECT_NEAR(joint_positions[1], -59.0 / 180.0 * M_PI, 1e-3);
    EXPECT_NEAR(joint_positions[2], 96.0 / 180.0 * M_PI, 1e-3);
    EXPECT_NEAR(joint_positions[3], -2.0 / 180.0 * M_PI, 1e-3);
    EXPECT_NEAR(joint_positions[4], -92.0 / 180.0 * M_PI, 1e-3);
    EXPECT_NEAR(joint_positions[5], 16.0 / 180.0 * M_PI, 1e-3);
    auto fk_resp = robot_.kinematics_forward(joint_positions);

    robot_.movej(
        {13.0 / 180.0 * M_PI, -52.0 / 180.0 * M_PI, 86.0 / 180.0 * M_PI,
         8.0 / 180.0 * M_PI, -59.0 / 180.0 * M_PI, -11.0 / 180.0 * M_PI},
        1.0, 0.5, 0.0, 0.0);
    robot_.movec({{"x", -0.282541},
                  {"y", -0.168246},
                  {"z", 0.265824},
                  {"rx", 1.27256},
                  {"ry", -0.206353},
                  {"rz", 0.937445}},
                 {{"x", -0.255832},
                  {"y", 0.00270435},
                  {"z", 0.266642},
                  {"rz", 1.27293},
                  {"ry", -0.20805},
                  {"rx", 0.94485}},
                 0.0, 1.0, 0.5, 0.0, 0.0);
    robot_.wait_move();
    joint_positions = robot_.get_target_joint_positions();
    ASSERT_EQ(6, joint_positions.size());
    fk_resp = robot_.kinematics_forward(joint_positions);
    // -0.255832, 0.00270435, 0.266642, 1.27293, -0.20805, 0.94485
    // -28/ 180.0 * M_PI, -59.0/ 180.0 * M_PI, 96.0/ 180.0 * M_PI, -2.0/ 180.0 *
    // M_PI, -92.0/ 180.0 * M_PI, 16.0/ 180.0 * M_PI
    EXPECT_NEAR(joint_positions[0], -28.0 / 180.0 * M_PI, 1e-3);
    EXPECT_NEAR(joint_positions[1], -59.0 / 180.0 * M_PI, 1e-3);
    EXPECT_NEAR(joint_positions[2], 96.0 / 180.0 * M_PI, 1e-3);
    EXPECT_NEAR(joint_positions[3], -2.0 / 180.0 * M_PI, 1e-3);
    EXPECT_NEAR(joint_positions[4], -92.0 / 180.0 * M_PI, 1e-3);
    EXPECT_NEAR(joint_positions[5], 16.0 / 180.0 * M_PI, 1e-3);
    // {-28/ 180.0 * M_PI, -59.0/ 180.0 * M_PI, 96.0/ 180.0 * M_PI, -2.0/ 180.0
    // * M_PI, -92.0/ 180.0 * M_PI, 16.0/ 180.0 * M_PI},
    //  0.0, 1.0, 0.5, 0.0, 0.0);
    // kinematics_forward: -0.282541, -0.168246, 0.265824, 1.27256, -0.206353,
    // 0.937445 kinematics_forward: -0.255832, 0.00270435, 0.266642, 1.27293,
    // -0.20805, 0.94485
    robot_.speedj(1.0, {0.5, 0.0, 0.0, 0.0, 0.0, 0.0}, 0.0);
    std::this_thread::sleep_for(std::chrono::seconds(2));
    robot_.stop_move();
    robot_.speedl(1.0,
                  {{"x", 0.0},
                   {"y", 0.0},
                   {"z", 0.1},
                   {"rx", 0.0},
                   {"ry", 0.0},
                   {"rz", 0.0}},
                  0.0);
    std::this_thread::sleep_for(std::chrono::seconds(2));
    robot_.stop_move();

    robot_.towardj({0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 1.0, 1.0, 0.0, 0.0);
    robot_.wait_move();
    joint_positions.clear();
    joint_positions = robot_.get_target_joint_positions();
    ASSERT_EQ(6, joint_positions.size());
    EXPECT_NEAR(joint_positions[0], 0.0, 1e-3);
    EXPECT_NEAR(joint_positions[1], 0.0, 1e-3);
    EXPECT_NEAR(joint_positions[2], 0.0, 1e-3);
    EXPECT_NEAR(joint_positions[3], 0.0, 1e-3);
    EXPECT_NEAR(joint_positions[4], 0.0, 1e-3);
    EXPECT_NEAR(joint_positions[5], 0.0, 1e-3);
  }
}
TEST_F(RobotTest, TestStatus) {
  robot_.start_sys();
  auto speed = robot_.get_actual_joint_speed();
  EXPECT_EQ(6, speed.size());
  speed = robot_.get_target_joint_speed();
  EXPECT_EQ(6, speed.size());
  auto cart_pose = robot_.get_actual_tcp_pose();
  EXPECT_EQ(6, cart_pose.size());
  cart_pose = robot_.get_target_tcp_pose();
  EXPECT_EQ(6, cart_pose.size());
  robot_.get_joint_temp(0);
  auto torques = robot_.get_actual_joint_torques();
  EXPECT_EQ(6, torques.size());
  torques = robot_.get_target_joint_torques();
  EXPECT_EQ(6, torques.size());
}
TEST_F(RobotTest, TestCabinetIO) {
  robot_.start_sys();
  robot_.set_do("ROBOT", 0, static_cast<unsigned int>(1));
  robot_.get_di("ROBOT", 0);
  robot_.set_ao("ROBOT", 0, 0.2);
  robot_.get_ai("ROBOT", 0);
}
TEST_F(RobotTest, TestFlangeIO) {
  robot_.start_sys();
  robot_.get_di("FLANGE", 0);
  robot_.set_do("FLANGE", 0, static_cast<unsigned int>(1));
}
TEST_F(RobotTest, TestExtraIO) {
  robot_.start_sys();
  robot_.get_di("EXTRA", 0);
  robot_.set_do("EXTRA", 0, static_cast<unsigned int>(1));
  robot_.get_ai("EXTRA", 0);
  robot_.set_ao("EXTRA", 0, 0.0);
  for (unsigned int i = 0; i < 100; ++i) {
    robot_.get_di("EXTRA", 0);
  }
}
TEST_F(RobotTest, TestClaw) {
  robot_.start_sys();
  robot_.set_claw(0, 0);
  auto claw_data = robot_.get_claw_data();
}
TEST_F(RobotTest, TestLed) {
  robot_.start_sys();
  robot_.set_led(0, 2, {1});
  robot_.set_voice(0, 1);
  robot_.set_fan(1);
}
TEST_F(RobotTest, TestSignal) {
  robot_.start_sys();
  robot_.set_signal(0, 20);
  std::this_thread::sleep_for(std::chrono::milliseconds(100));
  EXPECT_EQ(20, robot_.get_signal(0));
  robot_.set_signal(0, 30);
  std::this_thread::sleep_for(std::chrono::milliseconds(100));
  EXPECT_EQ(30, robot_.get_signal(0));
  robot_.add_signal(10, 50);
  robot_.set_signal(10, 60);
  std::this_thread::sleep_for(std::chrono::milliseconds(100));
  EXPECT_EQ(60, robot_.get_signal(10));
}

TEST_F(RobotTest, TestTaskReadSmoke) {
  const auto task_ids = robot_.get_task_list();
  for (const auto task_id : task_ids) {
    EXPECT_GE(task_id, 0U);
  }
  EXPECT_EQ("NONE", robot_.get_task_state());
  EXPECT_EQ("NONE", robot_.get_task_state(0));
}

TEST_F(RobotTest, TestKinematicsReadSmoke) {
  const auto fk_resp =
      robot_.kinematics_forward({0.0, 0.0, 0.0, 0.0, 0.0, 0.0});
  EXPECT_TRUE(fk_resp.ok);
  EXPECT_EQ(fk_resp.pose.size(), 6U);
  const auto inverse_pose = robot_.pose_inverse(
      {{"x", 0.0}, {"y", 0.0}, {"z", 0.0}, {"rx", 0.0}, {"ry", 0.0},
       {"rz", 0.0}});
  EXPECT_EQ(inverse_pose.size(), 6U);
}

TEST_F(RobotTest, TestTcpAndPayloadSmoke) {
  EXPECT_NO_THROW(robot_.set_tcp({0.0, 0.0, 0.0, 0.0, 0.0, 0.0}));
  const auto tcp = robot_.get_tcp();
  EXPECT_EQ(tcp.size(), 6U);

  robot_.set_velocity_factor(100);
  EXPECT_EQ(robot_.get_velocity_factor(), 100);

  robot_.set_payload(0.0, {{"x", 0.0}, {"y", 0.0}, {"z", 0.0}});
  const auto payload = robot_.get_payload();
  EXPECT_DOUBLE_EQ(payload.at("mass"), 0.0);
}

TEST_F(RobotTest, TestRobotics) {
  robot_.start_sys();
  std::vector<double> joint_positions(6);
  joint_positions[0] = 0.0;
  joint_positions[1] = -60.0 / 180.0 * M_PI;
  joint_positions[2] = 80.0 / 180.0 * M_PI;
  joint_positions[3] = -10.0 / 180.0 * M_PI;
  joint_positions[4] = -60.0 / 180.0 * M_PI;
  joint_positions[5] = 0.0;
  auto kf_resp = robot_.kinematics_forward(joint_positions);
  EXPECT_TRUE(kf_resp.ok);
  auto ki_resp = robot_.kinematics_inverse(kf_resp.pose, joint_positions);
  EXPECT_TRUE(ki_resp.ok);
  kf_resp.pose = robot_.pose_times(kf_resp.pose, kf_resp.pose);
  kf_resp.pose = robot_.pose_inverse(kf_resp.pose);
}
TEST_F(RobotTest, TestNetworkConnection) {
  EXPECT_TRUE(robot_.is_network_connected());
}
}  // namespace lebai

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
