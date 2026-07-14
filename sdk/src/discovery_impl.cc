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

#ifdef _WIN32
#include <winsock2.h>
#include <iphlpapi.h>
#define sleep(x) Sleep(x * 1000)
#else
#include <netdb.h>
#include <ifaddrs.h>
#include <net/if.h>
#endif

#include <cstdint>
#include <cstring>
#include <string>
#include <vector>

#include "discovery_accumulator.hh"
#include "discovery_impl.hh"

namespace lebai {
namespace zeroconf {
namespace {

mdns_string_t ipv4_address_to_string(char* buffer, size_t capacity,
                                     const struct sockaddr_in* addr,
                                     size_t addrlen) {
  char host[NI_MAXHOST] = {0};
  char service[NI_MAXSERV] = {0};
  int ret = getnameinfo((const struct sockaddr*)addr, (socklen_t)addrlen, host,
                        NI_MAXHOST, service, NI_MAXSERV,
                        NI_NUMERICSERV | NI_NUMERICHOST);
  int len = 0;
  if (ret == 0) {
    if (addr->sin_port != 0)
      len = snprintf(buffer, capacity, "%s:%s", host, service);
    else
      len = snprintf(buffer, capacity, "%s", host);
  }
  if (len >= (int)capacity) len = (int)capacity - 1;
  mdns_string_t str;
  str.str = buffer;
  str.length = len;
  return str;
}
mdns_string_t ipv6_address_to_string(char* buffer, size_t capacity,
                                     const struct sockaddr_in6* addr,
                                     size_t addrlen) {
  char host[NI_MAXHOST] = {0};
  char service[NI_MAXSERV] = {0};
  int ret = getnameinfo((const struct sockaddr*)addr, (socklen_t)addrlen, host,
                        NI_MAXHOST, service, NI_MAXSERV,
                        NI_NUMERICSERV | NI_NUMERICHOST);
  int len = 0;
  if (ret == 0) {
    if (addr->sin6_port != 0)
      len = snprintf(buffer, capacity, "[%s]:%s", host, service);
    else
      len = snprintf(buffer, capacity, "%s", host);
  }
  if (len >= (int)capacity) len = (int)capacity - 1;
  mdns_string_t str;
  str.str = buffer;
  str.length = len;
  return str;
}

std::string copy_mdns_string(mdns_string_t value) {
  if (!value.str || !value.length) return {};
  return std::string(value.str, value.length);
}

int query_callback(int sock, const struct sockaddr* from, size_t addrlen,
                   mdns_entry_type_t entry, uint16_t query_id, uint16_t rtype,
                   uint16_t rclass, uint32_t ttl, const void* data, size_t size,
                   size_t name_offset, size_t name_length, size_t record_offset,
                   size_t record_length, void* user_data) {
  (void)sock;
  (void)from;
  (void)addrlen;
  (void)entry;
  (void)query_id;
  (void)rclass;
  (void)name_length;

  auto* accumulator = static_cast<DiscoveryAccumulator*>(user_data);
  if (!accumulator || ttl == 0) return 0;

  char entry_buffer[256] = {};
  char record_buffer[256] = {};
  const auto owner = copy_mdns_string(mdns_string_extract(
      data, size, &name_offset, entry_buffer, sizeof(entry_buffer)));
  if (owner.empty()) return 0;

  if (rtype == MDNS_RECORDTYPE_PTR) {
    const auto target = copy_mdns_string(
        mdns_record_parse_ptr(data, size, record_offset, record_length,
                              record_buffer, sizeof(record_buffer)));
    accumulator->add_ptr(owner, target);
  } else if (rtype == MDNS_RECORDTYPE_SRV) {
    const auto srv =
        mdns_record_parse_srv(data, size, record_offset, record_length,
                              record_buffer, sizeof(record_buffer));
    accumulator->add_srv(owner, copy_mdns_string(srv.name));
  } else if (rtype == MDNS_RECORDTYPE_A) {
    if (record_length != 4) return 0;
    struct sockaddr_in addr = {};
    mdns_record_parse_a(data, size, record_offset, record_length, &addr);
    const auto address = ipv4_address_to_string(
        record_buffer, sizeof(record_buffer), &addr, sizeof(addr));
    accumulator->add_ipv4(owner, copy_mdns_string(address));
  } else if (rtype == MDNS_RECORDTYPE_AAAA) {
    if (record_length != 16) return 0;
    struct sockaddr_in6 addr = {};
    mdns_record_parse_aaaa(data, size, record_offset, record_length, &addr);
    const auto address = ipv6_address_to_string(
        record_buffer, sizeof(record_buffer), &addr, sizeof(addr));
    accumulator->add_ipv6(owner, copy_mdns_string(address));
  } else if (rtype == MDNS_RECORDTYPE_TXT) {
    mdns_record_txt_t txt_buffer[128] = {};
    const size_t parsed = mdns_record_parse_txt(
        data, size, record_offset, record_length, txt_buffer,
        sizeof(txt_buffer) / sizeof(mdns_record_txt_t));
    for (size_t itxt = 0; itxt < parsed; ++itxt) {
      const auto key = copy_mdns_string(txt_buffer[itxt].key);
      const auto value = copy_mdns_string(txt_buffer[itxt].value);
      if (!key.empty() && !value.empty())
        accumulator->add_txt(owner, key, value);
    }
  }
  return 0;
}

}  // namespace

Discovery::DiscoveryImpl::DiscoveryImpl() {}
Discovery::DiscoveryImpl::~DiscoveryImpl() {}
std::vector<ControllerInfo> Discovery::DiscoveryImpl::resolve() {
  mdns_query_t query[1] = {};
  size_t query_count = 0;
  query[query_count].name = PTR_.c_str();
  query[query_count].type = MDNS_RECORDTYPE_PTR;
  query[query_count].length = std::strlen(query[query_count].name);
  query_count++;
  return send_mdns_query(query, query_count);
}

std::vector<ControllerInfo> Discovery::DiscoveryImpl::send_mdns_query(
    mdns_query_t* query, size_t count) {
  constexpr size_t capacity = 2048;
  std::vector<std::uint32_t> buffer(capacity / sizeof(std::uint32_t));
  DiscoveryAccumulator accumulator;

  int sockets[32] = {};
  int query_id[32] = {};
  int num_sockets =
      open_client_sockets(sockets, sizeof(sockets) / sizeof(sockets[0]), 0);
  struct SocketGuard {
    int* sockets;
    int count;
    ~SocketGuard() {
      for (int index = 0; index < count; ++index)
        mdns_socket_close(sockets[index]);
    }
  } socket_guard{sockets, num_sockets};

  if (num_sockets <= 0) {
    printf("Failed to open any client sockets\n");
    return {};
  }

  for (size_t iq = 0; iq < count; ++iq) {
    if (query[iq].type != MDNS_RECORDTYPE_SRV &&
        query[iq].type != MDNS_RECORDTYPE_A &&
        query[iq].type != MDNS_RECORDTYPE_AAAA)
      query[iq].type = MDNS_RECORDTYPE_PTR;
  }
  for (int isock = 0; isock < num_sockets; ++isock) {
    query_id[isock] = mdns_multiquery_send(sockets[isock], query, count,
                                           buffer.data(), capacity, 0);
    if (query_id[isock] < 0)
      printf("Failed to send mDNS query: %s\n", strerror(errno));
  }

  int res;
  do {
    struct timeval timeout;
    timeout.tv_sec = 2;
    timeout.tv_usec = 0;

    int nfds = 0;
    fd_set readfs;
    FD_ZERO(&readfs);
    for (int isock = 0; isock < num_sockets; ++isock) {
      if (sockets[isock] >= nfds) nfds = sockets[isock] + 1;
      FD_SET(sockets[isock], &readfs);
    }

    res = select(nfds, &readfs, 0, 0, &timeout);
    if (res > 0) {
      for (int isock = 0; isock < num_sockets; ++isock) {
        if (FD_ISSET(sockets[isock], &readfs)) {
          mdns_query_recv(sockets[isock], buffer.data(), capacity,
                          query_callback, &accumulator, query_id[isock]);
        }
      }
    }
  } while (res > 0);

  return accumulator.controllers();
}

int Discovery::DiscoveryImpl::open_client_sockets(int* sockets, int max_sockets,
                                                  int port) {
  // When sending, each socket can only send to one network interface
  // Thus we need to open one socket for each interface and address family
  int num_sockets = 0;

#ifdef _WIN32

  IP_ADAPTER_ADDRESSES* adapter_address = 0;
  ULONG address_size = 8000;
  unsigned int ret;
  unsigned int num_retries = 4;
  do {
    adapter_address = (IP_ADAPTER_ADDRESSES*)malloc(address_size);
    ret = GetAdaptersAddresses(AF_UNSPEC,
                               GAA_FLAG_SKIP_MULTICAST | GAA_FLAG_SKIP_ANYCAST,
                               0, adapter_address, &address_size);
    if (ret == ERROR_BUFFER_OVERFLOW) {
      free(adapter_address);
      adapter_address = 0;
      address_size *= 2;
    } else {
      break;
    }
  } while (num_retries-- > 0);

  if (!adapter_address || (ret != NO_ERROR)) {
    free(adapter_address);
    printf("Failed to get network adapter addresses\n");
    return num_sockets;
  }

  for (PIP_ADAPTER_ADDRESSES adapter = adapter_address; adapter;
       adapter = adapter->Next) {
    if (adapter->TunnelType == TUNNEL_TYPE_TEREDO) continue;
    if (adapter->OperStatus != IfOperStatusUp) continue;

    for (IP_ADAPTER_UNICAST_ADDRESS* unicast = adapter->FirstUnicastAddress;
         unicast; unicast = unicast->Next) {
      if (unicast->Address.lpSockaddr->sa_family == AF_INET) {
        struct sockaddr_in* saddr =
            (struct sockaddr_in*)unicast->Address.lpSockaddr;
        if ((saddr->sin_addr.S_un.S_un_b.s_b1 != 127) ||
            (saddr->sin_addr.S_un.S_un_b.s_b2 != 0) ||
            (saddr->sin_addr.S_un.S_un_b.s_b3 != 0) ||
            (saddr->sin_addr.S_un.S_un_b.s_b4 != 1)) {
          if (num_sockets < max_sockets) {
            saddr->sin_port = htons((unsigned short)port);
            int sock = mdns_socket_open_ipv4(saddr);
            if (sock >= 0) sockets[num_sockets++] = sock;
          }
        }
      } else if (unicast->Address.lpSockaddr->sa_family == AF_INET6) {
        struct sockaddr_in6* saddr =
            (struct sockaddr_in6*)unicast->Address.lpSockaddr;
        // Ignore link-local addresses
        if (saddr->sin6_scope_id) continue;
        static const unsigned char localhost[] = {0, 0, 0, 0, 0, 0, 0, 0,
                                                  0, 0, 0, 0, 0, 0, 0, 1};
        static const unsigned char localhost_mapped[] = {
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xff, 0xff, 0x7f, 0, 0, 1};
        if ((unicast->DadState == NldsPreferred) &&
            memcmp(saddr->sin6_addr.s6_addr, localhost, 16) &&
            memcmp(saddr->sin6_addr.s6_addr, localhost_mapped, 16)) {
          if (num_sockets < max_sockets) {
            saddr->sin6_port = htons((unsigned short)port);
            int sock = mdns_socket_open_ipv6(saddr);
            if (sock >= 0) sockets[num_sockets++] = sock;
          }
        }
      }
    }
  }

  free(adapter_address);

#else

  struct ifaddrs* ifaddr = 0;
  struct ifaddrs* ifa = 0;

  if (getifaddrs(&ifaddr) < 0) {
    printf("Unable to get interface addresses\n");
    return num_sockets;
  }

  for (ifa = ifaddr; ifa; ifa = ifa->ifa_next) {
    if (!ifa->ifa_addr) continue;
    if (!(ifa->ifa_flags & IFF_UP) || !(ifa->ifa_flags & IFF_MULTICAST))
      continue;
    if ((ifa->ifa_flags & IFF_LOOPBACK) || (ifa->ifa_flags & IFF_POINTOPOINT))
      continue;

    if (ifa->ifa_addr->sa_family == AF_INET) {
      struct sockaddr_in* saddr = (struct sockaddr_in*)ifa->ifa_addr;
      if (saddr->sin_addr.s_addr != htonl(INADDR_LOOPBACK)) {
        if (num_sockets < max_sockets) {
          saddr->sin_port = htons(port);
          int sock = mdns_socket_open_ipv4(saddr);
          if (sock >= 0) sockets[num_sockets++] = sock;
        }
      }
    } else if (ifa->ifa_addr->sa_family == AF_INET6) {
      struct sockaddr_in6* saddr = (struct sockaddr_in6*)ifa->ifa_addr;
      // Ignore link-local addresses
      if (saddr->sin6_scope_id) continue;
      static const unsigned char localhost[] = {0, 0, 0, 0, 0, 0, 0, 0,
                                                0, 0, 0, 0, 0, 0, 0, 1};
      static const unsigned char localhost_mapped[] = {
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xff, 0xff, 0x7f, 0, 0, 1};
      if (memcmp(saddr->sin6_addr.s6_addr, localhost, 16) &&
          memcmp(saddr->sin6_addr.s6_addr, localhost_mapped, 16)) {
        if (num_sockets < max_sockets) {
          saddr->sin6_port = htons(port);
          int sock = mdns_socket_open_ipv6(saddr);
          if (sock >= 0) sockets[num_sockets++] = sock;
        }
      }
    }
  }

  freeifaddrs(ifaddr);

#endif

  return num_sockets;
}
}  // namespace zeroconf
}  // namespace lebai
