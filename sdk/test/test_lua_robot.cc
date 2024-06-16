/**
 * Copyright 2022-2023 lebai.ltd
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
#include "lebai/lua_robot.hh"

namespace lebai {
class LuaRobotTest : public ::testing::Test {
 public:
  LuaRobotTest() : lua_robot_(TEST_L_MASTER_IP) {
    // std::cout<<"TEST_L_MASTER_IP "<<TEST_L_MASTER_IP<<"\n";
  }
  void SetUp() {}

 protected:
  l_master::LuaRobot lua_robot_;
};
TEST_F(LuaRobotTest, SendOneCase) {
  EXPECT_NO_THROW(lua_robot_.send("start_sys()"));
  EXPECT_NO_THROW(
      lua_robot_.send("movej({j1 = 0.0, j2 = 0, j3 = 0, j4 = 0, j5 = 0, j6 = "
                      "0}, 1.2, 0.2, 0, 0)"));
  EXPECT_NO_THROW(
      lua_robot_.send("movej({j1 = 1.0, j2 = 0, j3 = 0, j4 = 0, j5 = 0, j6 = "
                      "0}, 1.2, 0.2, 0, 0)"));
}
TEST_F(LuaRobotTest, SendLinesCase) {
  std::vector<std::string> codes;
  codes.push_back("start_sys()");
  codes.push_back(
      "movej({j1 = 1.0, j2 = 0, j3 = 0, j4 = 0, j5 = 0, j6 = 0}, 1.2, 0.2, 0, "
      "0)");
  codes.push_back(
      "movej({j1 = 0.0, j2 = 0, j3 = 0, j4 = 0, j5 = 0, j6 = 0}, 1.2, 0.2, 0, "
      "0)");
  codes.push_back("sync()");
  codes.push_back("stop_sys()");
  EXPECT_NO_THROW(lua_robot_.send(codes));
}
TEST_F(LuaRobotTest, CallCase) {
  EXPECT_NO_THROW(lua_robot_.send("start_sys()"));
  EXPECT_NO_THROW(
      lua_robot_.send("movej({j1 = 0.0, j2 = 0, j3 = 0, j4 = 0, j5 = 0, j6 = "
                      "0}, 1.2, 0.2, 0, 0)"));
  EXPECT_NO_THROW(
      lua_robot_.send("movej({j1 = 1.0, j2 = 0, j3 = 0, j4 = 0, j5 = 0, j6 = "
                      "0}, 1.2, 0.2, 0, 0)"));

  std::string jps;
  EXPECT_NO_THROW(jps = lua_robot_.call("get_target_joint_positions()"));
  EXPECT_TRUE(jps.size());
}
}  // namespace lebai

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
