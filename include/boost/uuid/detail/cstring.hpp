#ifndef BOOST_UUID_DETAIL_CSTRING_INCLUDED
#define BOOST_UUID_DETAIL_CSTRING_INCLUDED

// Copyright 2025 Peter Dimov
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

#include <boost/uuid/detail/is_constant_evaluated.hpp>
#include <boost/config.hpp>
#include <cstring>
#include <cstddef>

namespace boost {
namespace uuids {
namespace detail {

// memcpy

BOOST_CXX14_CONSTEXPR inline void memcpy_cx( unsigned char* dest, unsigned char const* src, std::size_t n )
{
    if( is_constant_evaluated_cx() )
    {
        for( std::size_t i = 0; i < n; ++i ) dest[ i ] = src[ i ];
    }
    else
    {
        std::memcpy( dest, src, n );
    }
}

BOOST_UUID_CXX14_CONSTEXPR_RT inline void memcpy_rt( unsigned char* dest, unsigned char const* src, std::size_t n )
{
    if( is_constant_evaluated_rt() )
    {
        for( std::size_t i = 0; i < n; ++i ) dest[ i ] = src[ i ];
    }
    else
    {
        std::memcpy( dest, src, n );
    }
}

// memcmp

BOOST_CXX14_CONSTEXPR inline int memcmp_cx( unsigned char const* s1, unsigned char const* s2, std::size_t n )
{
    if( is_constant_evaluated_cx() )
    {
        for( std::size_t i = 0; i < n; ++i )
        {
            if( s1[ i ] < s2[ i ] ) return -1;
            if( s1[ i ] > s2[ i ] ) return +1;
        }

        return 0;
    }
    else
    {
        return std::memcmp( s1, s2, n );
    }
}

BOOST_UUID_CXX14_CONSTEXPR_RT inline int memcmp_rt( unsigned char const* s1, unsigned char const* s2, std::size_t n )
{
    if( is_constant_evaluated_rt() )
    {
        for( std::size_t i = 0; i < n; ++i )
        {
            if( s1[ i ] < s2[ i ] ) return -1;
            if( s1[ i ] > s2[ i ] ) return +1;
        }

        return 0;
    }
    else
    {
        return std::memcmp( s1, s2, n );
    }
}

}}} // namespace boost::uuids::detail

#endif // #ifndef BOOST_UUID_DETAIL_CSTRING_INCLUDED
