#include <gtest/gtest.h>

#include <asio.hpp>

#include <chrono>
#include <stdexcept>
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

TEST(TimedOperationTest, SynchronousStartFailureCancelsAndDrainsBeforeRethrow) {
  asio::io_context io;
  asio::steady_timer failed_operation(io);
  failed_operation.expires_after(std::chrono::milliseconds(0));
  int cancel_calls = 0;
  bool operation_completed = false;
  bool caught_start_error = false;

  try {
    lebai::run_timed_operation(
        io, kPendingDuration,
        [&](auto complete) {
          failed_operation.async_wait(
              [&, complete](const std::error_code& error) {
                operation_completed = true;
                complete(error);
              });
          throw std::length_error("known start failure");
        },
        [&] {
          ++cancel_calls;
          std::error_code ignored;
          failed_operation.cancel(ignored);
        });
    ADD_FAILURE() << "Expected std::length_error";
  } catch (const std::length_error& error) {
    caught_start_error = true;
    EXPECT_STREQ("known start failure", error.what());
  } catch (const std::exception& error) {
    ADD_FAILURE() << "Expected std::length_error, got: " << error.what();
  } catch (...) {
    ADD_FAILURE() << "Expected std::length_error, got a non-standard exception";
  }

  EXPECT_TRUE(caught_start_error);
  EXPECT_EQ(1, cancel_calls);
  EXPECT_TRUE(operation_completed);

  asio::steady_timer next_operation(io);
  next_operation.expires_after(std::chrono::milliseconds(0));
  const auto next_error = std::make_error_code(std::errc::connection_refused);
  const auto next_result = lebai::run_timed_operation(
      io, kPendingDuration,
      [&](auto complete) {
        next_operation.async_wait(
            [complete, next_error](const std::error_code&) {
              complete(next_error);
            });
      },
      [&] {
        std::error_code ignored;
        next_operation.cancel(ignored);
      });

  EXPECT_EQ(next_error, next_result.error);
  EXPECT_FALSE(next_result.timed_out);
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
