#ifndef BOOST_UUID_UUID_IO_HPP_INCLUDED
#define BOOST_UUID_UUID_IO_HPP_INCLUDED

// Boost uuid_io.hpp header file  ----------------------------------------------//

// Copyright 2009 Andy Tompkins.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// https://www.boost.org/LICENSE_1_0.txt)

#include <boost/uuid/uuid.hpp>
#include <iosfwd>
#include <istream>
#include <locale>
#include <algorithm>

#if defined(_MSC_VER)
#pragma warning(push) // Save warning settings.
#pragma warning(disable : 4996) // Disable deprecated std::ctype<char>::widen, std::copy
#endif

namespace boost {
namespace uuids {

namespace detail {

inline char to_char( unsigned i )
{
    if( i <= 9 )
    {
        return static_cast<char>( '0' + i );
    }
    else
    {
        // 'a'..'f' are actually consecutive in EBCDIC
        return static_cast<char>( 'a' + ( i - 10 ) );
    }
}

inline wchar_t to_wchar( unsigned i )
{
    if( i <= 9 )
    {
        return static_cast<wchar_t>( L'0' + i );
    }
    else
    {
        return static_cast<wchar_t>( L'a' + ( i - 10 ) );
    }
}

inline char* to_chars( uuid const& u, char* out )
{
    std::size_t i = 0;

    for( uuid::const_iterator it_data = u.begin(); it_data != u.end(); ++it_data, ++i )
    {
        const unsigned hi = ((*it_data) >> 4) & 0x0F;
        *out++ = detail::to_char( hi );

        const unsigned lo = (*it_data) & 0x0F;
        *out++ = detail::to_char( lo );

        if( i == 3 || i == 5 || i == 7 || i == 9 )
        {
            *out++ = '-';
        }
    }

    return out;
}

inline wchar_t* to_chars( uuid const& u, wchar_t* out )
{
    std::size_t i = 0;

    for( uuid::const_iterator it_data = u.begin(); it_data != u.end(); ++it_data, ++i )
    {
        const unsigned hi = ((*it_data) >> 4) & 0x0F;
        *out++ = detail::to_wchar( hi );

        const unsigned lo = (*it_data) & 0x0F;
        *out++ = detail::to_wchar( lo );

        if( i == 3 || i == 5 || i == 7 || i == 9 )
        {
            *out++ = L'-';
        }
    }

    return out;
}

} // namespace detail

template<class OutputIterator>
OutputIterator to_chars( uuid const& u, OutputIterator out )
{
    char tmp[ 36 ];
    detail::to_chars( u, tmp );

    return std::copy_n( tmp, 36, out );
}

inline bool to_chars( uuid const& u, char* first, char* last )
{
    if( last - first < 36 )
    {
        return false;
    }

    detail::to_chars( u, first );
    return true;
}

inline bool to_chars( uuid const& u, wchar_t* first, wchar_t* last )
{
    if( last - first < 36 )
    {
        return false;
    }

    detail::to_chars( u, first );
    return true;
}

template<class Ch, class Traits>
std::basic_ostream<Ch, Traits>& operator<<( std::basic_ostream<Ch, Traits>& os, uuid const& u )
{
    char tmp[ 37 ] = {};
    detail::to_chars( u, tmp );

    os << tmp;
    return os;
}

template<class Ch, class Traits>
std::basic_istream<Ch, Traits>& operator>>( std::basic_istream<Ch, Traits>& is, uuid& u )
{
    const typename std::basic_istream<Ch, Traits>::sentry ok( is );

    if( ok )
    {
        unsigned char data[ 16 ];

        typedef std::ctype<Ch> ctype_t;
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

inline std::string to_string( uuid const& u )
{
    std::string result( 36, char() );

    // string::data() returns const char* before C++17
    to_chars( u, &result[0] );
    return result;
}

inline std::wstring to_wstring( uuid const& u )
{
    std::wstring result( 36, wchar_t() );

    to_chars( u, &result[0] );
    return result;
}

}} //namespace boost::uuids

#if defined(_MSC_VER)
#pragma warning(pop) // Restore warnings to previous state.
#endif

#endif // BOOST_UUID_UUID_IO_HPP_INCLUDED
