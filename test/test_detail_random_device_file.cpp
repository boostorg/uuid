/* boost uuid/detail/random/random_device_file tests
 *
 * Copyright (c) 2017 James E. King III
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENCE_1_0.txt)
 *
 * $Id$
 */

#include <boost/core/ignore_unused.hpp>
#include <boost/detail/lightweight_test.hpp>
#include <boost/uuid/detail/random/random_device_file.hpp>

int main(int, char*[])
{
#if !defined(BOOST_WINDOWS)
    using boost::uuids::detail::random::random_device_file;
#if !defined(BOOST_UUID_TESTSTUBS)
    // happy path, using real /dev/urandom
    random_device_file<unsigned int> gen;
    BOOST_TEST_NE(gen(), gen());  // well, it's incredibly unlikely anyway
    BOOST_TEST_EQ(gen.name(), "/dev/urandom");
#else
    // "sad" path, using stubs
    using boost::uuids::entropy_error;

    // TODO: how to force open of /dev/urandom to fail
    // TODO: how to force read of /dev/urandom to fail
    // TODO: how to force read of /dev/urandom to be smaller than requested
#endif
#endif

    return boost::report_errors();
}
