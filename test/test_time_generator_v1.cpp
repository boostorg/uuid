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

detail::uuid_clock::time_point get_time_point_v1( uuid const& u )
{
    auto t = u.timestamp_v1();
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

            BOOST_TEST( u1.node_identifier() == u2.node_identifier() );

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

        BOOST_TEST( u1.node_identifier() == node );
        BOOST_TEST_EQ( u1.clock_seq(), 0x2222 );

        set.insert( u1 );

        for( int i = 0; i < N; ++i )
        {
            uuid u2 = generate_and_test( gen );

            BOOST_TEST( u2.node_identifier() == node );

            set.insert( u2 );
        }

        BOOST_TEST_EQ( set.size(), N + 1 );
    }

    return boost::report_errors();
}
