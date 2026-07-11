#include <gtest/gtest.h>

#include <asio.hpp>

#include <chrono>
#include <system_error>

#include "timed_operation.hh"

namespace {

constexpr auto kPendingDuration = std::chrono::hours(1);

TEST(TimedOperationTest, OperationFirstReturnsSuccess) {
  asio::io_context io;
  io.run();
  ASSERT_TRUE(io.stopped());

  asio::steady_timer operation(io);
  operation.expires_after(std::chrono::milliseconds(0));
  int cancel_calls = 0;
  bool operation_completed = false;

  const auto result = lebai::run_timed_operation(
      io, kPendingDuration,
      [&](auto complete) {
        operation.async_wait([&, complete](const std::error_code& error) {
          operation_completed = true;
          complete(error);
        });
      },
      [&] {
        ++cancel_calls;
        std::error_code ignored;
        operation.cancel(ignored);
      });

  EXPECT_FALSE(result.error);
  EXPECT_FALSE(result.timed_out);
  EXPECT_EQ(0, cancel_calls);
  EXPECT_TRUE(operation_completed);
}

TEST(TimedOperationTest, TimerFirstCancelsPendingOperation) {
  asio::io_context io;
  asio::steady_timer operation(io);
  operation.expires_after(kPendingDuration);
  int cancel_calls = 0;
  bool operation_completed = false;

  const auto result = lebai::run_timed_operation(
      io, std::chrono::milliseconds(0),
      [&](auto complete) {
        operation.async_wait([&, complete](const std::error_code& error) {
          operation_completed = true;
          complete(error);
        });
      },
      [&] {
        ++cancel_calls;
        std::error_code ignored;
        operation.cancel(ignored);
      });

  EXPECT_FALSE(result.error);
  EXPECT_TRUE(result.timed_out);
  EXPECT_EQ(1, cancel_calls);
  EXPECT_TRUE(operation_completed);
}

TEST(TimedOperationTest, OperationErrorIsReturned) {
  asio::io_context io;
  asio::steady_timer operation(io);
  operation.expires_after(std::chrono::milliseconds(0));
  const auto operation_error =
      std::make_error_code(std::errc::connection_refused);

  const auto result = lebai::run_timed_operation(
      io, kPendingDuration,
      [&](auto complete) {
        operation.async_wait(
            [complete, operation_error](const std::error_code&) {
              complete(operation_error);
            });
      },
      [&] {
        std::error_code ignored;
        operation.cancel(ignored);
      });

  EXPECT_EQ(operation_error, result.error);
  EXPECT_FALSE(result.timed_out);
}

TEST(TimedOperationTest, LateCompletionAfterTimeoutIsIgnored) {
  asio::io_context io;
  asio::steady_timer operation(io);
  operation.expires_after(kPendingDuration);
  const auto late_error = std::make_error_code(std::errc::io_error);
  int cancel_calls = 0;
  bool operation_completed = false;

  const auto result = lebai::run_timed_operation(
      io, std::chrono::milliseconds(0),
      [&](auto complete) {
        operation.async_wait([&, complete](const std::error_code&) {
          operation_completed = true;
          complete(late_error);
        });
      },
      [&] {
        ++cancel_calls;
        std::error_code ignored;
        operation.cancel(ignored);
      });

  EXPECT_FALSE(result.error);
  EXPECT_TRUE(result.timed_out);
  EXPECT_EQ(1, cancel_calls);
  EXPECT_TRUE(operation_completed);
}

}  // namespace

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
