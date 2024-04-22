// Copyright 2024 Peter Dimov
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

#include <boost/uuid/detail/endian.hpp>
#include <boost/core/lightweight_test.hpp>
#include <cstdint>

int main()
{
    namespace detail = boost::uuids::detail;

    // byteswap u32

    {
        std::uint32_t x = 0x01020304;
        std::uint32_t y = 0x04030201;

        BOOST_TEST_EQ( detail::byteswap( x ), y );
        BOOST_TEST_EQ( detail::byteswap( y ), x );
    }

    {
        std::uint32_t x = 0xFFEEDDCC;
        std::uint32_t y = 0xCCDDEEFF;

        BOOST_TEST_EQ( detail::byteswap( x ), y );
        BOOST_TEST_EQ( detail::byteswap( y ), x );
    }

    // byteswap u64

    {
        std::uint64_t x = 0x0102030405060708;
        std::uint64_t y = 0x0807060504030201;

        BOOST_TEST_EQ( detail::byteswap( x ), y );
        BOOST_TEST_EQ( detail::byteswap( y ), x );
    }

    {
        std::uint64_t x = 0xFFEEDDCCBBAA9988;
        std::uint64_t y = 0x8899AABBCCDDEEFF;

        BOOST_TEST_EQ( detail::byteswap( x ), y );
        BOOST_TEST_EQ( detail::byteswap( y ), x );
    }

    // load u32

    {
        std::uint32_t x = 0x01020304;
        std::uint32_t y = 0x04030201;

        unsigned char data[] = { 0x01, 0x02, 0x03, 0x04 };

        BOOST_TEST_EQ( detail::load_little_u32( data ), y );
        BOOST_TEST_EQ( detail::load_big_u32( data ), x );

#if BOOST_UUID_BYTE_ORDER == BOOST_UUID_ORDER_LITTLE_ENDIAN

        BOOST_TEST_EQ( detail::load_native_u32( data ), y );

#else

        BOOST_TEST_EQ( detail::load_native_u32( data ), x );

#endif
    }

    {
        std::uint32_t x = 0xFFEEDDCC;
        std::uint32_t y = 0xCCDDEEFF;

        unsigned char data[] = { 0xFF, 0xEE, 0xDD, 0xCC };

        BOOST_TEST_EQ( detail::load_little_u32( data ), y );
        BOOST_TEST_EQ( detail::load_big_u32( data ), x );

#if BOOST_UUID_BYTE_ORDER == BOOST_UUID_ORDER_LITTLE_ENDIAN

        BOOST_TEST_EQ( detail::load_native_u32( data ), y );

#else

        BOOST_TEST_EQ( detail::load_native_u32( data ), x );

#endif
    }

    // load u64

    {
        std::uint64_t x = 0x0102030405060708;
        std::uint64_t y = 0x0807060504030201;

        unsigned char data[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08 };

        BOOST_TEST_EQ( detail::load_little_u64( data ), y );
        BOOST_TEST_EQ( detail::load_big_u64( data ), x );

#if BOOST_UUID_BYTE_ORDER == BOOST_UUID_ORDER_LITTLE_ENDIAN

        BOOST_TEST_EQ( detail::load_native_u64( data ), y );

#else

        BOOST_TEST_EQ( detail::load_native_u64( data ), x );

#endif
    }

    {
        std::uint64_t x = 0xFFEEDDCCBBAA9988;
        std::uint64_t y = 0x8899AABBCCDDEEFF;

        unsigned char data[] = { 0xFF, 0xEE, 0xDD, 0xCC, 0xBB, 0xAA, 0x99, 0x88 };

        BOOST_TEST_EQ( detail::load_little_u64( data ), y );
        BOOST_TEST_EQ( detail::load_big_u64( data ), x );

#if BOOST_UUID_BYTE_ORDER == BOOST_UUID_ORDER_LITTLE_ENDIAN

        BOOST_TEST_EQ( detail::load_native_u64( data ), y );

#else

        BOOST_TEST_EQ( detail::load_native_u64( data ), x );

#endif
    }

    // store u32

    {
        std::uint32_t x = 0x01020304;
        std::uint32_t y = 0x04030201;

        unsigned char data[ 4 ] = {};

        detail::store_little_u32( data, x );
        BOOST_TEST_EQ( detail::load_little_u32( data ), x );
        BOOST_TEST_EQ( detail::load_big_u32( data ), y );

        detail::store_big_u32( data, x );
        BOOST_TEST_EQ( detail::load_little_u32( data ), y );
        BOOST_TEST_EQ( detail::load_big_u32( data ), x );

        detail::store_native_u32( data, x );

#if BOOST_UUID_BYTE_ORDER == BOOST_UUID_ORDER_LITTLE_ENDIAN

        BOOST_TEST_EQ( detail::load_little_u32( data ), x );
        BOOST_TEST_EQ( detail::load_big_u32( data ), y );

#else

        BOOST_TEST_EQ( detail::load_little_u32( data ), y );
        BOOST_TEST_EQ( detail::load_big_u32( data ), x );

#endif
    }

    {
        std::uint32_t x = 0xFFEEDDCC;
        std::uint32_t y = 0xCCDDEEFF;

        unsigned char data[ 4 ] = {};

        detail::store_little_u32( data, x );
        BOOST_TEST_EQ( detail::load_little_u32( data ), x );
        BOOST_TEST_EQ( detail::load_big_u32( data ), y );

        detail::store_big_u32( data, x );
        BOOST_TEST_EQ( detail::load_little_u32( data ), y );
        BOOST_TEST_EQ( detail::load_big_u32( data ), x );

        detail::store_native_u32( data, x );

#if BOOST_UUID_BYTE_ORDER == BOOST_UUID_ORDER_LITTLE_ENDIAN

        BOOST_TEST_EQ( detail::load_little_u32( data ), x );
        BOOST_TEST_EQ( detail::load_big_u32( data ), y );

#else

        BOOST_TEST_EQ( detail::load_little_u32( data ), y );
        BOOST_TEST_EQ( detail::load_big_u32( data ), x );

#endif
    }

    return boost::report_errors();
}
