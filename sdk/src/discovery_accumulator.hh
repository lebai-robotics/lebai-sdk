#pragma once

#include <lebai/discovery.hh>

#include <map>
#include <string>
#include <vector>

namespace lebai {
namespace zeroconf {

class DiscoveryAccumulator {
 public:
  void add_ptr(const std::string& service_owner, const std::string& instance);
  void add_srv(const std::string& instance, const std::string& target);
  void add_txt(const std::string& instance, const std::string& key,
               const std::string& value);
  void add_ipv4(const std::string& target, const std::string& address);
  void add_ipv6(const std::string& target, const std::string& address);

  std::vector<ControllerInfo> controllers() const;

 private:
  struct InstanceRecords {
    bool is_lebai_service{};
    std::string ptr_target;
    std::string srv_target;
    std::map<std::string, std::string> txt;
  };

  struct TargetAddresses {
    std::string ipv4;
    std::string ipv6;
  };

  static std::string normalize_dns_name(std::string name);

  std::map<std::string, InstanceRecords> instances_;
  std::map<std::string, TargetAddresses> addresses_;
};

}  // namespace zeroconf
}  // namespace lebai
