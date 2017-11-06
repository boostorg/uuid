/* boost uuid/detail/random/random_device_wincrypt tests
 *
 * Copyright (c) 2017 James E. King III
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENCE_1_0.txt)
 *
 * $Id$
 */

#include <boost/cstdint.hpp>
#include <boost/detail/lightweight_test.hpp>
#include <boost/timer/timer.hpp>
#include <boost/uuid/detail/random/random_device_wincrypt.hpp>
#include <boost/uuid/uuid.hpp>
#include <iostream>
#include <string.h>

#if (BOOST_WINAPI_PARTITION_DESKTOP || BOOST_WINAPI_PARTITION_SYSTEM) && defined(BOOST_UUID_TESTSTUBS) && defined(_MSC_VER)

#include <deque>
std::deque<boost::winapi::BOOL_> next_result;

boost::winapi::BOOL_ WINAPI
CryptAcquireContextA(
    boost::winapi::HCRYPTPROV_ *phProv,
    boost::winapi::LPCSTR_ pszContainer,
    boost::winapi::LPCSTR_ pszProvider,
    boost::winapi::DWORD_ dwProvType,
    boost::winapi::DWORD_ dwFlags)
{
    boost::ignore_unused(phProv);
    boost::ignore_unused(pszContainer);
    boost::ignore_unused(pszProvider);
    boost::ignore_unused(dwProvType);
    boost::ignore_unused(dwFlags);

    boost::winapi::BOOL_ result = next_result.front();
    next_result.pop_front();
    return result;
}

boost::winapi::BOOL_ WINAPI
CryptAcquireContextW(
    boost::winapi::HCRYPTPROV_ *phProv,
    boost::winapi::LPCWSTR_ szContainer,
    boost::winapi::LPCWSTR_ szProvider,
    boost::winapi::DWORD_ dwProvType,
    boost::winapi::DWORD_ dwFlags)
{
    boost::ignore_unused(phProv);
    boost::ignore_unused(szContainer);
    boost::ignore_unused(szProvider);
    boost::ignore_unused(dwProvType);
    boost::ignore_unused(dwFlags);

    boost::winapi::BOOL_ result = next_result.front();
    next_result.pop_front();
    return result;
}

boost::winapi::BOOL_ WINAPI
CryptGenRandom(
    boost::winapi::HCRYPTPROV_ hProv,
    boost::winapi::DWORD_ dwLen,
    boost::winapi::BYTE_ *pbBuffer)
{
    boost::ignore_unused(hProv);
    boost::ignore_unused(dwLen);
    boost::ignore_unused(pbBuffer);

    boost::winapi::BOOL_ result = next_result.front();
    next_result.pop_front();
    return result;
}

// the implementation never gets this far in error path testing
BOOST_SYMBOL_IMPORT boost::winapi::BOOL_ WINAPI
CryptReleaseContext(
    boost::winapi::HCRYPTPROV_ hProv,
#if defined(_MSC_VER) && (_MSC_VER+0) >= 1500 && (_MSC_VER+0) < 1900 && BOOST_USE_NTDDI_VERSION < BOOST_WINAPI_NTDDI_WINXP
    // see winapi crypt.hpp for more details on why these differ...
    boost::winapi::ULONG_PTR_ dwFlags
#else
    boost::winapi::DWORD_ dwFlags
#endif
)
{
    boost::ignore_unused(hProv);
    boost::ignore_unused(dwFlags);
    return true;
}

#endif

int main(int, char*[])
{
#if BOOST_WINAPI_PARTITION_DESKTOP || BOOST_WINAPI_PARTITION_SYSTEM
    using boost::uuids::detail::random::random_device_wincrypt;
    using boost::uuids::entropy_error;

#if !defined(BOOST_UUID_TESTSTUBS)

    // only test happy path if we're not stubbing things:
    random_device_wincrypt<unsigned int> gen;
    unsigned int entropy = gen();
    unsigned int entropy2 = gen();
    BOOST_TEST_NE(entropy, entropy2);
    BOOST_TEST_EQ(gen.name(), "wincrypt");

#elif defined(_MSC_VER)

    // "error on construction" test case: CryptAcquireContext fails
    next_result.push_back(false);
    BOOST_TEST_THROWS(random_device_wincrypt<unsigned int>(), boost::uuids::entropy_error);
    BOOST_TEST(next_result.empty());

    // "error on read" test case: CryptGenRandom fails
    next_result.push_back(true);
    next_result.push_back(false);
    BOOST_TEST_THROWS(random_device_wincrypt<unsigned int>()(), boost::uuids::entropy_error);
    BOOST_TEST(next_result.empty());

#endif // sad path
#endif // wincrypt enabled

    return boost::report_errors();
}
