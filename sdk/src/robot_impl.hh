#pragma once

#include <lebai-sdk/robot.hh>
#include <lebai-sdk/motion.hh>
#include "jsonrpc_connector.hh"

namespace lebai
{
class Robot::RobotImpl
{
public:
  RobotImpl(const::std::string &ip, bool simulator);
  virtual ~RobotImpl();
  int startSys();
  int stopSys();
  // int movej(const std::vector<double> & p, double v, double a, double t, double r, bool relative);
  int movej(const MoveRequest & req);
  // GetTcpForce
  int wait_move();

  protected:
  std::unique_ptr<JSONRpcConnector> json_rpc_connector_;
  int jsonrpc_id_ = 0;
    // WebSocketEndPoint endpoint_;
};

};