#include <iostream>
#include <chrono>
#include <thread>
#include <lebai-sdk/robot.hh>

int main(int /*argc*/, char ** /*argv*/)
{
  // Create robot instance
  lebai::Robot robot("172.17.0.3", true);
  std::this_thread::sleep_for(std::chrono::seconds(1));
  robot.start_sys();
  std::this_thread::sleep_for(std::chrono::seconds(2));
  robot.stop_sys();
  std::this_thread::sleep_for(std::chrono::seconds(3));
  robot.start_sys();
  // Create move request
  lebai::MoveRequest move_req;
  // Set move request data
  move_req.mutable_pose().mutable_joint()->
  mutable_delta().set_joint({1.0, 0.0, 0.0, 0.0, 0.0, 0.0});  
  move_req.mutable_param().set_v(0.1);
  robot.movej(move_req);
  move_req.mutable_pose().mutable_joint()->
  mutable_delta().set_joint({-1.0, 0.0, 0.0, 0.0, 0.0, 0.0});  
  robot.movej(move_req);
  return 0;
}

// int main(int /*argc*/, char** /*argv*/) {
//   lebai::Product product;
//   product.name = "abc";
//   product.id = 123;
//   std::cout<<product.JSONBase::ToJSONString()<<"\n";
// }