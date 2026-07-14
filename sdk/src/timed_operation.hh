#pragma once

#include <asio/io_context.hpp>

#include <chrono>
#include <functional>
#include <system_error>

namespace lebai {

struct TimedOperationResult {
  std::error_code error;
  bool timed_out{};
};

TimedOperationResult run_timed_operation(
    asio::io_context& io, std::chrono::milliseconds timeout,
    const std::function<void(std::function<void(std::error_code)>)>& start,
    const std::function<void()>& cancel);

}  // namespace lebai
