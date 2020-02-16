//
// inline_executor.cpp
// ~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2019 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

// Disable autolinking for unit tests.
#if !defined(BOOST_ALL_NO_LIB)
#define BOOST_ALL_NO_LIB 1
#endif // !defined(BOOST_ALL_NO_LIB)

// Test that header file is self-contained.
#include "asio/inline_executor.hpp"

#include "unit_test.hpp"

#include <memory>
#include <type_traits>
#include "asio/is_executor.hpp"

static_assert(asio::is_executor<asio::inline_executor>::value, "");

void inline_executor_test() {
  bool invoked = false;
  auto f = [&]() noexcept { invoked = true; };
  const auto& c = f;
  std::allocator<void> alloc;
  asio::inline_executor ex;
  ex.on_work_started();
  ex.on_work_finished();
  ASIO_CHECK(ex == ex);
  ASIO_CHECK(!(ex != ex));
  asio::inline_executor cpy(ex);
  ASIO_CHECK(ex == cpy);
  ASIO_CHECK(!(ex != cpy));
  asio::inline_executor other(ex);
  ASIO_CHECK(ex == other);
  ASIO_CHECK(!(ex != other));
  ex.dispatch(f);
  ASIO_CHECK(invoked);
  invoked = false;
  ex.dispatch(f, alloc);
  ASIO_CHECK(invoked);
  invoked = false;
  ex.dispatch(c);
  ASIO_CHECK(invoked);
  invoked = false;
  ex.dispatch(c, alloc);
  ASIO_CHECK(invoked);
  invoked = false;
  ex.defer(f);
  ASIO_CHECK(invoked);
  invoked = false;
  ex.defer(f, alloc);
  ASIO_CHECK(invoked);
  invoked = false;
  ex.defer(c);
  ASIO_CHECK(invoked);
  invoked = false;
  ex.defer(c, alloc);
  ASIO_CHECK(invoked);
  invoked = false;
  bool threw = false;
  try {
    ex.post(f);
  } catch (...) {
    threw = true;
  }
  ASIO_CHECK(!invoked);
  ASIO_CHECK(threw);
  threw = false;
  invoked = false;
  try {
    ex.post(c);
  } catch (...) {
    threw = true;
  }
  ASIO_CHECK(!invoked);
  ASIO_CHECK(threw);
  threw = false;
  invoked = false;
  try {
    ex.post(c, alloc);
  } catch (...) {
    threw = true;
  }
  ASIO_CHECK(!invoked);
  ASIO_CHECK(threw);
  threw = false;
  invoked = false;
  try {
    ex.post(f, alloc);
  } catch (...) {
    threw = true;
  }
  ASIO_CHECK(!invoked);
  ASIO_CHECK(threw);
}

ASIO_TEST_SUITE
(
  "inline_executor",
  ASIO_TEST_CASE(inline_executor_test)
)
