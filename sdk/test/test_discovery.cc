/**
 * Copyright 2002 lebai.ltd
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
#include <lebai/discovery.hh>

namespace lebai
{
  class DiscoveryTest : public ::testing::Test
  {
  public:
    DiscoveryTest()
    {
    }
    void SetUp()
    {
    }

  protected:
  };
  TEST_F(DiscoveryTest, Basic)
  {
    lebai::zeroconf::Discovery discovery;
    auto all_ip_list = discovery.resolve();
    for (auto& ip : all_ip_list)
    {
      std::cout << "ip: " << ip << std::endl;
    }
  }
}

int main(int argc, char **argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

// #include <zeroconf.hpp>
// #include <iostream>
// #include <string>
// #include <vector>

// void PrintLog(Zeroconf::LogLevel level, const std::string& message)
// {
//     switch (level)
//     {
//         case Zeroconf::LogLevel::Error:
//             std::cout << "E: " << message << std::endl;
//             break;
//         case Zeroconf::LogLevel::Warning:
//             std::cout << "W: " << message << std::endl;
//             break;
//     }
// }

// int main(int argc, char **argv)
// {

//   // std::string MdnsQuery = ;
//   // std::cout << "Query: " << MdnsQuery << std::endl;

//   // Zeroconf::SetLogCallback(PrintLog);

//   std::vector<Zeroconf::mdns_responce> result;
//   bool st = Zeroconf::Resolve("_coap._udp.local", /*scanTime*/ 3, &result);
//   std::cout << "st "<<st << std::endl;
//   std::cout << "result size "<<result.size() << std::endl;
//   return 0;
// }