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

#include <lebai/discovery.hh>

#include <exception>
#include <future>
#include <string>

namespace lebai {
class DiscoveryTest : public ::testing::Test {
 public:
  DiscoveryTest() {}
  void SetUp() {}

 protected:
};
TEST_F(DiscoveryTest, Basic) {
  lebai::zeroconf::Discovery discovery;
  auto controller_data = discovery.resolve();
  (void)controller_data;
}

TEST_F(DiscoveryTest, ConcurrentInstancesResolveWithoutSharedState) {
  lebai::zeroconf::Discovery first;
  lebai::zeroconf::Discovery second;

  const auto resolve_error = [](lebai::zeroconf::Discovery& discovery) {
    try {
      discovery.resolve();
      return std::string{};
    } catch (const std::exception& error) {
      return std::string(error.what());
    } catch (...) {
      return std::string("non-standard exception");
    }
  };

  auto first_result =
      std::async(std::launch::async, [&] { return resolve_error(first); });
  auto second_result =
      std::async(std::launch::async, [&] { return resolve_error(second); });

  const auto first_error = first_result.get();
  const auto second_error = second_result.get();
  EXPECT_TRUE(first_error.empty()) << first_error;
  EXPECT_TRUE(second_error.empty()) << second_error;
}
}  // namespace lebai

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
