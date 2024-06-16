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

#pragma once

#include <memory>
#include <string>
#include <vector>

namespace lebai {
namespace l_master {

/**
 *  @brief 机械臂的Lua接口，通过本对象向机械臂发送lua指令.
 *
 */
class LuaRobot {
 public:
  /**
   * @brief 内部实现.
   * @note 用户无需关注.
   */
  class LuaRobotImpl;
  /**
   * @brief 构造LuaRobot对象.可以通过该对象向机械臂发送lua指令，并且获取返回值。
   *
   * 可以参考[Lua指令说明](http://help.lebai.ltd/dev/lua.html)了解所有和机械臂相关的lua指令
   *
   *
   * @param ip: 机械臂IP地址.
   */
  explicit LuaRobot(std::string ip);
  /**
   * @brief 析构LuaRobot对象.
   *
   */
  virtual ~LuaRobot();
  /**
   * 示例代码:
   *
   *     lua_robot.send("start_sys()");
   *
   * @brief 向机械臂发送lua指令.
   * @note 该函数只会发送lua指令给机械臂，不会读取机械臂的返回数据.
   *
   * @param lua_code: lua指令.
   */
  void send(const std::string& lua_code);
  /**
   * 示例代码:
   *
   *     std::vector<std::string> codes;
   *     codes.push_back("start_sys()");
   *     codes.push_back("movej({j1 = 1.0, j2 = 0, j3 = 0, j4 = 0, j5 = 0, j6 =
   * 0}, 1.2, 0.2, 0, 0)"); codes.push_back("movej({j1 = 0.0, j2 = 0, j3 = 0, j4
   * = 0, j5 = 0, j6 = 0}, 1.2, 0.2, 0, 0)"); codes.push_back("sync()");
   *     codes.push_back("stop_sys()");
   *     lua_robot.send(codes);
   *
   * @brief 向机械臂发送多行lua指令.这些指令会按照顺序执行
   * @note 该函数只会发送lua指令给机械臂，不会读取机械臂的返回数据.
   *
   * @param lua_codes: 多个（行）lua指令.
   */
  void send(const std::vector<std::string>& lua_codes);
  /**
   * 示例代码:
   *
   *     auto ret = lua_robot.call("get_robot_mode()");
   *
   * @brief 向机械臂发送lua指令，并且获取机械臂的返回数据.
   * @note 该函数会发送lua指令给机械臂，并且读取机械臂的返回数据.
   *
   * @param lua_code: lua指令.
   * @return std::string: 机械臂返回的数据.
   */
  std::string call(const std::string& lua_code);
  /** @}*/
 protected:
  std::unique_ptr<LuaRobotImpl> impl_; /*!< 内部实现数据结构，用户无需关注. */
};

}  // namespace l_master

}  // namespace lebai
