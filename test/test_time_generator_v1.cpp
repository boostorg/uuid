// Copyright 2024 Peter Dimov
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

#include <boost/uuid/time_generator_v1.hpp>
#include <boost/uuid/time_generator_v1.hpp>
#include <boost/core/lightweight_test.hpp>
#include <atomic>
#include <set>
#include <cstdint>
#include <cstring>

using namespace boost::uuids;

time_generator_v1::node_type get_node( uuid const& u )
{
    time_generator_v1::node_type node = {};

    std::memcpy( node.data(), u.data + 10, 6 );
    return node;
}

std::uint16_t get_clock_seq( uuid const& u )
{
    return detail::load_big_u16( u.data + 8 ) & 0x3FFF;
}

int main()
{
    int const N = 1024;

    {
        std::set<uuid> set;

        time_generator_v1 gen;

        uuid u1 = gen();

        BOOST_TEST_EQ( u1.variant(), uuid::variant_rfc_4122 );
        BOOST_TEST_EQ( u1.version(), uuid::version_time_based );

        set.insert( u1 );

        for( int i = 0; i < N; ++i )
        {
            uuid u2 = gen();

            BOOST_TEST_EQ( u2.variant(), uuid::variant_rfc_4122 );
            BOOST_TEST_EQ( u2.version(), uuid::version_time_based );
            BOOST_TEST( get_node( u1 ) == get_node( u2 ) );

            set.insert( u2 );
        }

        BOOST_TEST_EQ( set.size(), N + 1 );
    }

    {
        std::set<uuid> set;

        time_generator_v1::node_type node{{ 0x01, 0x02, 0x03, 0x04, 0x05, 0x06 }};
        std::atomic< time_generator_v1::state_type > state{{ 0, 0x2222 }};

        time_generator_v1 gen( node, state );

        uuid u1 = gen();

        BOOST_TEST_EQ( u1.variant(), uuid::variant_rfc_4122 );
        BOOST_TEST_EQ( u1.version(), uuid::version_time_based );
        BOOST_TEST( get_node( u1 ) == node );
        BOOST_TEST_EQ( get_clock_seq( u1 ), 0x2222 );

        set.insert( u1 );

        for( int i = 0; i < N; ++i )
        {
            uuid u2 = gen();

            BOOST_TEST_EQ( u2.variant(), uuid::variant_rfc_4122 );
            BOOST_TEST_EQ( u2.version(), uuid::version_time_based );
            BOOST_TEST( get_node( u1 ) == get_node( u2 ) );

            set.insert( u2 );
        }

        BOOST_TEST_EQ( set.size(), N + 1 );
    }

    return boost::report_errors();
}
