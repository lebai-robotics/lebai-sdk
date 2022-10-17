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


#include <lebai/discovery.hh>
#include <mdns.h>
#include <map>
#include <string>
namespace lebai
{
  namespace zeroconf
  {
    class Discovery::DiscoveryImpl
    {
    public:
      DiscoveryImpl();
      virtual ~DiscoveryImpl();
      std::vector<ControllerInfo> resolve();
    private:
      void send_mdns_query(mdns_query_t* query, size_t count);
      int open_client_sockets(int* sockets, int max_sockets, int port);
      const std::string PTR_ = "_lebai._tcp.local";
    };
  } // namespace zeroconf
}