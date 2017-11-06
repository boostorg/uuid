/* boost uuid/detail/random_device_cloudabi header file
*
* Copyright 2017 James E. King, III
*
* Distributed under the Boost Software License, Version 1.0. (See
* accompanying file LICENSE_1_0.txt or copy at
* http://www.boost.org/LICENSE_1_0.txt)
*
* $Id$
*/

#ifndef BOOST_UUID_RANDOM_DETAIL_RANDOM_DEVICE_CLOUDABI
#define BOOST_UUID_RANDOM_DETAIL_RANDOM_DEVICE_CLOUDABI

#include <boost/config.hpp>
#include <boost/noncopyable.hpp>
#include <boost/predef/platform/cloudabi.h>
#include <boost/throw_exception.hpp>
#include <boost/uuid/entropy_error.hpp>

#ifdef BOOST_HAS_PRAGMA_ONCE
#pragma once
#endif

#if BOOST_PLAT_CLOUDABI

#include <cloudabi_syscalls.h>

namespace boost {
namespace uuids {
namespace detail {
namespace random {

template<class Entropy>
class random_device_cloudabi : private noncopyable
{
public:
    typedef Entropy result_type;

    result_type operator()()
    {
        result_type result;
        cloudabi_errno_t err = cloudabi_sys_random_get(&result, sizeof(result_type));
        if (err)
        {
            BOOST_THROW_EXCEPTION(entropy_error(err, "cloudabi_sys_random_get"));
        }

        return result;
    }

    std::string name() const
    {
        return "cloudabi";
    }
};

} // random
} // detail
} // uuids
} // boost

#endif // BOOST_PLAT_CLOUDABI
#endif // BOOST_UUID_RANDOM_DETAIL_RANDOM_DEVICE_CLOUDABI
