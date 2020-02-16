//
// detail/handler_work.hpp
// ~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2019 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef ASIO_DETAIL_HANDLER_WORK_HPP
#define ASIO_DETAIL_HANDLER_WORK_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include "asio/detail/config.hpp"
#include "asio/associated_executor.hpp"
#include "asio/detail/handler_invoke_helpers.hpp"
#include "asio/inline_executor.hpp"

#include "asio/detail/push_options.hpp"

namespace asio {
namespace detail {

// A helper class template to allow completion handlers to be dispatched
// through either the new executors framework or the old invocaton hook. The
// primary template uses the new executors framework.
template <typename Handler,
    typename IoExecutor = inline_executor, typename HandlerExecutor
      = typename associated_executor<Handler, IoExecutor>::type>
class handler_work
{
public:
  explicit handler_work(Handler& handler) ASIO_NOEXCEPT
    : io_executor_(),
      executor_(asio::get_associated_executor(handler, io_executor_))
  {
  }

  handler_work(Handler& handler, const IoExecutor& io_ex) ASIO_NOEXCEPT
    : io_executor_(io_ex),
      executor_(asio::get_associated_executor(handler, io_executor_))
  {
  }

  static void start(Handler& handler) ASIO_NOEXCEPT
  {
    HandlerExecutor ex(asio::get_associated_executor(handler));
    ex.on_work_started();
  }

  static void start(Handler& handler,
      const IoExecutor& io_ex) ASIO_NOEXCEPT
  {
    HandlerExecutor ex(asio::get_associated_executor(handler, io_ex));
    ex.on_work_started();
    io_ex.on_work_started();
  }

  ~handler_work()
  {
    io_executor_.on_work_finished();
    executor_.on_work_finished();
  }

  template <typename Function>
  void complete(Function& function, Handler& handler)
  {
    executor_.dispatch(ASIO_MOVE_CAST(Function)(function),
        asio::get_associated_allocator(handler));
  }

private:
  // Disallow copying and assignment.
  handler_work(const handler_work&);
  handler_work& operator=(const handler_work&);

  IoExecutor io_executor_;
  HandlerExecutor executor_;
};

} // namespace detail
} // namespace asio

#include "asio/detail/pop_options.hpp"

#endif // ASIO_DETAIL_HANDLER_WORK_HPP
