// Copyright 2024 Peter Dimov
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

#include <boost/uuid/time_generator_v1.hpp>
#include <boost/uuid/detail/uuid_clock.hpp>
#include <boost/core/lightweight_test.hpp>
#include <atomic>
#include <chrono>
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

std::uint64_t get_timestamp_v1( uuid const& u )
{
    std::uint32_t time_low = detail::load_big_u32( u.data + 0 );
    std::uint16_t time_mid = detail::load_big_u16( u.data + 4 );
    std::uint16_t time_hi = detail::load_big_u16( u.data + 6 ) & 0x0FFF;

    return time_low | static_cast<std::uint64_t>( time_mid ) << 32 | static_cast<std::uint64_t>( time_hi ) << 48;
}

detail::uuid_clock::time_point get_time_point_v1( uuid const& u )
{
    auto t = get_timestamp_v1( u );
    auto d = detail::uuid_clock::duration( t );

    return detail::uuid_clock::time_point( d );
}

uuid generate_and_test( time_generator_v1& gen )
{
    auto sys_before = std::chrono::time_point_cast<detail::uuid_clock::duration>( std::chrono::system_clock::now() );

    uuid u = gen();

    BOOST_TEST_EQ( u.variant(), uuid::variant_rfc_4122 );
    BOOST_TEST_EQ( u.version(), uuid::version_time_based );

    auto sys_after = std::chrono::time_point_cast<detail::uuid_clock::duration>( std::chrono::system_clock::now() );

    auto uuid_time_point = get_time_point_v1( u );

    auto sys_time_point = detail::uuid_clock::to_sys( uuid_time_point );

    BOOST_TEST( sys_before <= sys_time_point );
    BOOST_TEST( sys_time_point <= sys_after );

    return u;
}

int main()
{
    int const N = 1024;

    {
        std::set<uuid> set;

        time_generator_v1 gen;

        uuid u1 = generate_and_test( gen );

        set.insert( u1 );

        for( int i = 0; i < N; ++i )
        {
            uuid u2 = generate_and_test( gen );

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

        uuid u1 = generate_and_test( gen );

        BOOST_TEST( get_node( u1 ) == node );
        BOOST_TEST_EQ( get_clock_seq( u1 ), 0x2222 );

        set.insert( u1 );

        for( int i = 0; i < N; ++i )
        {
            uuid u2 = generate_and_test( gen );

            BOOST_TEST( get_node( u1 ) == get_node( u2 ) );

            set.insert( u2 );
        }

        BOOST_TEST_EQ( set.size(), N + 1 );
    }

    return boost::report_errors();
}
