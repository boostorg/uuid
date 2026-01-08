#ifndef BOOST_UUID_DETAIL_THROW_INVALID_UUID_INCLUDED
#define BOOST_UUID_DETAIL_THROW_INVALID_UUID_INCLUDED

// Copyright 2025 Peter Dimov
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

#include <boost/uuid/detail/from_chars_result.hpp>
#include <boost/throw_exception.hpp>
#include <boost/config.hpp>
#include <stdexcept>
#include <cstdio>
#include <cstddef>

namespace boost {
namespace uuids {
namespace detail {

BOOST_CXX14_CONSTEXPR inline char const* fc_error_to_string( from_chars_error err ) noexcept
{
    switch( err )
    {
    case from_chars_error::none: return "no error";
    case from_chars_error::unexpected_end_of_input: return "unexpected end of input";
    case from_chars_error::hex_digit_expected: return "hex digit expected";
    case from_chars_error::dash_expected: return "dash expected";
    case from_chars_error::closing_brace_expected: return "closing brace expected";
    case from_chars_error::unexpected_extra_input: return "unexpected extra input";
    default: return "unknown error";
    }
}

BOOST_NORETURN inline void throw_invalid_uuid( std::ptrdiff_t pos, from_chars_error err )
{
    char buffer[ 128 ];
    std::snprintf( buffer, sizeof( buffer ), "Invalid UUID string at position %td: %s", pos, fc_error_to_string( err ) );

    BOOST_THROW_EXCEPTION( std::runtime_error( buffer ) );
}

}}} // namespace boost::uuids::detail

#endif // #ifndef BOOST_UUID_DETAIL_THROW_INVALID_UUID_INCLUDED
