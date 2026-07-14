#include "discovery_accumulator.hh"

#include <utility>

namespace lebai {
namespace zeroconf {
namespace {

constexpr char kLebaiService[] = "_lebai._tcp.local";

const std::string* find_nonempty(
    const std::map<std::string, std::string>& records, const std::string& key) {
  const auto record = records.find(key);
  if (record == records.end() || record->second.empty()) return nullptr;
  return &record->second;
}

}  // namespace

void DiscoveryAccumulator::add_ptr(const std::string& service_owner,
                                   const std::string& instance) {
  if (normalize_dns_name(service_owner) != kLebaiService) return;

  const auto normalized_instance = normalize_dns_name(instance);
  if (normalized_instance.empty()) return;

  auto& records = instances_[normalized_instance];
  records.is_lebai_service = true;
  if (records.ptr_target.empty()) records.ptr_target = instance;
}

void DiscoveryAccumulator::add_srv(const std::string& instance,
                                   const std::string& target) {
  const auto normalized_instance = normalize_dns_name(instance);
  const auto normalized_target = normalize_dns_name(target);
  if (normalized_instance.empty() || normalized_target.empty()) return;

  auto& srv_target = instances_[normalized_instance].srv_target;
  if (srv_target.empty()) srv_target = normalized_target;
}

void DiscoveryAccumulator::add_txt(const std::string& instance,
                                   const std::string& key,
                                   const std::string& value) {
  const auto normalized_instance = normalize_dns_name(instance);
  if (normalized_instance.empty()) return;
  instances_[normalized_instance].txt.emplace(key, value);
}

void DiscoveryAccumulator::add_ipv4(const std::string& target,
                                    const std::string& address) {
  const auto normalized_target = normalize_dns_name(target);
  if (normalized_target.empty() || address.empty()) return;

  auto& ipv4 = addresses_[normalized_target].ipv4;
  if (ipv4.empty()) ipv4 = address;
}

void DiscoveryAccumulator::add_ipv6(const std::string& target,
                                    const std::string& address) {
  const auto normalized_target = normalize_dns_name(target);
  if (normalized_target.empty() || address.empty()) return;

  auto& ipv6 = addresses_[normalized_target].ipv6;
  if (ipv6.empty()) ipv6 = address;
}

std::vector<ControllerInfo> DiscoveryAccumulator::controllers() const {
  std::vector<ControllerInfo> controllers;
  for (const auto& instance : instances_) {
    const auto& records = instance.second;
    if (!records.is_lebai_service || records.ptr_target.empty() ||
        records.srv_target.empty())
      continue;

    const auto model = find_nonempty(records.txt, "model");
    const auto ds = find_nonempty(records.txt, "ds");
    const auto rc = find_nonempty(records.txt, "rc");
    const auto id = find_nonempty(records.txt, "id");
    const auto mac = find_nonempty(records.txt, "mac");
    if (!model || !ds || !rc || !id || !mac) continue;

    const auto target = addresses_.find(records.srv_target);
    if (target == addresses_.end()) continue;
    const auto& address =
        target->second.ipv4.empty() ? target->second.ipv6 : target->second.ipv4;
    if (address.empty()) continue;

    const auto label_end = records.ptr_target.find('.');
    const auto hostname = records.ptr_target.substr(0, label_end);
    if (hostname.empty()) continue;

    ControllerInfo controller;
    controller.hostname = hostname;
    controller.ip_address = address;
    controller.mac_address = *mac;
    controller.model = *model;
    controller.ds_version = *ds;
    controller.rc_version = *rc;
    controller.id = *id;
    controllers.push_back(std::move(controller));
  }
  return controllers;
}

std::string DiscoveryAccumulator::normalize_dns_name(std::string name) {
  while (!name.empty() && name.back() == '.') name.pop_back();
  for (auto& character : name) {
    if (character >= 'A' && character <= 'Z')
      character = static_cast<char>(character - 'A' + 'a');
  }
  return name;
}

}  // namespace zeroconf
}  // namespace lebai
