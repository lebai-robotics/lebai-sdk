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

#include "discovery_impl.hh"
#include <iostream>
#include <vector>
#ifdef _WIN32
#include <winsock2.h>
#include <iphlpapi.h>
#define sleep(x) Sleep(x * 1000)
#else
#include <netdb.h>
#include <ifaddrs.h>
#include <net/if.h>
#endif
#include <exception>


namespace lebai
{
  namespace zeroconf
  {
    static char addrbuffer[64];
    static char entrybuffer[256];
    static char namebuffer[256];
    static char sendbuffer[1024];
    static mdns_record_txt_t txtbuffer[128];
    static struct sockaddr_in service_address_ipv4;
    static struct sockaddr_in6 service_address_ipv6;
    static std::vector<ControllerInfo> g_controller_info_array;
    ControllerInfo g_controller_info;

    static int has_ipv4;
    static int has_ipv6;
    static mdns_string_t
    ipv4_address_to_string(char* buffer, size_t capacity, const struct sockaddr_in* addr,
                          size_t addrlen) {
      char host[NI_MAXHOST] = {0};
      char service[NI_MAXSERV] = {0};
      int ret = getnameinfo((const struct sockaddr*)addr, (socklen_t)addrlen, host, NI_MAXHOST,
                            service, NI_MAXSERV, NI_NUMERICSERV | NI_NUMERICHOST);
      int len = 0;
      if (ret == 0) {
        if (addr->sin_port != 0)
          len = snprintf(buffer, capacity, "%s:%s", host, service);
        else
          len = snprintf(buffer, capacity, "%s", host);
      }
      if (len >= (int)capacity)
        len = (int)capacity - 1;
      mdns_string_t str;
      str.str = buffer;
      str.length = len;
      return str;
    }
    static mdns_string_t
    ipv6_address_to_string(char* buffer, size_t capacity, const struct sockaddr_in6* addr,
                          size_t addrlen) {
      char host[NI_MAXHOST] = {0};
      char service[NI_MAXSERV] = {0};
      int ret = getnameinfo((const struct sockaddr*)addr, (socklen_t)addrlen, host, NI_MAXHOST,
                            service, NI_MAXSERV, NI_NUMERICSERV | NI_NUMERICHOST);
      int len = 0;
      if (ret == 0) {
        if (addr->sin6_port != 0)
          len = snprintf(buffer, capacity, "[%s]:%s", host, service);
        else
          len = snprintf(buffer, capacity, "%s", host);
      }
      if (len >= (int)capacity)
        len = (int)capacity - 1;
      mdns_string_t str;
      str.str = buffer;
      str.length = len;
      return str;
    }    

