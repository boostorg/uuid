/* boost uuid/detail/random_device_bcrypt header file
*
* Copyright 2017 James E. King, III
*
* Distributed under the Boost Software License, Version 1.0. (See
* accompanying file LICENSE_1_0.txt or copy at
* http://www.boost.org/LICENSE_1_0.txt)
*
* $Id$
*
* Revision history
*  2017-09-14  initial bcrypt implementation
*/

#ifndef BOOST_UUID_RANDOM_DETAIL_RANDOM_DEVICE_BCRYPT
#define BOOST_UUID_RANDOM_DETAIL_RANDOM_DEVICE_BCRYPT

#include <boost/config.hpp>

#ifdef BOOST_HAS_PRAGMA_ONCE
#pragma once
#endif

#if defined(BOOST_WINDOWS)

#include <boost/winapi/config.hpp>

#if BOOST_WINAPI_PARTITION_APP_SYSTEM && \
        (defined(BOOST_WINAPI_IS_MINGW_W64) || (BOOST_USE_WINAPI_VERSION >= BOOST_WINAPI_VERSION_WIN6))

#define BOOST_UUID_RANDOM_DEVICE_BCRYPT_AVAILABLE

#include <boost/core/ignore_unused.hpp>
#include <boost/winapi/bcrypt.hpp>
#include <boost/winapi/detail/cast_ptr.hpp>
#include <boost/winapi/get_last_error.hpp>
#include <boost/noncopyable.hpp>
#include <boost/throw_exception.hpp>
#include <boost/uuid/entropy_error.hpp>
#include <string>

#if !defined(BOOST_UUID_RANDOM_DEVICE_NO_LIB)
#   define BOOST_LIB_NAME "bcrypt"
#   define BOOST_AUTO_LINK_NOMANGLE
#   include <boost/config/auto_link.hpp>
#endif

namespace boost {
namespace uuids {
namespace detail {
namespace random {

template<class Entropy>
class random_device_bcrypt : private noncopyable
{
public:
    typedef Entropy result_type;

    random_device_bcrypt()
        : hProv_(NULL)
    {
        boost::winapi::NTSTATUS_ status =
            boost::winapi::BCryptOpenAlgorithmProvider(
                &hProv_, 
                boost::winapi::BCRYPT_RNG_ALGORITHM_,
                NULL,
                0);

        if (status)
        {
            BOOST_THROW_EXCEPTION(entropy_error(
                status, "BCryptOpenAlgorithmProvider"));
        }
    }

    ~random_device_bcrypt() BOOST_NOEXCEPT
    {
        if (hProv_)
        {
            ignore_unused(boost::winapi::BCryptCloseAlgorithmProvider(hProv_, 0));
        }
    }

    result_type operator()()
    {
        result_type result;

        boost::winapi::NTSTATUS_ status =
            boost::winapi::BCryptGenRandom(
                hProv_,
                reinterpret_cast<boost::winapi::PUCHAR_>(&result),
                sizeof(result),
                0);

        if (status)
        {
            BOOST_THROW_EXCEPTION(entropy_error(
                status, "BCryptGenRandom"));
        }

        return result;
    }

    std::string name() const
    {
        return "bcrypt";
    }

private:
    boost::winapi::BCRYPT_ALG_HANDLE_ hProv_;
};

} // random
} // detail
} // uuids
} // boost

#endif // BOOST_WINAPI_PARTITION_APP_SYSTEM etc... && defined(BOOST_UUID_RANDOM_DEVICE_FORCE_BCRYPT)
#endif // BOOST_WINDOWS
#endif // BOOST_UUID_RANDOM_DETAIL_RANDOM_DEVICE_BCRYPT
