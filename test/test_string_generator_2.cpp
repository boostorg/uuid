// Copyright 2025 Peter Dimov
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

#include <boost/uuid/string_generator.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/core/lightweight_test.hpp>

using namespace boost::uuids;

template<class Ch> void test( uuid const& expected, Ch const* str )
{
    BOOST_TEST_EQ( string_generator()( str ), expected );
}

int main()
{
    uuid const exp = {{ 0x12, 0x34, 0x56, 0x78, 0x90, 0xab, 0xcd, 0xef, 0x12, 0x34, 0x56, 0x78, 0x90, 0xab, 0xcd, 0xef }};

    test( exp,   "12345678-90AB-CDEF-1234-567890abcdef" );
    test( exp,  L"12345678-90AB-CDEF-1234-567890abcdef" );
    test( exp,  u"12345678-90AB-CDEF-1234-567890abcdef" );
    test( exp,  U"12345678-90AB-CDEF-1234-567890abcdef" );
    test( exp, u8"12345678-90AB-CDEF-1234-567890abcdef" );

    test( exp,   "{12345678-90AB-CDEF-1234-567890abcdef}" );
    test( exp,  L"{12345678-90AB-CDEF-1234-567890abcdef}" );
    test( exp,  u"{12345678-90AB-CDEF-1234-567890abcdef}" );
    test( exp,  U"{12345678-90AB-CDEF-1234-567890abcdef}" );
    test( exp, u8"{12345678-90AB-CDEF-1234-567890abcdef}" );

    test( exp,   "1234567890ABCDEF1234567890abcdef" );
    test( exp,  L"1234567890ABCDEF1234567890abcdef" );
    test( exp,  u"1234567890ABCDEF1234567890abcdef" );
    test( exp,  U"1234567890ABCDEF1234567890abcdef" );
    test( exp, u8"1234567890ABCDEF1234567890abcdef" );

    test( exp,   "{1234567890ABCDEF1234567890abcdef}" );
    test( exp,  L"{1234567890ABCDEF1234567890abcdef}" );
    test( exp,  u"{1234567890ABCDEF1234567890abcdef}" );
    test( exp,  U"{1234567890ABCDEF1234567890abcdef}" );
    test( exp, u8"{1234567890ABCDEF1234567890abcdef}" );

    return boost::report_errors();
}
