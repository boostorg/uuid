// Copyright 2024 Peter Dimov
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

#include <boost/uuid/uuid.hpp>
#include <boost/core/lightweight_test.hpp>
#include <boost/config/pragma_message.hpp>
#include <cstddef>

#if !defined(__GNUC__)

BOOST_PRAGMA_MESSAGE( "Test skipped, __GNUC__ is not defined" )
int main() {}

#else

using namespace boost::uuids;

struct __attribute__((packed)) X
{
    uuid a;
    unsigned char b;
    unsigned c;
    unsigned char d;
};

int main()
{
    BOOST_TEST_EQ( offsetof(X, c), 17 );
    BOOST_TEST_EQ( sizeof(X), 22 );

    return boost::report_errors();
}

#endif
