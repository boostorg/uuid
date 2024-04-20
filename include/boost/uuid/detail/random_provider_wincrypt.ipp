/* boost uuid/detail/random_provider_wincrypt implementation
*
* Copyright Jens Maurer 2000
* Copyright 2007 Andy Tompkins.
* Copyright Steven Watanabe 2010-2011
* Copyright 2017 James E. King III
*
* Distributed under the Boost Software License, Version 1.0. (See
* accompanying file LICENSE_1_0.txt or copy at
* https://www.boost.org/LICENSE_1_0.txt)
*
* $Id$
*/

#include <cstddef>
#include <boost/config.hpp>
#include <boost/core/ignore_unused.hpp>
#include <boost/move/core.hpp>
#include <boost/numeric/conversion/cast.hpp>
#include <boost/winapi/crypt.hpp>
#include <boost/winapi/get_last_error.hpp>
#include <boost/throw_exception.hpp>

#if defined(_MSC_VER) && !defined(BOOST_UUID_DISABLE_AUTO_LINK) && !defined(BOOST_UUID_RANDOM_PROVIDER_NO_LIB)
# if defined(_WIN32_WCE)
#   pragma comment(lib, "coredll.lib")
# else
#   pragma comment(lib, "advapi32.lib")
# endif
#endif

namespace boost {
namespace uuids {
namespace detail {

class random_provider_base
{
    BOOST_MOVABLE_BUT_NOT_COPYABLE(random_provider_base)

public:
    random_provider_base()
        : hProv_(0)
    {
        boost::winapi::BOOL_ res = boost::winapi::CryptAcquireContextW(
            &hProv_,
            NULL,
            NULL,
            boost::winapi::PROV_RSA_FULL_,
            boost::winapi::CRYPT_VERIFYCONTEXT_ | boost::winapi::CRYPT_SILENT_);
        if (BOOST_UNLIKELY(!res))
        {
            boost::winapi::DWORD_ err = boost::winapi::GetLastError();
            BOOST_THROW_EXCEPTION(entropy_error(err, "CryptAcquireContext"));
        }
    }

    random_provider_base(BOOST_RV_REF(random_provider_base) that) BOOST_NOEXCEPT : hProv_(that.hProv_)
    {
        that.hProv_ = 0;
    }

    random_provider_base& operator= (BOOST_RV_REF(random_provider_base) that) BOOST_NOEXCEPT
    {
        destroy();
        hProv_ = that.hProv_;
        that.hProv_ = 0;
        return *this;
    }

    ~random_provider_base() BOOST_NOEXCEPT
    {
        destroy();
    }

    //! Obtain entropy and place it into a memory location
    //! \param[in]  buf  the location to write entropy
    //! \param[in]  siz  the number of bytes to acquire
    void get_random_bytes(void *buf, std::size_t siz)
    {
        boost::winapi::BOOL_ res = boost::winapi::CryptGenRandom(
            hProv_,
            boost::numeric_cast<boost::winapi::DWORD_>(siz),
            static_cast<boost::winapi::BYTE_ *>(buf));
        if (BOOST_UNLIKELY(!res))
        {
            boost::winapi::DWORD_ err = boost::winapi::GetLastError();
            BOOST_THROW_EXCEPTION(entropy_error(err, "CryptGenRandom"));
        }
    }

private:
    void destroy() BOOST_NOEXCEPT
    {
        if (hProv_)
        {
            boost::ignore_unused(boost::winapi::CryptReleaseContext(hProv_, 0));
        }
    }

private:
    boost::winapi::HCRYPTPROV_ hProv_;
};

} // detail
} // uuids
} // boost
