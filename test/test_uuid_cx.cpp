// Copyright 2024 Peter Dimov
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

#include <boost/uuid/uuid.hpp>
#include <boost/config.hpp>

#if defined(BOOST_NO_CXX14_CONSTEXPR)

#include <boost/config/pragma_message.hpp>
BOOST_PRAGMA_MESSAGE("Skipping test because BOOST_NO_CXX14_CONSTEXPR is defined")

#else

#define STATIC_ASSERT(...) static_assert(__VA_ARGS__, #__VA_ARGS__)

using namespace boost::uuids;

constexpr uuid u1;
constexpr uuid u2 = {{ 0x01, 0x02 }};

#endif
