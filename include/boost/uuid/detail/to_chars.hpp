#ifndef BOOST_UUID_DETAIL_TO_CHARS_HPP_INCLUDED
#define BOOST_UUID_DETAIL_TO_CHARS_HPP_INCLUDED

// Copyright 2009 Andy Tompkins
// Copyright 2024 Peter Dimov
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/detail/config.hpp>
#include <boost/uuid/detail/is_constant_evaluated.hpp>
#include <boost/uuid/detail/to_chars_generic.hpp>
#if defined(BOOST_UUID_USE_SSSE3)
#include <boost/uuid/detail/to_chars_x86.hpp>
#endif

namespace boost {
namespace uuids {
namespace detail {

template<class Ch> BOOST_UUID_CXX14_CONSTEXPR_RT inline Ch* to_chars( uuid const& u, Ch* out ) noexcept
{
#if defined(BOOST_UUID_USE_SSSE3)
    if( detail::is_constant_evaluated_rt() )
    {
        return detail::to_chars_generic( u, out );
    }
    else
    {
        return detail::to_chars_simd( u, out );
    }
#else
    return detail::to_chars_generic( u, out );
#endif
}

} // namespace detail
}} //namespace boost::uuids

#endif // BOOST_UUID_DETAIL_TO_CHARS_HPP_INCLUDED
