#include <gtest/gtest.h>

#include <string>
#include <vector>

#include "discovery_accumulator.hh"

namespace {

using lebai::zeroconf::ControllerInfo;
using lebai::zeroconf::DiscoveryAccumulator;

void add_txt_records(DiscoveryAccumulator& accumulator,
                     const std::string& instance, const std::string& suffix) {
  accumulator.add_txt(instance, "model", "model-" + suffix);
  accumulator.add_txt(instance, "ds", "ds-" + suffix);
  accumulator.add_txt(instance, "rc", "rc-" + suffix);
  accumulator.add_txt(instance, "id", "id-" + suffix);
  accumulator.add_txt(instance, "mac", "mac-" + suffix);
}

void expect_controller(const ControllerInfo& controller,
                       const std::string& hostname, const std::string& address,
                       const std::string& suffix) {
  EXPECT_EQ(hostname, controller.hostname);
  EXPECT_EQ(address, controller.ip_address);
  EXPECT_EQ("mac-" + suffix, controller.mac_address);
  EXPECT_EQ("model-" + suffix, controller.model);
  EXPECT_EQ("ds-" + suffix, controller.ds_version);
  EXPECT_EQ("rc-" + suffix, controller.rc_version);
  EXPECT_EQ("id-" + suffix, controller.id);
}

TEST(DiscoveryAccumulatorTest,
     InterleavedControllersAreReturnedInDeterministicOrder) {
  DiscoveryAccumulator accumulator;

  accumulator.add_ptr("_lebai._tcp.local", "Zulu._lebai._tcp.local");
  accumulator.add_srv("Alpha._lebai._tcp.local", "alpha-host.local");
  accumulator.add_txt("Zulu._lebai._tcp.local", "model", "model-zulu");
  accumulator.add_ipv4("alpha-host.local", "192.0.2.10");
  accumulator.add_ptr("_lebai._tcp.local", "Alpha._lebai._tcp.local");
  accumulator.add_txt("Zulu._lebai._tcp.local", "ds", "ds-zulu");
  add_txt_records(accumulator, "Alpha._lebai._tcp.local", "alpha");
  accumulator.add_srv("Zulu._lebai._tcp.local", "zulu-host.local");
  accumulator.add_txt("Zulu._lebai._tcp.local", "rc", "rc-zulu");
  accumulator.add_ipv4("zulu-host.local", "192.0.2.20");
  accumulator.add_txt("Zulu._lebai._tcp.local", "id", "id-zulu");
  accumulator.add_txt("Zulu._lebai._tcp.local", "mac", "mac-zulu");

  const auto controllers = accumulator.controllers();

  ASSERT_EQ(2U, controllers.size());
  expect_controller(controllers[0], "Alpha", "192.0.2.10", "alpha");
  expect_controller(controllers[1], "Zulu", "192.0.2.20", "zulu");
}

TEST(DiscoveryAccumulatorTest, RecordsMayArriveBeforePtrMembership) {
  DiscoveryAccumulator accumulator;
  const std::string instance = "Reverse._lebai._tcp.local";

  accumulator.add_ipv4("reverse-host.local", "198.51.100.7");
  add_txt_records(accumulator, instance, "reverse");
  accumulator.add_srv(instance, "reverse-host.local");
  accumulator.add_ptr("_lebai._tcp.local", instance);

  const auto controllers = accumulator.controllers();

  ASSERT_EQ(1U, controllers.size());
  expect_controller(controllers[0], "Reverse", "198.51.100.7", "reverse");
}

TEST(DiscoveryAccumulatorTest,
     DnsNamesAssociateAcrossCaseAndTrailingDotDifferences) {
  DiscoveryAccumulator accumulator;

  accumulator.add_ptr("_LeBaI._TcP.LoCaL...", "MiXeD._LEBAI._TCP.LOCAL...");
  accumulator.add_srv("mixed._lebai._tcp.local", "HOST.Local...");
  add_txt_records(accumulator, "MIXED._LEBAI._TCP.LOCAL.", "mixed");
  accumulator.add_ipv4("host.local", "203.0.113.11");

  const auto controllers = accumulator.controllers();

  ASSERT_EQ(1U, controllers.size());
  expect_controller(controllers[0], "MiXeD", "203.0.113.11", "mixed");
}

TEST(DiscoveryAccumulatorTest, PtrOwnerOutsideLebaiServiceIsIgnored) {
  DiscoveryAccumulator accumulator;
  const std::string instance = "Other._lebai._tcp.local";

  accumulator.add_ptr("_other._tcp.local", instance);
  accumulator.add_srv(instance, "other-host.local");
  add_txt_records(accumulator, instance, "other");
  accumulator.add_ipv4("other-host.local", "192.0.2.30");

  EXPECT_TRUE(accumulator.controllers().empty());
}

TEST(DiscoveryAccumulatorTest, Ipv4IsPreferredAndIpv6IsABareFallback) {
  DiscoveryAccumulator accumulator;
  const std::string dual_instance = "Dual._lebai._tcp.local";
  const std::string ipv6_instance = "V6Only._lebai._tcp.local";

  accumulator.add_ptr("_lebai._tcp.local", dual_instance);
  accumulator.add_srv(dual_instance, "dual-host.local");
  add_txt_records(accumulator, dual_instance, "dual");
  accumulator.add_ipv6("dual-host.local", "2001:db8::20");
  accumulator.add_ipv4("dual-host.local", "192.0.2.40");

  accumulator.add_ptr("_lebai._tcp.local", ipv6_instance);
  accumulator.add_srv(ipv6_instance, "v6-host.local");
  add_txt_records(accumulator, ipv6_instance, "v6");
  accumulator.add_ipv6("v6-host.local", "2001:db8::60");

  const auto controllers = accumulator.controllers();

  ASSERT_EQ(2U, controllers.size());
  expect_controller(controllers[0], "Dual", "192.0.2.40", "dual");
  expect_controller(controllers[1], "V6Only", "2001:db8::60", "v6");
}

TEST(DiscoveryAccumulatorTest, ExactDuplicateRecordsYieldOneController) {
  DiscoveryAccumulator accumulator;
  const std::string instance = "Duplicate._lebai._tcp.local";

  for (int i = 0; i < 2; ++i) {
    accumulator.add_ptr("_lebai._tcp.local", instance);
    accumulator.add_srv(instance, "duplicate-host.local");
    add_txt_records(accumulator, instance, "duplicate");
    accumulator.add_ipv4("duplicate-host.local", "192.0.2.50");
  }

  const auto controllers = accumulator.controllers();

  ASSERT_EQ(1U, controllers.size());
  expect_controller(controllers[0], "Duplicate", "192.0.2.50", "duplicate");
}

enum class OmittedRecord { kPtr, kSrv, kModel, kDs, kRc, kId, kMac, kAddress };

TEST(DiscoveryAccumulatorTest, OmitsControllerMissingAnyMandatoryRecord) {
  const std::vector<OmittedRecord> omissions = {
      OmittedRecord::kPtr, OmittedRecord::kSrv,    OmittedRecord::kModel,
      OmittedRecord::kDs,  OmittedRecord::kRc,     OmittedRecord::kId,
      OmittedRecord::kMac, OmittedRecord::kAddress};

  for (const auto omission : omissions) {
    SCOPED_TRACE(static_cast<int>(omission));
    DiscoveryAccumulator accumulator;
    const std::string instance = "Incomplete._lebai._tcp.local";

    if (omission != OmittedRecord::kPtr)
      accumulator.add_ptr("_lebai._tcp.local", instance);
    if (omission != OmittedRecord::kSrv)
      accumulator.add_srv(instance, "incomplete-host.local");
    if (omission != OmittedRecord::kModel)
      accumulator.add_txt(instance, "model", "model-incomplete");
    if (omission != OmittedRecord::kDs)
      accumulator.add_txt(instance, "ds", "ds-incomplete");
    if (omission != OmittedRecord::kRc)
      accumulator.add_txt(instance, "rc", "rc-incomplete");
    if (omission != OmittedRecord::kId)
      accumulator.add_txt(instance, "id", "id-incomplete");
    if (omission != OmittedRecord::kMac)
      accumulator.add_txt(instance, "mac", "mac-incomplete");
    if (omission != OmittedRecord::kAddress)
      accumulator.add_ipv4("incomplete-host.local", "192.0.2.60");

    EXPECT_TRUE(accumulator.controllers().empty());
  }
}

TEST(DiscoveryAccumulatorTest, ServiceInstancesCannotBorrowEachOthersFields) {
  DiscoveryAccumulator accumulator;
  const std::string first = "First._lebai._tcp.local";
  const std::string second = "Second._lebai._tcp.local";

  accumulator.add_ptr("_lebai._tcp.local", first);
  accumulator.add_srv(first, "first-host.local");
  accumulator.add_ipv4("first-host.local", "192.0.2.70");
  accumulator.add_txt(first, "model", "model-first");
  accumulator.add_txt(first, "ds", "ds-first");

  accumulator.add_ptr("_lebai._tcp.local", second);
  accumulator.add_srv(second, "second-host.local");
  accumulator.add_ipv4("second-host.local", "192.0.2.80");
  accumulator.add_txt(second, "rc", "rc-second");
  accumulator.add_txt(second, "id", "id-second");
  accumulator.add_txt(second, "mac", "mac-second");

  EXPECT_TRUE(accumulator.controllers().empty());
}

TEST(DiscoveryAccumulatorTest, AccumulatorObjectsDoNotShareState) {
  DiscoveryAccumulator records_without_membership;
  DiscoveryAccumulator membership_without_records;
  const std::string instance = "Isolated._lebai._tcp.local";

  records_without_membership.add_srv(instance, "isolated-host.local");
  add_txt_records(records_without_membership, instance, "isolated");
  records_without_membership.add_ipv4("isolated-host.local", "192.0.2.90");
  membership_without_records.add_ptr("_lebai._tcp.local", instance);

  EXPECT_TRUE(records_without_membership.controllers().empty());
  EXPECT_TRUE(membership_without_records.controllers().empty());
}

}  // namespace

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
