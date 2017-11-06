/* boost uuid/detail/random/random_device various tests
 *
 * Copyright (c) 2010 Steven Watanabe
 * Copyright (c) 2017 James E. King III
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENCE_1_0.txt)
 *
 * $Id$
 */

#include <boost/cstdint.hpp>
#include <boost/detail/lightweight_test.hpp>
#include <boost/uuid/detail/random/random_device.hpp>
#include <limits>
#include <string.h>

template<class Type>
void test_random_device()
{
    BOOST_UUID_DETAIL_MAKE_RANDOM_DEVICE_TYPE(Type, test_random_device);
    test_random_device rng;

    BOOST_TEST_EQ((std::numeric_limits<Type>::min)(), rng.min());
    BOOST_TEST_EQ((std::numeric_limits<Type>::max)(), rng.max());

    size_t dupes = 0;
    typename test_random_device::result_type prev = 0;
    for (int i = 0; i < 100; ++i)
    {
        typename test_random_device::result_type val = rng();
        BOOST_TEST_GE(val, (std::numeric_limits<Type>::min)());
        BOOST_TEST_LE(val, (std::numeric_limits<Type>::max)());
        if (prev == val) { ++dupes; }
        prev = val;
    }
    BOOST_TEST_NE(dupes, 100);
}

struct a_chunk_of_entropy
{
    boost::uint8_t entropy[64 * 1024];
};

void test_huge_entropy()
{
    BOOST_UUID_DETAIL_MAKE_RANDOM_DEVICE_TYPE(a_chunk_of_entropy, test_random_device);
    test_random_device rng;

    a_chunk_of_entropy e1 = rng();
    a_chunk_of_entropy e2 = rng();
    BOOST_TEST_NE(0, memcmp(e1.entropy, e2.entropy, sizeof(a_chunk_of_entropy)));
}

int main(int, char*[])
{
    test_random_device<char>();
    test_random_device<boost::uint8_t>();
    test_random_device<short>();
    test_random_device<boost::uint16_t>();
    test_random_device<int>();
    test_random_device<boost::uint32_t>();
    test_random_device<boost::uint64_t>();

    test_huge_entropy();

    return boost::report_errors();
}
