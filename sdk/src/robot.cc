#include <lebai-sdk/robot.hh>
#include <memory>
#include "robot_impl.hh"


namespace lebai {

Robot::Robot(std::string ip, bool simulator)
// :ip_(ip)
{
  impl_ = std::make_unique<RobotImpl>(ip, simulator);
}
Robot::~Robot() {}

void Robot::start_sys()
{
  impl_->startSys();
}

void Robot::stop_sys()
{
  impl_->stopSys();
}

// void Robot::movej(const std::vector<double> & p, double v, double a, double t, double r, bool relative, bool wait)
// {
//   impl_->movej(p, v, a, t, r, relative);
//   if(wait)
//   {
//     std::this_thread::sleep_for(std::chrono::milliseconds(10));
//     wait_move();
//   }
// }

void Robot::movej(const MoveRequest & req)
{
  impl_->movej(req);
}

void Robot::wait_move()
{
  impl_->wait_move();
}

}  // namespace l_master_sdk