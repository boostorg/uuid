// Copyright 2026 Peter Dimov
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

#define BOOST_UUID_REPORT_IMPLEMENTATION

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/core/lightweight_test.hpp>
#include <string>
#include <sstream>
#include <iomanip>

using namespace boost::uuids;

int main()
{
    uuid const u1 = {};

    {
        std::ostringstream os;
        os << u1;
        BOOST_TEST_EQ( os.str(), "00000000-0000-0000-0000-000000000000" );
    }

    {
        std::ostringstream os;
        os << std::uppercase << u1;
        BOOST_TEST_EQ( os.str(), "00000000-0000-0000-0000-000000000000" );
    }

    {
        std::ostringstream os;
        os << std::left << std::setfill( '*' ) << std::setw( 40 ) << u1;
        BOOST_TEST_EQ( os.str(), "00000000-0000-0000-0000-000000000000****" );
    }

    {
        std::ostringstream os;
        os << std::right << std::setfill( '*' ) << std::setw( 40 ) << u1;
        BOOST_TEST_EQ( os.str(), "****00000000-0000-0000-0000-000000000000" );
    }

    {
        std::ostringstream os;
        os << std::uppercase << std::left << std::setfill( '*' ) << std::setw( 40 ) << u1;
        BOOST_TEST_EQ( os.str(), "00000000-0000-0000-0000-000000000000****" );
    }

    {
        std::ostringstream os;
        os << std::uppercase << std::right << std::setfill( '*' ) << std::setw( 40 ) << u1;
        BOOST_TEST_EQ( os.str(), "****00000000-0000-0000-0000-000000000000" );
    }

    uuid const u2 = {{ 0xAB, 0xCD, 0xEF, 0x12, 0x34, 0x56, 0x78, 0x90, 0x12, 0x34, 0x56, 0x78, 0x90, 0xAB, 0xCD, 0xEF }};

    {
        std::ostringstream os;
        os << u2;
        BOOST_TEST_EQ( os.str(), "abcdef12-3456-7890-1234-567890abcdef" );
    }

    {
        std::ostringstream os;
        os << std::uppercase << u2;
        BOOST_TEST_EQ( os.str(), "ABCDEF12-3456-7890-1234-567890ABCDEF" );
    }

    {
        std::ostringstream os;
        os << std::left << std::setfill( '*' ) << std::setw( 40 ) << u2;
        BOOST_TEST_EQ( os.str(), "abcdef12-3456-7890-1234-567890abcdef****" );
    }

    {
        std::ostringstream os;
        os << std::right << std::setfill( '*' ) << std::setw( 40 ) << u2;
        BOOST_TEST_EQ( os.str(), "****abcdef12-3456-7890-1234-567890abcdef" );
    }

    {
        std::ostringstream os;
        os << std::uppercase << std::left << std::setfill( '*' ) << std::setw( 40 ) << u2;
        BOOST_TEST_EQ( os.str(), "ABCDEF12-3456-7890-1234-567890ABCDEF****" );
    }

    {
        std::ostringstream os;
        os << std::uppercase << std::right << std::setfill( '*' ) << std::setw( 40 ) << u2;
        BOOST_TEST_EQ( os.str(), "****ABCDEF12-3456-7890-1234-567890ABCDEF" );
    }

    return boost::report_errors();
}