    static mdns_string_t
    ip_address_to_string(char* buffer, size_t capacity, const struct sockaddr* addr, size_t addrlen) {
      if (addr->sa_family == AF_INET6)
        return ipv6_address_to_string(buffer, capacity, (const struct sockaddr_in6*)addr, addrlen);
      return ipv4_address_to_string(buffer, capacity, (const struct sockaddr_in*)addr, addrlen);
    }
    static int query_callback(int sock, const struct sockaddr* from, size_t addrlen, mdns_entry_type_t entry,
                  uint16_t query_id, uint16_t rtype, uint16_t rclass, uint32_t ttl, const void* data,
                  size_t size, size_t name_offset, size_t name_length, size_t record_offset,
                  size_t record_length, void* user_data) {
      (void)sizeof(sock);
      (void)sizeof(query_id);
      (void)sizeof(name_length);
      (void)sizeof(user_data);
      mdns_string_t fromaddrstr = ip_address_to_string(addrbuffer, sizeof(addrbuffer), from, addrlen);
      const char* entrytype = (entry == MDNS_ENTRYTYPE_ANSWER) ?
                                    "answer" :
                                    ((entry == MDNS_ENTRYTYPE_AUTHORITY) ? "authority" : "additional");
      mdns_string_t entrystr =
          mdns_string_extract(data, size, &name_offset, entrybuffer, sizeof(entrybuffer));
      if (rtype == MDNS_RECORDTYPE_PTR) {
        mdns_string_t namestr = mdns_record_parse_ptr(data, size, record_offset, record_length,
                                                      namebuffer, sizeof(namebuffer));
        // printf("%.*s : %s %.*s PTR %.*s rclass 0x%x ttl %u length %d\n",
        //       MDNS_STRING_FORMAT(fromaddrstr), entrytype, MDNS_STRING_FORMAT(entrystr),
        //       MDNS_STRING_FORMAT(namestr), rclass, ttl, (int)record_length);
        g_controller_info.hostname = std::string(namestr.str, namestr.length);
        g_controller_info.hostname = g_controller_info.hostname.substr(0, g_controller_info.hostname.find("."));
      } else if (rtype == MDNS_RECORDTYPE_SRV) {
        mdns_record_srv_t srv = mdns_record_parse_srv(data, size, record_offset, record_length,
                                                      namebuffer, sizeof(namebuffer));
        // printf("%.*s : %s %.*s SRV %.*s priority %d weight %d port %d\n",
        //       MDNS_STRING_FORMAT(fromaddrstr), entrytype, MDNS_STRING_FORMAT(entrystr),
        //       MDNS_STRING_FORMAT(srv.name), srv.priority, srv.weight, srv.port);
      } else if (rtype == MDNS_RECORDTYPE_A) {
        struct sockaddr_in addr;
        mdns_record_parse_a(data, size, record_offset, record_length, &addr);
        mdns_string_t addrstr =
            ipv4_address_to_string(namebuffer, sizeof(namebuffer), &addr, sizeof(addr));
        // printf("%.*s : %s %.*s A %.*s\n", MDNS_STRING_FORMAT(fromaddrstr), entrytype,
        //       MDNS_STRING_FORMAT(entrystr), MDNS_STRING_FORMAT(addrstr));
        g_controller_info.ip_address = std::string(addrstr.str, addrstr.length);
      } else if (rtype == MDNS_RECORDTYPE_AAAA) {
        struct sockaddr_in6 addr;
        mdns_record_parse_aaaa(data, size, record_offset, record_length, &addr);
        mdns_string_t addrstr =
            ipv6_address_to_string(namebuffer, sizeof(namebuffer), &addr, sizeof(addr));
        // printf("%.*s : %s %.*s AAAA %.*s\n", MDNS_STRING_FORMAT(fromaddrstr), entrytype,
        //       MDNS_STRING_FORMAT(entrystr), MDNS_STRING_FORMAT(addrstr));
      } else if (rtype == MDNS_RECORDTYPE_TXT) {
        size_t parsed = mdns_record_parse_txt(data, size, record_offset, record_length, txtbuffer,
                                              sizeof(txtbuffer) / sizeof(mdns_record_txt_t));
        for (size_t itxt = 0; itxt < parsed; ++itxt) {
          if (txtbuffer[itxt].value.length) {
            // printf("%.*s : %s %.*s TXT %.*s = %.*s\n", MDNS_STRING_FORMAT(fromaddrstr),
            //       entrytype, MDNS_STRING_FORMAT(entrystr),
            //       MDNS_STRING_FORMAT(txtbuffer[itxt].key),
            //       MDNS_STRING_FORMAT(txtbuffer[itxt].value));
            // controller_info.hostname = controller_info.hostname.substr(0, controller_info.hostname.find("."));
            std::string key = std::string(txtbuffer[itxt].key.str, txtbuffer[itxt].key.length);
            if(key == "model")
            {
              g_controller_info.model = std::string(txtbuffer[itxt].value.str,txtbuffer[itxt].value.length);
            }
            else if(key == "ds")
            {
              g_controller_info.ds_version = std::string(txtbuffer[itxt].value.str,txtbuffer[itxt].value.length);
            }
            else if(key == "rc")
            {
              g_controller_info.rc_version = std::string(txtbuffer[itxt].value.str,txtbuffer[itxt].value.length);              
            }
            else if(key == "id")
            {
              g_controller_info.id = std::string(txtbuffer[itxt].value.str,txtbuffer[itxt].value.length);              
            }
            else if(key == "mac")
            {
              g_controller_info.mac_address = std::string(txtbuffer[itxt].value.str,txtbuffer[itxt].value.length);
            }
          } else {
            // printf("%.*s : %s %.*s TXT %.*s\n", MDNS_STRING_FORMAT(fromaddrstr), entrytype,
            //       MDNS_STRING_FORMAT(entrystr), MDNS_STRING_FORMAT(txtbuffer[itxt].key));
          }
        }
      } else {
        // printf("%.*s : %s %.*s type %u rclass 0x%x ttl %u length %d\n",
        //       MDNS_STRING_FORMAT(fromaddrstr), entrytype, MDNS_STRING_FORMAT(entrystr), rtype,
        //       rclass, ttl, (int)record_length);
      }
      if(!g_controller_info.hostname.empty() && 
        !g_controller_info.model.empty() && 
        !g_controller_info.ds_version.empty() && 
        !g_controller_info.rc_version.empty() && 
        !g_controller_info.id.empty() && 
        !g_controller_info.mac_address.empty() && 
        !g_controller_info.ip_address.empty())
      {
        g_controller_info_array.push_back(g_controller_info);        
        g_controller_info.hostname.clear();
        g_controller_info.model.clear();
        g_controller_info.ds_version.clear();
        g_controller_info.rc_version.clear();
        g_controller_info.id.clear();
        g_controller_info.mac_address.clear();
        g_controller_info.ip_address.clear();            
      }
      return 0;
    }
    Discovery::DiscoveryImpl::DiscoveryImpl()
    {
    }
    Discovery::DiscoveryImpl::~DiscoveryImpl() {}
    std::vector<ControllerInfo> Discovery::DiscoveryImpl::resolve()
    {
      mdns_query_t query[16];
      size_t query_count = 0;
      query[query_count].name = PTR_.c_str();
      query[query_count].type = MDNS_RECORDTYPE_PTR;
      query[query_count].length = strlen(query[query_count].name);      
      query_count++;
      send_mdns_query(query, query_count);
      // for(auto && controller_info : g_controller_info_array)
      // {
      //   std::cout<<"================================\n";
      //   std::cout<<"controller_info.hostname "<<controller_info.hostname<<"\n";
      //   std::cout<<"controller_info.model "<<controller_info.model<<"\n";
      //   std::cout<<"controller_info.ds_version "<<controller_info.ds_version<<"\n";
      //   std::cout<<"controller_info.rc_version "<<controller_info.rc_version<<"\n";
      //   std::cout<<"controller_info.id "<<controller_info.id<<"\n";
      //   std::cout<<"controller_info.mac_address "<<controller_info.mac_address<<"\n";
      //   std::cout<<"controller_info.ip_address "<<controller_info.ip_address<<"\n";
      // }
      return g_controller_info_array;
    }

