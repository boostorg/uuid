//
// Copyright (c) 2017 James E. King III
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//   https://www.boost.org/LICENSE_1_0.txt)
//
// BCrypt provider for entropy
//

#include <cstddef>
#include <boost/config.hpp>
#include <boost/core/ignore_unused.hpp>
#include <boost/move/core.hpp>
#include <boost/numeric/conversion/cast.hpp>
#include <boost/winapi/bcrypt.hpp>
#include <boost/winapi/get_last_error.hpp>
#include <boost/throw_exception.hpp>

#if defined(BOOST_UUID_FORCE_AUTO_LINK) || (!defined(BOOST_ALL_NO_LIB) && !defined(BOOST_UUID_RANDOM_PROVIDER_NO_LIB))
#   if defined(BOOST_MSVC) \
    || defined(__BORLANDC__) \
    || (defined(__MWERKS__) && defined(_WIN32) && (__MWERKS__ >= 0x3000)) \
    || (defined(__ICL) && defined(_MSC_EXTENSIONS) && (_MSC_VER >= 1200)) \
    || (defined(BOOST_CLANG) && defined(BOOST_WINDOWS) && defined(_MSC_VER) && (__clang_major__ >= 4))
#      pragma comment(lib, "bcrypt.lib")
#   endif
#endif

namespace boost {
namespace uuids {
namespace detail {

class random_provider_base
{
    BOOST_MOVABLE_BUT_NOT_COPYABLE(random_provider_base)

public:
    random_provider_base()
      : hProv_(NULL)
    {
        boost::winapi::NTSTATUS_ status =
            boost::winapi::BCryptOpenAlgorithmProvider(
                &hProv_, 
                boost::winapi::BCRYPT_RNG_ALGORITHM_,
                NULL,
                0);

        if (BOOST_UNLIKELY(status != 0))
        {
            BOOST_THROW_EXCEPTION(entropy_error(status, "BCryptOpenAlgorithmProvider"));
        }
    }

    random_provider_base(BOOST_RV_REF(random_provider_base) that) BOOST_NOEXCEPT : hProv_(that.hProv_)
    {
        that.hProv_ = NULL;
    }

    random_provider_base& operator= (BOOST_RV_REF(random_provider_base) that) BOOST_NOEXCEPT
    {
        destroy();
        hProv_ = that.hProv_;
        that.hProv_ = NULL;
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
        boost::winapi::NTSTATUS_ status =
            boost::winapi::BCryptGenRandom(
                hProv_,
                static_cast<boost::winapi::PUCHAR_>(buf),
                boost::numeric_cast<boost::winapi::ULONG_>(siz),
                0);

        if (BOOST_UNLIKELY(status != 0))
        {
            BOOST_THROW_EXCEPTION(entropy_error(status, "BCryptGenRandom"));
        }
    }

private:
    void destroy() BOOST_NOEXCEPT
    {
        if (hProv_)
        {
            boost::ignore_unused(boost::winapi::BCryptCloseAlgorithmProvider(hProv_, 0));
        }
    }

private:
    boost::winapi::BCRYPT_ALG_HANDLE_ hProv_;
};

} // detail
} // uuids
} // boost
