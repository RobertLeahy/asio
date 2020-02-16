//
// inline_executor.hpp
// ~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2019 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef ASIO_INLINE_EXECUTOR_HPP
#define ASIO_INLINE_EXECUTOR_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include "asio/detail/config.hpp"
#include "asio/execution_context.hpp"

#include "asio/detail/push_options.hpp"

namespace asio {

class inline_context : public execution_context {};

class inline_executor {
private:
  template<typename Handler>
  void impl(Handler&& h) {
    h();
  }
  template<typename Handler>
  void impl(Handler& h) {
    h();
  }
  template<typename Handler>
  void impl(const Handler& h) {
    Handler cpy(h);
    cpy();
  }
public:
  void on_work_started() const noexcept {}
  void on_work_finished() const noexcept {}
  template<typename Handler>
  void dispatch(ASIO_MOVE_ARG(Handler) h) {
    impl(ASIO_MOVE_CAST(Handler)(h));
  }
  template<typename Handler, typename Allocator>
  void dispatch(ASIO_MOVE_ARG(Handler) h, const Allocator&) {
    impl(ASIO_MOVE_CAST(Handler)(h));
  }
  template<typename Handler>
  void defer(ASIO_MOVE_ARG(Handler) h) {
    impl(ASIO_MOVE_CAST(Handler)(h));
  }
  template<typename Handler, typename Allocator>
  void defer(ASIO_MOVE_ARG(Handler) h, const Allocator&) {
    impl(ASIO_MOVE_CAST(Handler)(h));
  }
  template<typename Handler>
  void post(const Handler&) {
    throw std::logic_error("Cannot post to inline_executor");
  }
  template<typename Handler, typename Allocator>
  void post(const Handler& h, const Allocator&) {
    post(h);
  }
  inline_context& context() const noexcept {
    static inline_context retr;
    return retr;
  }
private:
  friend bool operator==(const inline_executor&, const inline_executor&) noexcept {
    return true;
  }
  friend bool operator!=(const inline_executor&, const inline_executor&) noexcept {
    return false;
  }
};

} // namespace asio

#include "asio/detail/pop_options.hpp"

#endif // ASIO_INLINE_EXECUTOR_HPP
