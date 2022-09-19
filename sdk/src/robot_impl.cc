#include "robot_impl.hh"
#include <rapidjson/rapidjson.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

namespace lebai
{
  Robot::RobotImpl::RobotImpl(const ::std::string &ip, bool simulator)
  {
    uint16_t port = simulator ? 3030 : 3031;
    json_rpc_connector_ = std::make_unique<JSONRpcConnector>(ip, port);
  }
  Robot::RobotImpl::~RobotImpl() {}

  int Robot::RobotImpl::startSys()
  {
    // Empty
    MoveParam req;
    json_rpc_connector_->CallRpc(req.ToJSONRpcReqString("start_sys",jsonrpc_id_++), nullptr);
  }

  int Robot::RobotImpl::stopSys()
  {
    // Empty
    MoveParam req;
    json_rpc_connector_->CallRpc(req.ToJSONRpcReqString("stop_sys",jsonrpc_id_++), nullptr);
  }

//   int Robot::RobotImpl::movej(const std::vector<double> & p, double v, double a, double t, double r, bool relative)
//   {
//     rapidjson::Document d;
//     rapidjson::Value req;
//     req.SetObject();
//     rapidjson::Value resp;
//     // {"pose":{"joint":{"base":{"kind":"CURRENT","joints":{"joint":[1,2,3,4,5,6]}}}},"param":{"v":0.1,"a":0.5}}
//     rapidjson::Value pose;
//     pose.SetObject();
//     rapidjson::Value joint;
//     joint.SetObject();
//     rapidjson::Value delta;
//     delta.SetObject();
//     rapidjson::Value joints;
//     joints.SetObject();    
//     rapidjson::Value param;
//     param.SetObject();
   
//     if(relative)
//     {
//       rapidjson::Value base;
//       base.SetObject();
//       rapidjson::Value kind;
//       kind.SetString("LAST");
//       base.AddMember("kind", kind, d.GetAllocator());
//       joint.AddMember("base", base, d.GetAllocator());
//       // delta.AddMember("kind", kind, d.GetAllocator());
//     }
    
    
//     {
//       rapidjson::Value joint;
//       joint.SetArray();
//       for(auto && jp : p)
//       {
//         joint.PushBack(jp, d.GetAllocator());
//       }
//       delta.AddMember("joint", joint, d.GetAllocator());      
//     }
// // {"pose":{"joint":{"delta":{"joint":[-1,-1,1,-2,1,2]}}},"param":{"a": 0.1,"v":0.1}}
// // {"pose":{"joint":{"base":{"kind":"LAST"},"delta":{"joint":[1,2,3,4,5,6]}}},"param":{"v":0.1,"a":0.5}}
// // {"pose":{"joint":{"base":{"kind":"LAST"},"delta":{"joint":[1.0,-0.5,0.2,0.0,0.0,2.0]}}},"param":{"v":0.2,"a":0.5}}
// //pose
// // {"joint":{"base":{"kind":"LAST"},"delta":{"joint":[1.0,-0.5,0.2,0.0,0.0,2.0]}}
// // {"joint":{"base":{"kind":0,"joints":{"joints":[]}},"delta":{"joints":[1.0,2.0,3.0]}}}
//     joint.AddMember("delta", delta, d.GetAllocator());
//     pose.AddMember("joint", joint, d.GetAllocator());
//     req.AddMember("pose", pose, d.GetAllocator());

//     param.AddMember("v", v, d.GetAllocator());
//     if(a > 0)
//     {
//       param.AddMember("a", a, d.GetAllocator());
//     }

//     if(t > 1e-6)
//     {
//       param.AddMember("t", t, d.GetAllocator());
//     }
//     if(r > 1e-6)
//     {
//       param.AddMember("r", r, d.GetAllocator());
//     }   

//     req.AddMember("param", param, d.GetAllocator());
//     rapidjson::StringBuffer buffer;
//     rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
//     req.Accept(writer);
//     std::string req_str = buffer.GetString();
//     json_rpc_connector_->Call("movej", req, resp);
//   }

  int Robot::RobotImpl::movej(const MoveRequest & req)
  {    
    std::string resp;  
    // std::string temp =
    // "{\"param\":{\"a\":0.1,\"v\":0.1},\"pose\":{\"joint\":{\"delta\":{\"joint\":[-1,0,0,0,0,0]}}}}";  
    // {"param":{"a": 0.1,"v":0.1},"pose":{"joint":{"delta":{"joint":[-1,-1,1,-2,1,2]}}}}
    // {"param":{"v":0.1},"pose":{"joint":{"delta":{"joint":[1.0,0.0,0.0,0.0,0.0,0.0]}}}}
    // json_rpc_connector_->CallString("movej", req.ToJSONString(), &resp);
    json_rpc_connector_->CallRpc(req.ToJSONRpcReqString("movej",jsonrpc_id_++), &resp);
  }

  int Robot::RobotImpl::wait_move()
  {
    rapidjson::Value req;
    rapidjson::Value resp;
    bool keep_waiting = true;
    while(keep_waiting)
    {
      json_rpc_connector_->Call("get_robot_data", req, resp);
      auto iter = resp.FindMember("robot_state");
      if(iter == resp.MemberEnd())
      {
        return -1;
      }
      auto & robot_state = iter->value;
      if(std::string(robot_state.GetString()) == std::string("Paused") || std::string(robot_state.GetString()) == std::string("Moving"))
      {
        keep_waiting = true;
      }
      else
      {
        keep_waiting = false;
      }
    }    
  }

}