    void Discovery::DiscoveryImpl::send_mdns_query(mdns_query_t* query, size_t count)
    {
      int sockets[32];
      int query_id[32];
      int num_sockets = open_client_sockets(sockets, sizeof(sockets) / sizeof(sockets[0]), 0);
      if (num_sockets <= 0)
      {
        printf("Failed to open any client sockets\n");
        return;
      }
      // printf("Opened %d socket%s for mDNS query\n", num_sockets, num_sockets ? "s" : "");

      size_t capacity = 2048;
      void *buffer = malloc(capacity);
      void *user_data = 0;

      // printf("Sending mDNS query");
      for (size_t iq = 0; iq < count; ++iq)
      {
        const char *record_name = "PTR";
        if (query[iq].type == MDNS_RECORDTYPE_SRV)
          record_name = "SRV";
        else if (query[iq].type == MDNS_RECORDTYPE_A)
          record_name = "A";
        else if (query[iq].type == MDNS_RECORDTYPE_AAAA)
          record_name = "AAAA";
        else
          query[iq].type = MDNS_RECORDTYPE_PTR;
        // printf(" : %s %s", query[iq].name, record_name);
      }
      // printf("\n");
      for (int isock = 0; isock < num_sockets; ++isock)
      {
        query_id[isock] =
            mdns_multiquery_send(sockets[isock], query, count, buffer, capacity, 0);
        if (query_id[isock] < 0)
          printf("Failed to send mDNS query: %s\n", strerror(errno));
      }

      // This is a simple implementation that loops for 5 seconds or as long as we get replies
      int res;
      // printf("Reading mDNS query replies\n");
      int records = 0;
      g_controller_info_array.clear();
      do
      {
        struct timeval timeout;
        timeout.tv_sec = 2;
        timeout.tv_usec = 0;

        int nfds = 0;
        fd_set readfs;
        FD_ZERO(&readfs);
        for (int isock = 0; isock < num_sockets; ++isock)
        {
          if (sockets[isock] >= nfds)
            nfds = sockets[isock] + 1;
          FD_SET(sockets[isock], &readfs);
        }

        res = select(nfds, &readfs, 0, 0, &timeout);
        if (res > 0)
        { 
          g_controller_info.hostname.clear();
          g_controller_info.model.clear();
          g_controller_info.ds_version.clear();
          g_controller_info.rc_version.clear();
          g_controller_info.id.clear();
          g_controller_info.mac_address.clear();
          g_controller_info.ip_address.clear();
          for (int isock = 0; isock < num_sockets; ++isock)
          {
            if (FD_ISSET(sockets[isock], &readfs))
            {
              int rec = mdns_query_recv(sockets[isock], buffer, capacity, query_callback,
                                        user_data, query_id[isock]);
              if (rec > 0)
                records += rec;
            }
            FD_SET(sockets[isock], &readfs);
          }
        }
      } while (res > 0);

      // printf("Read %d records\n", records);

      free(buffer);

      for (int isock = 0; isock < num_sockets; ++isock)
        mdns_socket_close(sockets[isock]);
      // printf("Closed socket%s\n", num_sockets ? "s" : "");
      return;
    }

