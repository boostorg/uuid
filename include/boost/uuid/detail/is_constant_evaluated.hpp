#ifndef BOOST_UUID_DETAIL_IS_CONSTANT_EVALUATED_INCLUDED
#define BOOST_UUID_DETAIL_IS_CONSTANT_EVALUATED_INCLUDED

// Copyright 2025 Peter Dimov
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

#include <boost/config.hpp>

#if defined(__has_builtin)
# if __has_builtin(__builtin_is_constant_evaluated)
#  define BOOST_UUID_HAS_BUILTIN_IS_CONSTANT_EVALUATED
# endif
#endif

#if !defined(BOOST_UUID_HAS_BUILTIN_IS_CONSTANT_EVALUATED) && defined(BOOST_MSVC) && BOOST_MSVC >= 1925
# define BOOST_UUID_HAS_BUILTIN_IS_CONSTANT_EVALUATED
#endif

namespace boost {
namespace uuids {
namespace detail {

#if defined(BOOST_NO_CXX14_CONSTEXPR)

constexpr bool is_constant_evaluated() noexcept
{
    return false;
}

#elif defined(BOOST_UUID_HAS_BUILTIN_IS_CONSTANT_EVALUATED)

constexpr bool is_constant_evaluated() noexcept
{
    return __builtin_is_constant_evaluated();
}

#else

constexpr bool is_constant_evaluated() noexcept
{
    return true;
}

#endif

}}} // namespace boost::uuids::detail

#endif // #ifndef BOOST_UUID_DETAIL_IS_CONSTANT_EVALUATED_INCLUDED
