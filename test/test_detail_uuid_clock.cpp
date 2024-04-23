// Copyright 2024 Peter Dimov
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

#include <boost/uuid/detail/uuid_clock.hpp>
#include <boost/core/lightweight_test.hpp>
#include <chrono>
#include <cstdint>

int main()
{
    using boost::uuids::detail::uuid_clock;
    using std::chrono::system_clock;

    // ddaa7790-018c-11ef-a9df-325096b39f47

    std::uint64_t uuid_timestamp = 0x1ef018cddaa7790;

    auto uuid_time = uuid_clock::time_point( uuid_clock::duration( uuid_timestamp ) );

    auto sys_time = uuid_clock::to_sys( uuid_time );

    auto sys_microseconds = std::chrono::duration_cast<std::chrono::microseconds>( sys_time.time_since_epoch() );

    BOOST_TEST_EQ( sys_microseconds.count(), 1713888999776040 ); // 2024-04-23 16:16:39.7760400

    auto uuid_time_2 = uuid_clock::from_sys( sys_time );

    BOOST_TEST_EQ( uuid_time_2.time_since_epoch().count(), uuid_timestamp );

    return boost::report_errors();
}
