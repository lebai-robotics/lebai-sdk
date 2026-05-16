#include <gtest/gtest.h>

#include <atomic>
#include <climits>

#include "rpc_client.hh"

namespace lebai {
struct RpcClientTestAccess {
  static auto next_id() -> int { return RpcClient::NextId(); }
  static auto mutable_next_id() -> std::atomic<int>& {
    return RpcClient::MutableNextIdForTesting();
  }
};
}  // namespace lebai

TEST(RpcClientTest, NextIdRemainsPositiveAcrossWrap) {
  lebai::RpcClientTestAccess::mutable_next_id().store(INT_MAX - 1);

  EXPECT_EQ(INT_MAX, lebai::RpcClientTestAccess::next_id());
  EXPECT_EQ(1, lebai::RpcClientTestAccess::next_id());
  EXPECT_EQ(2, lebai::RpcClientTestAccess::next_id());
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
