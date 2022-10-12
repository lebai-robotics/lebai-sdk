/**
 * Copyright 2002 lebai.ltd
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

#include "discovery_impl.hh"
#include <exception>
#include <zeroconf.hpp>
#include <iostream>
#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif

namespace lebai
{
  namespace zeroconf {
  Discovery::DiscoveryImpl::DiscoveryImpl()
  {

  }
  Discovery::DiscoveryImpl::~DiscoveryImpl() {}
  std::vector<std::string> Discovery::DiscoveryImpl::resolve()
  {
    std::vector<Zeroconf::mdns_responce> result;
    bool st = Zeroconf::Resolve("_lebai._tcp.local", 5, &result);
    // std::cout << "st "<<st << std::endl;
    // std::cout << "result size "<<result.size() << std::endl;    
    if(!st)
    {
      return std::vector<std::string>();
    }
    std::vector<std::string> ret;
    for (size_t i = 0; i < result.size(); i++)
    {
      auto& item = result[i];
      char buffer[INET6_ADDRSTRLEN + 1] = {0};
      switch(item.peer.ss_family) {
        case AF_INET:
          inet_ntop(AF_INET, &(((struct sockaddr_in *)(&item.peer))->sin_addr),
                  buffer, INET6_ADDRSTRLEN);
          ret.push_back(std::string(buffer));
          break;
        case AF_INET6:
          inet_ntop(AF_INET6, &(((struct sockaddr_in6 *)(&item.peer))->sin6_addr),
                  buffer, INET6_ADDRSTRLEN);
          ret.push_back(std::string(buffer));
          break;
        default:
          strncpy(buffer, "Unknown AF", INET6_ADDRSTRLEN);
          break;
      }     
    }
    return ret;
  }
  }
}