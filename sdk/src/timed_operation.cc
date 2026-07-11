#include "timed_operation.hh"

#include <asio/steady_timer.hpp>

namespace lebai {

TimedOperationResult run_timed_operation(
    asio::io_context& io, std::chrono::milliseconds timeout,
    const std::function<void(std::function<void(std::error_code)>)>& start,
    const std::function<void()>& cancel) {
  TimedOperationResult result;
  bool completed = false;
  bool cancel_called = false;

  io.restart();
  asio::steady_timer deadline(io);
  deadline.expires_after(timeout);
  deadline.async_wait([&](const std::error_code& error) {
    if (error || completed) {
      return;
    }

    completed = true;
    result.timed_out = true;
    if (!cancel_called) {
      cancel_called = true;
      cancel();
    }
  });

  start([&](std::error_code error) {
    if (completed) {
      return;
    }

    completed = true;
    result.error = error;
    std::error_code ignored;
    deadline.cancel(ignored);
  });

  io.run();
  return result;
}

}  // namespace lebai
