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

#pragma once

#include <memory>
#include <string>
#include <vector>

namespace lebai
{
  namespace zeroconf
  {
  struct ControllerInfo
  {
    std::string str()
    {
      std::string str;
      str += "name:" + hostname + ",";
      str += "ip_address:" + ip_address + ",";
      str += "mac_address:" + mac_address + ",";
      str += "model:" + model + ",";
      str += "ds_version:" + ds_version + ",";
      str += "rc_version:" + rc_version + ",";
      str += "id:" + id;
      return str;
    }
    std::string hostname;
    std::string ip_address;
    std::string mac_address;
    std::string model;
    std::string ds_version;
    std::string rc_version;
    std::string id;
  };
    
  /**
   *  @brief 自动发现局域网内lebai机械臂。
   *
   */
  class Discovery
  {
  public:
    /**
     * @brief Discovery的内部实现.
     * @note 用户无需使用.
     */
    class DiscoveryImpl;
    /**
     * @brief 构造Discovery对象.
     * 
     */
    Discovery();
    /**
     * @brief 析构Discovery对象.
     * 
     */
    virtual ~Discovery();

    /**
     * @brief 获取局域网内lebai机械臂的IP地址.
     * 
     * @return std::vector<std::string> 所有的机械臂的IP列表
     */
    std::vector<ControllerInfo> resolve();

  protected:
    std::unique_ptr<DiscoveryImpl> impl_; /*!< 内部实现数据结构，用户无需关注。 */
  };
  }

} // namespace lebai
