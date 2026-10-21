// Copyright 2026 Peter Dimov
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

#define BOOST_UUID_REPORT_IMPLEMENTATION

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/core/lightweight_test.hpp>
#include <boost/config.hpp>
#include <boost/config/pragma_message.hpp>

#if defined(BOOST_NO_CXX20_HDR_FORMAT)

BOOST_PRAGMA_MESSAGE( "Test skipped, because BOOST_NO_CXX20_HDR_FORMAT is defined" )
int main() {}

#else

#include <format>

using namespace boost::uuids;

int main()
{
    uuid const u1 = {};

    BOOST_TEST_EQ( std::format( "{}", u1 ), "00000000-0000-0000-0000-000000000000" );

    BOOST_TEST_EQ( std::format( "{:*<40}", u1 ), "00000000-0000-0000-0000-000000000000****" );
    BOOST_TEST_EQ( std::format( "{:*>40}", u1 ), "****00000000-0000-0000-0000-000000000000" );
    BOOST_TEST_EQ( std::format( "{:*^40}", u1 ), "**00000000-0000-0000-0000-000000000000**" );

    uuid const u2 = {{ 0xAB, 0xCD, 0xEF, 0x12, 0x34, 0x56, 0x78, 0x90, 0x12, 0x34, 0x56, 0x78, 0x90, 0xAB, 0xCD, 0xEF }};

    BOOST_TEST_EQ( std::format( "{}", u2 ), "abcdef12-3456-7890-1234-567890abcdef" );

    BOOST_TEST_EQ( std::format( "{:*<40}", u2 ), "abcdef12-3456-7890-1234-567890abcdef****" );
    BOOST_TEST_EQ( std::format( "{:*>40}", u2 ), "****abcdef12-3456-7890-1234-567890abcdef" );
    BOOST_TEST_EQ( std::format( "{:*^40}", u2 ), "**abcdef12-3456-7890-1234-567890abcdef**" );

    return boost::report_errors();
}

#endif // defined(BOOST_NO_CXX20_HDR_FORMAT)
