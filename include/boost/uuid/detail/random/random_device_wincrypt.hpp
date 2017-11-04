/* boost uuid/detail/random_device_wincrypt header file
 *
 * Copyright Jens Maurer 2000
 * Copyright 2007 Andy Tompkins.
 * Copyright Steven Watanabe 2010-2011
 * Copyright 2017 James E. King, III
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * $Id$
 *
 * Revision history
 *  2017-09-14  wincrypt implementation moved here
 */

#ifndef BOOST_UUID_RANDOM_DETAIL_RANDOM_DEVICE_WINCRYPT
#define BOOST_UUID_RANDOM_DETAIL_RANDOM_DEVICE_WINCRYPT

#include <boost/config.hpp>

#ifdef BOOST_HAS_PRAGMA_ONCE
#pragma once
#endif

#if defined(BOOST_WINDOWS)

#include <boost/detail/winapi/config.hpp>

#if BOOST_WINAPI_PARTITION_DESKTOP || BOOST_WINAPI_PARTITION_SYSTEM

#include <boost/core/ignore_unused.hpp>
#include <boost/winapi/crypt.hpp>
#include <boost/winapi/detail/cast_ptr.hpp>
#include <boost/winapi/get_last_error.hpp>
#include <boost/noncopyable.hpp>
#include <boost/throw_exception.hpp>
#include <boost/uuid/entropy_error.hpp>
#include <string>

#if !defined(BOOST_UUID_RANDOM_DEVICE_NO_LIB)
#   if defined(_WIN32_WCE)
#      define BOOST_LIB_NAME "coredll"
#   else
#      define BOOST_LIB_NAME "advapi32"
#   endif
#   define BOOST_AUTO_LINK_NOMANGLE
#   include <boost/config/auto_link.hpp>
#endif

namespace boost {
namespace uuids {
namespace detail {
namespace random {

template<class Entropy>
class random_device_wincrypt : private noncopyable
{
public:
    typedef Entropy result_type;

    random_device_wincrypt()
      : hProv_(NULL), calls_(0)
    {
        if (!
#if defined(BOOST_NO_ANSI_APIS)
        boost::winapi::CryptAcquireContextW
#else
        boost::winapi::CryptAcquireContextA
#endif
           (&hProv_,
            NULL,
            NULL,
            boost::winapi::PROV_RSA_FULL_,
            boost::winapi::CRYPT_VERIFYCONTEXT_ | boost::winapi::CRYPT_SILENT_))
        {
            BOOST_THROW_EXCEPTION(entropy_error(
                boost::winapi::GetLastError(), "CryptAcquireContextW"));
        }
    }

    ~random_device_wincrypt() BOOST_NOEXCEPT
    {
        if (hProv_)
        {
            ignore_unused(boost::winapi::CryptReleaseContext(hProv_, 0));
        }
    }

    result_type operator()()
    {
        ++calls_;
        result_type result;
        if (!boost::winapi::CryptGenRandom(hProv_, sizeof(result), boost::winapi::detail::cast_ptr(&result)))
        {
            BOOST_THROW_EXCEPTION(entropy_error(
                boost::winapi::GetLastError(), "CryptGenRandom"));
        }
        return result;
    }

    std::string name() const
    {
        return "wincrypt";
    }

private:
    boost::winapi::HCRYPTPROV_ hProv_;
    boost::uint64_t calls_;
};

} // random
} // detail
} // uuids
} // boost

#endif // BOOST_WINAPI_PARTITION_DESKTOP || BOOST_WINAPI_PARTITION_SYSTEM
#endif // BOOST_WINDOWS
#endif // BOOST_UUID_RANDOM_DETAIL_RANDOM_DEVICE_WINCRYPT
