#include "timed_operation.hh"

#include <asio/steady_timer.hpp>

#include <exception>

namespace lebai {

TimedOperationResult run_timed_operation(
    asio::io_context& io, std::chrono::milliseconds timeout,
    const std::function<void(std::function<void(std::error_code)>)>& start,
    const std::function<void()>& cancel) {
  TimedOperationResult result;
  std::exception_ptr start_error;
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

  try {
    start([&](std::error_code error) {
      if (completed) {
        return;
      }

      completed = true;
      result.error = error;
      std::error_code ignored;
      deadline.cancel(ignored);
    });
  } catch (...) {
    start_error = std::current_exception();
    completed = true;

    std::error_code ignored;
    deadline.cancel(ignored);
    if (!cancel_called) {
      cancel_called = true;
      cancel();
    }
  }

  io.run();
  if (start_error) {
    std::rethrow_exception(start_error);
  }
  return result;
}

}  // namespace lebai
