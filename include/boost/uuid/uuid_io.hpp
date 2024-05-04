#ifndef BOOST_UUID_UUID_IO_HPP_INCLUDED
#define BOOST_UUID_UUID_IO_HPP_INCLUDED

// Copyright 2009 Andy Tompkins
// Copyright 2024 Peter Dimov
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/detail/to_chars.hpp>
#include <iosfwd>
#include <istream>
#include <locale>
#include <algorithm>
#include <string>

#if defined(_MSC_VER)
#pragma warning(push) // Save warning settings.
#pragma warning(disable : 4996) // Disable deprecated std::ctype<char>::widen, std::copy
#endif

namespace boost {
namespace uuids {

// to_chars

template<class OutputIterator>
OutputIterator to_chars( uuid const& u, OutputIterator out )
{
    char tmp[ 36 ];
    detail::to_chars( u, tmp );

    return std::copy_n( tmp, 36, out );
}

inline bool to_chars( uuid const& u, char* first, char* last ) noexcept
{
    if( last - first < 36 )
    {
        return false;
    }

    detail::to_chars( u, first );
    return true;
}

inline bool to_chars( uuid const& u, wchar_t* first, wchar_t* last ) noexcept
{
    if( last - first < 36 )
    {
        return false;
    }

    detail::to_chars( u, first );
    return true;
}

// operator<<

template<class Ch, class Traits>
std::basic_ostream<Ch, Traits>& operator<<( std::basic_ostream<Ch, Traits>& os, uuid const& u )
{
    char tmp[ 37 ] = {};
    detail::to_chars( u, tmp );

    os << tmp;
    return os;
}

// operator>>

template<class Ch, class Traits>
std::basic_istream<Ch, Traits>& operator>>( std::basic_istream<Ch, Traits>& is, uuid& u )
{
    const typename std::basic_istream<Ch, Traits>::sentry ok( is );

    if( ok )
    {
        unsigned char data[ 16 ];

        using ctype_t = std::ctype<Ch>;
        ctype_t const& ctype = std::use_facet<ctype_t>( is.getloc() );

        Ch xdigits[ 16 ];

        {
            char szdigits[] = "0123456789ABCDEF";
            ctype.widen( szdigits, szdigits + 16, xdigits );
        }

        Ch* const xdigits_end = xdigits + 16;

        Ch c;

        for( std::size_t i = 0; i < u.size() && is; ++i )
        {
            is >> c;
            c = ctype.toupper( c );

            Ch* f = std::find( xdigits, xdigits_end, c );

            if( f == xdigits_end )
            {
                is.setstate( std::ios_base::failbit );
                break;
            }

            unsigned char byte = static_cast<unsigned char>( std::distance( &xdigits[0], f ) );

            is >> c;
            c = ctype.toupper(c);

            f = std::find( xdigits, xdigits_end, c );

            if( f == xdigits_end )
            {
                is.setstate( std::ios_base::failbit );
                break;
            }

            byte <<= 4;
            byte |= static_cast<unsigned char>( std::distance( &xdigits[0], f ) );

            data[ i ] = byte;

            if( is )
            {
                if( i == 3 || i == 5 || i == 7 || i == 9 )
                {
                    is >> c;

                    if( c != is.widen( '-' ) )
                    {
                        is.setstate( std::ios_base::failbit );
                    }
                }
            }
        }

        if( is )
        {
            std::copy( data, data + 16, u.begin() );
        }
    }

    return is;
}

// to_string

inline std::string to_string( uuid const& u )
{
    std::string result( 36, char() );

    // string::data() returns const char* before C++17
    detail::to_chars( u, &result[0] );
    return result;
}

inline std::wstring to_wstring( uuid const& u )
{
    std::wstring result( 36, wchar_t() );

    detail::to_chars( u, &result[0] );
    return result;
}

}} //namespace boost::uuids

#if defined(_MSC_VER)
#pragma warning(pop) // Restore warnings to previous state.
#endif

#endif // BOOST_UUID_UUID_IO_HPP_INCLUDED
