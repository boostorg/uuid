// Copyright 2024 Peter Dimov
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

#include <boost/uuid/time_generator_v7.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/core/lightweight_test.hpp>
#include <chrono>

using namespace boost::uuids;

int main()
{
    time_generator_v7 gen;

    auto now = std::chrono::system_clock::now();
    auto now_in_ms = std::chrono::time_point_cast<std::chrono::milliseconds>( now );

    uuid u1 = gen( now );
    BOOST_TEST( u1.time_point_v7() == now_in_ms );

    uuid u2 = gen( now );
    BOOST_TEST( u2.time_point_v7() == now_in_ms );

    BOOST_TEST_LT( u1, u2 );

    return boost::report_errors();
}
