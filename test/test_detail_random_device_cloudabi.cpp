/* boost uuid/detail/random/random_device_cloudabi tests
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
#include <boost/uuid/detail/random/random_device_cloudabi.hpp>

int main(int, char*[])
{
#if BOOST_PLAT_CLOUDABI
    using boost::uuids::detail::random::random_device_cloudabi;
#if !defined(BOOST_UUID_TESTSTUBS)
    // happy path
    random_device_cloudabi<unsigned int> gen;
    BOOST_TEST_NE(gen(), gen());  // well, it's incredibly unlikely anyway
    BOOST_TEST_EQ(gen.name(), "cloudabi");
#else
    // "sad" path, using stubs
    using boost::uuids::entropy_error;

    // TODO: how to force system call cloudabi_sys_random_get to return an error
#endif
#endif

    return boost::report_errors();
}
