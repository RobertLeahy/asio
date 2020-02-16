//
// associated_executor.cpp
// ~~~~~~~~~~~~~~~~~~~~~~~
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
#include "asio/associated_executor.hpp"
#include "asio/inline_executor.hpp"

#include "unit_test.hpp"

#include <type_traits>

static_assert(std::is_same<asio::associated_executor_t<int, float>, float>::value, "");
static_assert(std::is_same<asio::associated_executor_t<int>, asio::inline_executor>::value, "");

struct foo {};

namespace asio {
template<typename E>
struct associated_executor<::foo, E> {
  using type = int;
  static type get(const ::foo&, const E& e = E()) {
    return 5;
  }
};
}

static_assert(std::is_same<asio::associated_executor_t<foo>, int>::value, "");
static_assert(std::is_same<asio::associated_executor_t<foo, float>, int>::value, "");

struct bar {};

namespace asio {
template<typename E>
struct associated_executor<::bar, E> {
  using type = int;
  static type get(const ::bar&, const E& e = E()) {
    return 5;
  }
};
}

static_assert(std::is_same<asio::associated_executor_t<bar>, int>::value, "");
static_assert(std::is_same<asio::associated_executor_t<bar, float>, int>::value, "");

ASIO_TEST_SUITE
(
  "associated_executor",
  ASIO_TEST_CASE(null_test)
)
