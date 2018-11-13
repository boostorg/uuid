//  libs/uuid/test/test_md5.cpp  --------------------------------//

// (C) Copyright 2017 James E. King III

// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/cstdint.hpp>
#include <boost/detail/lightweight_test.hpp>
#include <boost/uuid/detail/md5.hpp>
#include <boost/predef/other/endian.h>

/* the endian was updated in MD5_Final() from md5.hpp */

#if !BOOST_ENDIAN_BIG_BYTE
#define ntohl(x)        (x)
#else
#define ntohl(x)        ((((x) & 0x000000ff) << 24) |   \
                         (((x) & 0x0000ff00) <<  8) |   \
                         (((x) & 0x00ff0000) >>  8) |   \
                         (((x) & 0xff000000) >> 24))
#endif

int main(int, char**)
{
    typedef struct
    {
        const char * data;
        boost::uint32_t len;
        unsigned char expected[16];
    } Expectation;

    /* http://www.febooti.com/products/filetweak/members/hash-and-crc/test-vectors/ */
    Expectation expectations[3] = {
        { "The quick brown fox jumps over the lazy dog", 43,
          { 0x9e, 0x10, 0x7d, 0x9d, 0x37, 0x2b, 0xb6, 0x82,
            0x6b, 0xd8, 0x1d, 0x35, 0x42, 0xa4, 0x19, 0xd6 }},
        { "Test vector from febooti.com", 28,
          { 0x50, 0x0a, 0xb6, 0x61, 0x3c, 0x6d, 0xb7, 0xfb,
            0xd3, 0x0c, 0x62, 0xf5, 0xff, 0x57, 0x3d, 0x0f }},
        { "", 0,
          { 0xd4, 0x1d, 0x8c, 0xd9, 0x8f, 0x00, 0xb2, 0x04,
            0xe9, 0x80, 0x09, 0x98, 0xec, 0xf8, 0x42, 0x7e }}
    };

    for (boost::uint32_t i = 0; i < 3; ++i) {
        boost::uuids::detail::md5 hash;
        hash.process_bytes(expectations[i].data, expectations[i].len);
        boost::uuids::detail::md5::digest_type result;
        hash.get_digest(result);
        for (boost::uint32_t j = 0; j < 4; ++j) {
            result[j] = ntohl(result[j]);
        }
        BOOST_TEST_EQ(0, memcmp(result, expectations[i].expected,
            sizeof(boost::uuids::detail::md5::digest_type)));
        BOOST_TEST_EQ(hash.get_version(), 0x03);

    }

    return boost::report_errors();
}
