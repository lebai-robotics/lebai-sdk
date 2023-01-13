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
  /**
   * \brief Lebai机械臂控制器的信息数据结构.
   * 
   */
  class ControllerInfo
  {
  public:
    /**
     * \brief 将控制器信息转换为字符串.
     * 
     * \return 字符串.
     */
    std::string str();

    std::string hostname; /*!< 主机名字. */
    std::string ip_address; /*!< IP地址. */
    std::string mac_address; /*!< MAC地址. */
    std::string model; /*!< 机械臂模型. */
    std::string ds_version; /*!< ds软件版本. */
    std::string rc_version; /*!< rc软件版本. */
    std::string id; /*!< 机械臂产品ID. */
  };
    
  /**
   *  @brief 自动发现局域网内lebai机械臂.
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
     * @brief 获取局域网内lebai机械臂的信息数据,可以用来自动发现局域网内的所有机械臂控制器.
     * 
     * @return std::vector<std::string> 所有的机械臂的信息数据.
     */
    std::vector<ControllerInfo> resolve();

  protected:
    std::unique_ptr<DiscoveryImpl> impl_; /*!< 内部实现数据结构，用户无需关注. */
  };
  }

} // namespace lebai
