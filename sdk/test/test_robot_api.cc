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

#include <string>
#include <type_traits>

#include "lebai/robot.hh"

using ForceModeSignature =
    void (lebai::l_master::Robot::*)(double, double, double, double);
using BoxTestSignature = int (lebai::l_master::Robot::*)(
    const std::string &, const std::string &);

static_assert(
    std::is_same_v<decltype(&lebai::l_master::Robot::set_force_mode_param),
                   ForceModeSignature>);
static_assert(std::is_same_v<decltype(&lebai::l_master::Robot::box_test),
                             BoxTestSignature>);

TEST(RobotApiTest, ApprovedSignaturesCompile) { SUCCEED(); }

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
