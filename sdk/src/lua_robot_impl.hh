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


#include <lebai/lua_robot.hh>
#include <asio.hpp>
#include <iostream>

namespace lebai
{
  namespace l_master
  {
    class LuaRobot::LuaRobotImpl
    {
    public:
      LuaRobotImpl(const::std::string &ip);
      virtual ~LuaRobotImpl();
      int connect(const std::string & ip);
      void send(const std::string & lua_code);
      std::string call(const std::string & lua_code);
      
    protected:
      void doConnect(asio::ip::tcp::resolver::iterator endpoint_iterator)
      {
        asio::async_connect(*socket_, endpoint_iterator,
            [this](std::error_code ec, asio::ip::tcp::resolver::iterator)
            {
              if (!ec)
              {
                std::cerr<<"Connect ok.\n";
              }
              else
              {
                std::cerr<<"Connect failed.\n";
                std::cerr<<"error code "<<ec.message()<<"\n";
              }
            });
      }    
      double timeout_ = 1.0;
      const uint16_t port_ = 5180;
      std::unique_ptr<asio::io_service> io_service_;
      std::unique_ptr<asio::ip::tcp::socket> socket_;
      // asio::deadline_timer deadline_;
      bool connnected_ = false;
    };
  } // namespace l_master
}