#pragma once

// #include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <lebai-sdk/motion.hh>

namespace lebai
{

  /**
   *  @brief
   *
   */
  class Robot
  {
  public:
    class RobotImpl;
    explicit Robot(std::string ip, bool simulator = false);
    virtual ~Robot();
    void start_sys();
    void stop_sys();
    // void movej(const std::vector<double> & p, double v, double a = 1.0, double t = 0.0, double r = 0.0, bool relative = false, bool wait = false);
    void wait_move();
    void movej(const MoveRequest & req);
    // void powerdown();

  protected:
    std::unique_ptr<RobotImpl> impl_;
  };

} // namespace l_master_sdk
