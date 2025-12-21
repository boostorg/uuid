#ifndef BOOST_UUID_DETAIL_FROM_CHARS_HPP_INCLUDED
#define BOOST_UUID_DETAIL_FROM_CHARS_HPP_INCLUDED

// Copyright 2025 Peter Dimov
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

#include <boost/uuid/uuid.hpp>
#include <boost/config.hpp>

namespace boost {
namespace uuids {
namespace detail {

constexpr char const* from_chars_digits( char const* ) noexcept
{
    return "0123456789ABCDEFabcdef-";
}

constexpr wchar_t const* from_chars_digits( wchar_t const* ) noexcept
{
    return L"0123456789ABCDEFabcdef-";
}

constexpr char16_t const* from_chars_digits( char16_t const* ) noexcept
{
    return u"0123456789ABCDEFabcdef-";
}

constexpr char32_t const* from_chars_digits( char32_t const* ) noexcept
{
    return U"0123456789ABCDEFabcdef-";
}

#if defined(__cpp_char8_t) && __cpp_char8_t >= 201811L

constexpr char8_t const* from_chars_digits( char8_t const* ) noexcept
{
    return u8"0123456789ABCDEFabcdef-";
}

#endif

template<class Ch>
BOOST_CXX14_CONSTEXPR inline
unsigned char from_chars_digit_value( Ch ch )
{
    constexpr Ch const* digits = detail::from_chars_digits( static_cast<Ch const*>( nullptr ) );

    std::size_t i = 0;

    for( ; i < 16; ++i )
    {
        if( digits[ i ] == ch ) return static_cast< unsigned char >( i );
    }

    for( ; i < 22; ++i )
    {
        if( digits[ i ] == ch ) return static_cast< unsigned char >( i - 6 );
    }

    return 255;
}

template<class Ch>
BOOST_CXX14_CONSTEXPR inline
unsigned char from_chars_is_dash( Ch ch )
{
    constexpr Ch const* digits = detail::from_chars_digits( static_cast<Ch const*>( nullptr ) );

    return ch == digits[ 22 ];
}

} // namespace detail

enum class from_chars_error
{
    none = 0,

    unexpected_end_of_input,
    hex_digit_expected,
    dash_expected
};

template<class Ch> struct from_chars_result
{
    Ch const* ptr;
    from_chars_error ec;
};

template<class Ch>
BOOST_CXX14_CONSTEXPR inline
from_chars_result<Ch> from_chars( Ch const* first, Ch const* last, uuid& u ) noexcept
{
    u = {};

    for( std::size_t i = 0; i < 16; ++i )
    {
        if( first == last )
        {
            return { first, from_chars_error::unexpected_end_of_input };
        }

        unsigned char v1 = detail::from_chars_digit_value( *first );

        if( v1 == 255 )
        {
            return { first, from_chars_error::hex_digit_expected };
        }

        ++first;

        if( first == last )
        {
            return { first, from_chars_error::unexpected_end_of_input };
        }

        unsigned char v2 = detail::from_chars_digit_value( *first );

        if( v2 == 255 )
        {
            return { first, from_chars_error::hex_digit_expected };
        }

        ++first;

        u.data()[ i ] = static_cast<unsigned char>( ( v1 << 4 ) + v2 );

        if( i == 3 || i == 5 || i == 7 || i == 9 )
        {
            if( first == last )
            {
                return { first, from_chars_error::unexpected_end_of_input };
            }

            if( !detail::from_chars_is_dash( *first ) )
            {
                return { first, from_chars_error::dash_expected };
            }

            ++first;
        }
    }

    return { first, from_chars_error::none };
}

}} //namespace boost::uuids

#endif // BOOST_UUID_DETAIL_TO_CHARS_HPP_INCLUDED
