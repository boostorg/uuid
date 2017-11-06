/* boost uuid/detail/random/random_device_bcrypt tests
 *
 * Copyright (c) 2017 James E. King III
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENCE_1_0.txt)
 *
 * $Id$
 */

#include <boost/detail/lightweight_test.hpp>
#include <boost/uuid/detail/random/random_device_bcrypt.hpp>

#if (defined(BOOST_UUID_RANDOM_DEVICE_BCRYPT_AVAILABLE) && !defined(BOOST_UUID_RANDOM_DEVICE_FORCE_WINCRYPT)) \
    && defined(BOOST_UUID_TESTSTUBS) && defined(_MSC_VER)

#include <deque>
std::deque<boost::winapi::NTSTATUS_> next_result;

boost::winapi::NTSTATUS_ WINAPI
BCryptOpenAlgorithmProvider(
    boost::winapi::BCRYPT_ALG_HANDLE_ *phAlgorithm,
    boost::winapi::LPCWSTR_           pszAlgId,
    boost::winapi::LPCWSTR_           pszImplementation,
    boost::winapi::DWORD_             dwFlags
)
{
    boost::ignore_unused(phAlgorithm);
    boost::ignore_unused(pszAlgId);
    boost::ignore_unused(pszImplementation);
    boost::ignore_unused(dwFlags);

    boost::winapi::NTSTATUS_ result = next_result.front();
    next_result.pop_front();
    return result;
}

boost::winapi::NTSTATUS_ WINAPI
BCryptGenRandom(
    boost::winapi::BCRYPT_ALG_HANDLE_ hAlgorithm,
    boost::winapi::PUCHAR_            pbBuffer,
    boost::winapi::ULONG_             cbBuffer,
    boost::winapi::ULONG_             dwFlags
)
{
    boost::ignore_unused(hAlgorithm);
    boost::ignore_unused(pbBuffer);
    boost::ignore_unused(cbBuffer);
    boost::ignore_unused(dwFlags);

    boost::winapi::NTSTATUS_ result = next_result.front();
    next_result.pop_front();
    return result;
}

// the implementation never gets this far in error path testing
boost::winapi::NTSTATUS_ WINAPI
BCryptCloseAlgorithmProvider(
    boost::winapi::BCRYPT_ALG_HANDLE_ hAlgorithm,
    boost::winapi::ULONG_             dwFlags
)
{
    boost::ignore_unused(hAlgorithm);
    boost::ignore_unused(dwFlags);
    return 0;
}

#endif

int main(int, char*[])
{
#if defined(BOOST_UUID_RANDOM_DEVICE_BCRYPT_AVAILABLE) && !defined(BOOST_UUID_RANDOM_DEVICE_FORCE_WINCRYPT)
    using boost::uuids::detail::random::random_device_bcrypt;
    using boost::uuids::entropy_error;

#if !defined(BOOST_UUID_TESTSTUBS)

    // only test happy path if we're not stubbing things:
    random_device_bcrypt<unsigned int> gen;
    unsigned int entropy = gen();
    unsigned int entropy2 = gen();
    BOOST_TEST_NE(entropy, entropy2);
    BOOST_TEST_EQ(gen.name(), "bcrypt");

#elif defined(_MSC_VER)

    // "error on construction" test case: BCryptOpenAlgorithmProvider fails
    next_result.push_back(17);
    BOOST_TEST_THROWS(random_device_bcrypt<unsigned int>(), boost::uuids::entropy_error);
    BOOST_TEST(next_result.empty());

    // "error on read" test case: BCryptOpenAlgorithmProvider succeeds, but BCryptGenRandom fails
    next_result.push_back(0);
    next_result.push_back(18);
    BOOST_TEST_THROWS(random_device_bcrypt<unsigned int>()(), boost::uuids::entropy_error);
    BOOST_TEST(next_result.empty());

#endif // sad path
#endif // bcrypt enabled

    return boost::report_errors();
}