    int
    Discovery::DiscoveryImpl::open_client_sockets(int *sockets, int max_sockets, int port)
    {
      // When sending, each socket can only send to one network interface
      // Thus we need to open one socket for each interface and address family
      int num_sockets = 0;

#ifdef _WIN32

      IP_ADAPTER_ADDRESSES *adapter_address = 0;
      ULONG address_size = 8000;
      unsigned int ret;
      unsigned int num_retries = 4;
      do
      {
        adapter_address = (IP_ADAPTER_ADDRESSES *)malloc(address_size);
        ret = GetAdaptersAddresses(AF_UNSPEC, GAA_FLAG_SKIP_MULTICAST | GAA_FLAG_SKIP_ANYCAST, 0,
                                   adapter_address, &address_size);
        if (ret == ERROR_BUFFER_OVERFLOW)
        {
          free(adapter_address);
          adapter_address = 0;
          address_size *= 2;
        }
        else
        {
          break;
        }
      } while (num_retries-- > 0);

      if (!adapter_address || (ret != NO_ERROR))
      {
        free(adapter_address);
        printf("Failed to get network adapter addresses\n");
        return num_sockets;
      }

      int first_ipv4 = 1;
      int first_ipv6 = 1;
      for (PIP_ADAPTER_ADDRESSES adapter = adapter_address; adapter; adapter = adapter->Next)
      {
        if (adapter->TunnelType == TUNNEL_TYPE_TEREDO)
          continue;
        if (adapter->OperStatus != IfOperStatusUp)
          continue;

        for (IP_ADAPTER_UNICAST_ADDRESS *unicast = adapter->FirstUnicastAddress; unicast;
             unicast = unicast->Next)
        {
          if (unicast->Address.lpSockaddr->sa_family == AF_INET)
          {
            struct sockaddr_in *saddr = (struct sockaddr_in *)unicast->Address.lpSockaddr;
            if ((saddr->sin_addr.S_un.S_un_b.s_b1 != 127) ||
                (saddr->sin_addr.S_un.S_un_b.s_b2 != 0) ||
                (saddr->sin_addr.S_un.S_un_b.s_b3 != 0) ||
                (saddr->sin_addr.S_un.S_un_b.s_b4 != 1))
            {
              int log_addr = 0;
              if (first_ipv4)
              {
                service_address_ipv4 = *saddr;
                first_ipv4 = 0;
                log_addr = 1;
              }
              has_ipv4 = 1;
              if (num_sockets < max_sockets)
              {
                saddr->sin_port = htons((unsigned short)port);
                int sock = mdns_socket_open_ipv4(saddr);
                if (sock >= 0)
                {
                  sockets[num_sockets++] = sock;
                  log_addr = 1;
                }
                else
                {
                  log_addr = 0;
                }
              }
              if (log_addr)
              {
                char buffer[128];
                mdns_string_t addr = ipv4_address_to_string(buffer, sizeof(buffer), saddr,
                                                            sizeof(struct sockaddr_in));
                // printf("Local IPv4 address: %.*s\n", MDNS_STRING_FORMAT(addr));
              }
            }
          }
          else if (unicast->Address.lpSockaddr->sa_family == AF_INET6)
          {
            struct sockaddr_in6 *saddr = (struct sockaddr_in6 *)unicast->Address.lpSockaddr;
            // Ignore link-local addresses
            if (saddr->sin6_scope_id)
              continue;
            static const unsigned char localhost[] = {0, 0, 0, 0, 0, 0, 0, 0,
                                                      0, 0, 0, 0, 0, 0, 0, 1};
            static const unsigned char localhost_mapped[] = {0, 0, 0, 0, 0, 0, 0, 0,
                                                             0, 0, 0xff, 0xff, 0x7f, 0, 0, 1};
            if ((unicast->DadState == NldsPreferred) &&
                memcmp(saddr->sin6_addr.s6_addr, localhost, 16) &&
                memcmp(saddr->sin6_addr.s6_addr, localhost_mapped, 16))
            {
              int log_addr = 0;
              if (first_ipv6)
              {
                service_address_ipv6 = *saddr;
                first_ipv6 = 0;
                log_addr = 1;
              }
              has_ipv6 = 1;
              if (num_sockets < max_sockets)
              {
                saddr->sin6_port = htons((unsigned short)port);
                int sock = mdns_socket_open_ipv6(saddr);
                if (sock >= 0)
                {
                  sockets[num_sockets++] = sock;
                  log_addr = 1;
                }
                else
                {
                  log_addr = 0;
                }
              }
              if (log_addr)
              {
                char buffer[128];
                mdns_string_t addr = ipv6_address_to_string(buffer, sizeof(buffer), saddr,
                                                            sizeof(struct sockaddr_in6));
                // printf("Local IPv6 address: %.*s\n", MDNS_STRING_FORMAT(addr));
              }
            }
          }
        }
      }

      free(adapter_address);

#else

      struct ifaddrs *ifaddr = 0;
      struct ifaddrs *ifa = 0;

      if (getifaddrs(&ifaddr) < 0)
        printf("Unable to get interface addresses\n");

      int first_ipv4 = 1;
      int first_ipv6 = 1;
      for (ifa = ifaddr; ifa; ifa = ifa->ifa_next)
      {
        if (!ifa->ifa_addr)
          continue;
        if (!(ifa->ifa_flags & IFF_UP) || !(ifa->ifa_flags & IFF_MULTICAST))
          continue;
        if ((ifa->ifa_flags & IFF_LOOPBACK) || (ifa->ifa_flags & IFF_POINTOPOINT))
          continue;

        if (ifa->ifa_addr->sa_family == AF_INET)
        {
          struct sockaddr_in *saddr = (struct sockaddr_in *)ifa->ifa_addr;
          if (saddr->sin_addr.s_addr != htonl(INADDR_LOOPBACK))
          {
            int log_addr = 0;
            if (first_ipv4)
            {
              service_address_ipv4 = *saddr;
              first_ipv4 = 0;
              log_addr = 1;
            }
            has_ipv4 = 1;
            if (num_sockets < max_sockets)
            {
              saddr->sin_port = htons(port);
              int sock = mdns_socket_open_ipv4(saddr);
              if (sock >= 0)
              {
                sockets[num_sockets++] = sock;
                log_addr = 1;
              }
              else
              {
                log_addr = 0;
              }
            }
            if (log_addr)
            {
              char buffer[128];
              mdns_string_t addr = ipv4_address_to_string(buffer, sizeof(buffer), saddr,
                                                          sizeof(struct sockaddr_in));
              // printf("Local IPv4 address: %.*s\n", MDNS_STRING_FORMAT(addr));
            }
          }
        }
        else if (ifa->ifa_addr->sa_family == AF_INET6)
        {
          struct sockaddr_in6 *saddr = (struct sockaddr_in6 *)ifa->ifa_addr;
          // Ignore link-local addresses
          if (saddr->sin6_scope_id)
            continue;
          static const unsigned char localhost[] = {0, 0, 0, 0, 0, 0, 0, 0,
                                                    0, 0, 0, 0, 0, 0, 0, 1};
          static const unsigned char localhost_mapped[] = {0, 0, 0, 0, 0, 0, 0, 0,
                                                           0, 0, 0xff, 0xff, 0x7f, 0, 0, 1};
          if (memcmp(saddr->sin6_addr.s6_addr, localhost, 16) &&
              memcmp(saddr->sin6_addr.s6_addr, localhost_mapped, 16))
          {
            int log_addr = 0;
            if (first_ipv6)
            {
              service_address_ipv6 = *saddr;
              first_ipv6 = 0;
              log_addr = 1;
            }
            has_ipv6 = 1;
            if (num_sockets < max_sockets)
            {
              saddr->sin6_port = htons(port);
              int sock = mdns_socket_open_ipv6(saddr);
              if (sock >= 0)
              {
                sockets[num_sockets++] = sock;
                log_addr = 1;
              }
              else
              {
                log_addr = 0;
              }
            }
            if (log_addr)
            {
              char buffer[128];
              mdns_string_t addr = ipv6_address_to_string(buffer, sizeof(buffer), saddr,
                                                          sizeof(struct sockaddr_in6));
              // printf("Local IPv6 address: %.*s\n", MDNS_STRING_FORMAT(addr));
            }
          }
        }
      }

      freeifaddrs(ifaddr);

#endif

      return num_sockets;
    }
  }
}