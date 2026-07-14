/**
 * Copyright 2026 lebai.ltd
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

#include <stdexcept>
#include <string>
#include <type_traits>
#include <vector>

#include "lebai/robot.hh"

using ForceModeSignature = void (lebai::l_master::Robot::*)(double, double,
                                                            double, double);
using BoxTestSignature = int (lebai::l_master::Robot::*)(const std::string&,
                                                         const std::string&);

static_assert(
    std::is_same_v<decltype(&lebai::l_master::Robot::set_force_mode_param),
                   ForceModeSignature>);
static_assert(std::is_same_v<decltype(&lebai::l_master::Robot::box_test),
                             BoxTestSignature>);

namespace {

void expect_invalid_pvat_lengths(lebai::l_master::Robot& robot,
                                 const std::vector<double>& p,
                                 const std::vector<double>& v,
                                 const std::vector<double>& a,
                                 const char* mismatch) {
  SCOPED_TRACE(mismatch);
  try {
    robot.move_pvat(p, v, a, 0.1);
  } catch (const std::invalid_argument& exception) {
    const std::string message = exception.what();
    EXPECT_NE(message.find("p, v, and a"), std::string::npos) << message;
    return;
  } catch (const std::exception& exception) {
    FAIL() << "expected std::invalid_argument, got: " << exception.what();
  } catch (...) {
    FAIL() << "expected std::invalid_argument, got a non-standard exception";
  }

  FAIL() << "expected std::invalid_argument";
}

}  // namespace

TEST(RobotApiTest, ApprovedSignaturesCompile) { SUCCEED(); }

TEST(RobotApiTest, MovePvatRejectsMismatchedVectorLengths) {
  lebai::l_master::Robot robot("127.0.0.1", true);
  const std::vector<double> p(2, 0.0);

  expect_invalid_pvat_lengths(robot, p, std::vector<double>(1, 0.0),
                              std::vector<double>(2, 0.0), "v shorter than p");
  expect_invalid_pvat_lengths(robot, p, std::vector<double>(3, 0.0),
                              std::vector<double>(2, 0.0), "v longer than p");
  expect_invalid_pvat_lengths(robot, p, std::vector<double>(2, 0.0),
                              std::vector<double>(1, 0.0), "a shorter than p");
  expect_invalid_pvat_lengths(robot, p, std::vector<double>(2, 0.0),
                              std::vector<double>(3, 0.0), "a longer than p");
}

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
