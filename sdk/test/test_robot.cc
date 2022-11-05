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

namespace lebai
{
  class RobotTest : public ::testing::Test
  {
  public:
    RobotTest()
    :robot_(TEST_L_MASTER_IP, true)
    {
      // std::cout<<"TEST_L_MASTER_IP "<<TEST_L_MASTER_IP<<"\n";
    }
    void SetUp()
    {
    }

  protected:
    l_master::Robot robot_;
  };
  TEST_F(RobotTest, TestStartStop)
  {
    robot_.stop_sys();
    std::this_thread::sleep_for(std::chrono::seconds(3));
    robot_.start_sys();
    std::this_thread::sleep_for(std::chrono::seconds(1));
    EXPECT_EQ(5, robot_.get_robot_mode());
    robot_.stop_sys();
    std::this_thread::sleep_for(std::chrono::milliseconds(20));
    EXPECT_EQ(10, robot_.get_robot_mode());
    std::this_thread::sleep_for(std::chrono::seconds(3));
    EXPECT_EQ(12, robot_.get_robot_mode());
    robot_.estop();
    std::this_thread::sleep_for(std::chrono::seconds(1));
    EXPECT_EQ(1, robot_.get_robot_mode());
    robot_.start_sys();
    std::this_thread::sleep_for(std::chrono::seconds(1));
    EXPECT_EQ(5, robot_.get_robot_mode());
    robot_.teach_mode();
    std::this_thread::sleep_for(std::chrono::seconds(1));
    EXPECT_EQ(11, robot_.get_robot_mode());
    robot_.end_teach_mode();
    std::this_thread::sleep_for(std::chrono::seconds(1));
    EXPECT_EQ(5, robot_.get_robot_mode());
  }
  TEST_F(RobotTest, TestMove)
  {
    robot_.start_sys();
    std::this_thread::sleep_for(std::chrono::seconds(1));
    ASSERT_EQ(5, robot_.get_robot_mode());
    {    
      std::map<std::string, double> joint_positions;
      joint_positions["j1"] = 0.0;
      joint_positions["j2"] = 0.0;
      joint_positions["j3"] = 0.0;
      joint_positions["j4"] = 0.0;
      joint_positions["j5"] = 0.0;
      joint_positions["j6"] = 0.0;
      robot_.movej(joint_positions, 1.0, 0.5, 0.0, 0.0);
      joint_positions["j1"] = 0.0;
      joint_positions["j2"] = -60.0 / 180.0 * M_PI;
      joint_positions["j3"] = 80.0 / 180.0 * M_PI;
      joint_positions["j4"] = -10.0 / 180.0 * M_PI;
      joint_positions["j5"] = -60.0 / 180.0 * M_PI;
      joint_positions["j6"] = 0.0;
      robot_.movej(joint_positions, 1.0, 0.5, 0.0, 0.0);
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
      EXPECT_EQ(7, robot_.get_robot_mode());
      robot_.wait_move();
      auto jp = robot_.get_target_joint_positions();
      ASSERT_TRUE(jp.find("j1") != jp.end());
      ASSERT_TRUE(jp.find("j2") != jp.end());
      ASSERT_TRUE(jp.find("j3") != jp.end());
      ASSERT_TRUE(jp.find("j4") != jp.end());
      ASSERT_TRUE(jp.find("j5") != jp.end());
      ASSERT_TRUE(jp.find("j6") != jp.end());
      EXPECT_NEAR(jp["j1"], joint_positions["j1"], 1e-3);
      EXPECT_NEAR(jp["j2"], joint_positions["j2"], 1e-3);
      EXPECT_NEAR(jp["j3"], joint_positions["j3"], 1e-3);
      EXPECT_NEAR(jp["j4"], joint_positions["j4"], 1e-3);
      EXPECT_NEAR(jp["j5"], joint_positions["j5"], 1e-3);
      EXPECT_NEAR(jp["j6"], joint_positions["j6"], 1e-3);
      std::this_thread::sleep_for(std::chrono::milliseconds(200));
      EXPECT_EQ(5, robot_.get_robot_mode());
      robot_.movej({-0.296,-0.295,0.285,60.0 / 180.0 * M_PI,-5.0 / 180.0 * M_PI,81.0 / 180.0 * M_PI}, 3.0, 1.0, 0.0, 0.0);
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
      EXPECT_EQ(7, robot_.get_robot_mode());
      robot_.wait_move();
      auto tcp = robot_.get_target_tcp_pose();
      EXPECT_NEAR(tcp[0], -0.296, 1e-3);
      EXPECT_NEAR(tcp[1], -0.295, 1e-3);
      EXPECT_NEAR(tcp[2], 0.285, 1e-3);
      EXPECT_NEAR(tcp[3], 60.0 / 180.0 * M_PI, 1e-3);
      EXPECT_NEAR(tcp[4], -5.0 / 180.0 * M_PI, 1e-3);
      EXPECT_NEAR(tcp[5], 81.0 / 180.0 * M_PI, 1e-3);
      joint_positions["j1"] = 0.0;
      joint_positions["j2"] = -60.0 / 180.0 * M_PI;
      joint_positions["j3"] = 80.0 / 180.0 * M_PI;
      joint_positions["j4"] = -10.0 / 180.0 * M_PI;
      joint_positions["j5"] = -60.0 / 180.0 * M_PI;
      joint_positions["j6"] = 0.0;
      robot_.movel(joint_positions, 0.3, 1.0, 0.0, 0.0);
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
      EXPECT_EQ(7, robot_.get_robot_mode());
      robot_.wait_move();
      EXPECT_NEAR(jp["j1"], joint_positions["j1"], 1e-3);
      EXPECT_NEAR(jp["j2"], joint_positions["j2"], 1e-3);
      EXPECT_NEAR(jp["j3"], joint_positions["j3"], 1e-3);
      EXPECT_NEAR(jp["j4"], joint_positions["j4"], 1e-3);
      EXPECT_NEAR(jp["j5"], joint_positions["j5"], 1e-3);
      EXPECT_NEAR(jp["j6"], joint_positions["j6"], 1e-3);
      robot_.movel({-0.296,-0.295,0.285,60.0 / 180.0 * M_PI,-5.0 / 180.0 * M_PI,81.0 / 180.0 * M_PI}, 1.0, 0.5, 0.0, 0.0);
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
      EXPECT_EQ(7, robot_.get_robot_mode());
      robot_.wait_move();      
      tcp = robot_.get_target_tcp_pose();
      EXPECT_NEAR(tcp[0], -0.296, 1e-3);
      EXPECT_NEAR(tcp[1], -0.295, 1e-3);
      EXPECT_NEAR(tcp[2], 0.285, 1e-3);
      EXPECT_NEAR(tcp[3], 60.0 / 180.0 * M_PI, 1e-3);
      EXPECT_NEAR(tcp[4], -5.0 / 180.0 * M_PI, 1e-3);
      EXPECT_NEAR(tcp[5], 81.0 / 180.0 * M_PI, 1e-3);
      std::this_thread::sleep_for(std::chrono::milliseconds(200));
      EXPECT_EQ(5, robot_.get_robot_mode());
    }    
  }
  TEST_F(RobotTest, TestStatus)
  {
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
  TEST_F(RobotTest, TestCabinetIO)
  {
    robot_.start_sys();
    robot_.set_do(0, false);
    robot_.set_do(0, static_cast<unsigned int>(1));
    robot_.get_di(0);
    robot_.set_ao(0, 0.2);
    robot_.get_ai(0);
  }
  TEST_F(RobotTest, TestFlangeIO)
  {
    robot_.start_sys();
    robot_.get_flange_di(0);
    robot_.set_flange_do(0, true);
    robot_.set_flange_do(0, static_cast<unsigned int>(1));
  }
  TEST_F(RobotTest, TestExtraIO)
  {
    robot_.start_sys();
    robot_.get_extra_di(0);
    robot_.set_extra_do(0, true);
    robot_.set_extra_do(0, static_cast<unsigned int>(1));
    robot_.get_extra_ai(0);
    robot_.set_extra_ao(0, 0.0);    
  }
  TEST_F(RobotTest, TestClaw)
  {
    robot_.start_sys();
    robot_.set_claw(0,0);
    auto claw_data = robot_.get_claw();
  }
  TEST_F(RobotTest, TestLed)
  {
    robot_.start_sys();
    robot_.set_led(0,2,{1});
    robot_.set_voice(0,1);
    robot_.set_fan(1);
  }
  TEST_F(RobotTest, TestSignal)
  {
    robot_.start_sys();
    robot_.set_signal(0,20);
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    EXPECT_EQ(20, robot_.get_signal(0));
    robot_.set_signal(0,30);
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    EXPECT_EQ(30, robot_.get_signal(0));
    robot_.add_signal(10,50);
    robot_.set_signal(10,60);
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    EXPECT_EQ(60, robot_.get_signal(10));
  }
  TEST_F(RobotTest, TestRobotics)
  {
    robot_.start_sys();
    std::map<std::string, double> joint_positions;
    joint_positions["j1"] = 0.0;
    joint_positions["j2"] = -60.0 / 180.0 * M_PI;
    joint_positions["j3"] = 80.0 / 180.0 * M_PI;
    joint_positions["j4"] = -10.0 / 180.0 * M_PI;
    joint_positions["j5"] = -60.0 / 180.0 * M_PI;
    joint_positions["j6"] = 0.0;    
    auto kf_resp = robot_.kinematics_forward(joint_positions);
    EXPECT_TRUE(kf_resp.ok);
    auto ki_resp = robot_.kinematics_inverse(kf_resp.pose, joint_positions);
    EXPECT_TRUE(ki_resp.ok);
    kf_resp.pose = robot_.pose_times(kf_resp.pose, kf_resp.pose);
    kf_resp.pose = robot_.pose_inverse(kf_resp.pose);
  }  
}

int main(int argc, char **argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
