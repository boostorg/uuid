#ifndef BOOST_UUID_DETAIL_FROM_CHARS_RISCV_HPP_INCLUDED
#define BOOST_UUID_DETAIL_FROM_CHARS_RISCV_HPP_INCLUDED

// Copyright 2026 Dan Kahyan
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

#include <boost/uuid/detail/config.hpp>

#if defined(BOOST_UUID_USE_RISCV_V)

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <riscv_vector.h>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/detail/endian.hpp>
#include <boost/uuid/detail/from_chars_result.hpp>
#include <boost/uuid/detail/simd_vector.hpp>

#if defined(BOOST_UUID_REPORT_IMPLEMENTATION)
#include <boost/config/pragma_message.hpp>
BOOST_PRAGMA_MESSAGE( "Using from_chars_riscv.hpp, RISC-V Vector Extension" )
#endif // #if defined(BOOST_UUID_REPORT_IMPLEMENTATION)

namespace boost {
namespace uuids {
namespace detail {

template<
    typename Char,
    bool IsCharASCIICompatible = ('0' == 0x30 && '9' == 0x39 && 'A' == 0x41 && 'F' == 0x46 && 'a' == 0x61 && 'f' == 0x66 && '-' == 0x2D),
    bool IsWCharASCIICompatible = (L'0' == 0x30 && L'9' == 0x39 && L'A' == 0x41 && L'F' == 0x46 && L'a' == 0x61 && L'f' == 0x66 && L'-' == 0x2D)
>
struct from_chars_simd_char_constants
{
    static const simd_vector128< std::uint8_t > mm_expected_dashes1;
    static const simd_vector128< std::uint8_t > mm_expected_dashes2;

    static constexpr std::uint8_t char_code2 = 0x61; // 'a' in ASCII
    static constexpr std::uint8_t char_code2_sub = static_cast< std::uint8_t >(char_code2 - 10u);
    static constexpr std::uint8_t char_code1 = 0x41; // 'A' in ASCII
    static constexpr std::uint8_t char_code1_sub = static_cast< std::uint8_t >(char_code1 - 10u);
    static constexpr std::uint8_t char_code0 = 0x30; // '0' in ASCII
    static constexpr std::uint8_t char_code0_sub = char_code0;

    static constexpr std::uint32_t char_code_sub =
        (static_cast< std::uint32_t >(char_code0_sub) << 16u) | (static_cast< std::uint32_t >(char_code1_sub) << 8u) | char_code2_sub;

    static const simd_vector128< std::uint8_t > mm_char_code2_cmp;
    static const simd_vector128< std::uint8_t > mm_char_code1_cmp;

    static const simd_vector128< std::uint8_t > mm_char_code2_sub;
    static const simd_vector128< std::uint8_t > mm_char_code1_sub;
    static const simd_vector128< std::uint8_t > mm_char_code0_sub;
};

template< typename Char, bool IsCharASCIICompatible, bool IsWCharASCIICompatible >
const simd_vector128< std::uint8_t > from_chars_simd_char_constants< Char, IsCharASCIICompatible, IsWCharASCIICompatible >::mm_expected_dashes1 =
    {{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x2D, 0x00, 0x00, 0x00, 0x00, 0x2D, 0x00, 0x00 }};

template< typename Char, bool IsCharASCIICompatible, bool IsWCharASCIICompatible >
const simd_vector128< std::uint8_t > from_chars_simd_char_constants< Char, IsCharASCIICompatible, IsWCharASCIICompatible >::mm_expected_dashes2 =
    {{ 0x00, 0x00, 0x2D, 0x00, 0x00, 0x00, 0x00, 0x2D, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }};

template< typename Char, bool IsCharASCIICompatible, bool IsWCharASCIICompatible >
const simd_vector128< std::uint8_t > from_chars_simd_char_constants< Char, IsCharASCIICompatible, IsWCharASCIICompatible >::mm_char_code2_cmp =
{{
    static_cast< std::uint8_t >(char_code2 - 1u), static_cast< std::uint8_t >(char_code2 - 1u),
    static_cast< std::uint8_t >(char_code2 - 1u), static_cast< std::uint8_t >(char_code2 - 1u),
    static_cast< std::uint8_t >(char_code2 - 1u), static_cast< std::uint8_t >(char_code2 - 1u),
    static_cast< std::uint8_t >(char_code2 - 1u), static_cast< std::uint8_t >(char_code2 - 1u),
    static_cast< std::uint8_t >(char_code2 - 1u), static_cast< std::uint8_t >(char_code2 - 1u),
    static_cast< std::uint8_t >(char_code2 - 1u), static_cast< std::uint8_t >(char_code2 - 1u),
    static_cast< std::uint8_t >(char_code2 - 1u), static_cast< std::uint8_t >(char_code2 - 1u),
    static_cast< std::uint8_t >(char_code2 - 1u), static_cast< std::uint8_t >(char_code2 - 1u)
}};

template< typename Char, bool IsCharASCIICompatible, bool IsWCharASCIICompatible >
const simd_vector128< std::uint8_t > from_chars_simd_char_constants< Char, IsCharASCIICompatible, IsWCharASCIICompatible >::mm_char_code1_cmp =
{{
    static_cast< std::uint8_t >(char_code1 - 1u), static_cast< std::uint8_t >(char_code1 - 1u),
    static_cast< std::uint8_t >(char_code1 - 1u), static_cast< std::uint8_t >(char_code1 - 1u),
    static_cast< std::uint8_t >(char_code1 - 1u), static_cast< std::uint8_t >(char_code1 - 1u),
    static_cast< std::uint8_t >(char_code1 - 1u), static_cast< std::uint8_t >(char_code1 - 1u),
    static_cast< std::uint8_t >(char_code1 - 1u), static_cast< std::uint8_t >(char_code1 - 1u),
    static_cast< std::uint8_t >(char_code1 - 1u), static_cast< std::uint8_t >(char_code1 - 1u),
    static_cast< std::uint8_t >(char_code1 - 1u), static_cast< std::uint8_t >(char_code1 - 1u),
    static_cast< std::uint8_t >(char_code1 - 1u), static_cast< std::uint8_t >(char_code1 - 1u)
}};

template< typename Char, bool IsCharASCIICompatible, bool IsWCharASCIICompatible >
const simd_vector128< std::uint8_t > from_chars_simd_char_constants< Char, IsCharASCIICompatible, IsWCharASCIICompatible >::mm_char_code2_sub =
{{
    char_code2_sub, char_code2_sub, char_code2_sub, char_code2_sub, char_code2_sub, char_code2_sub, char_code2_sub, char_code2_sub,
    char_code2_sub, char_code2_sub, char_code2_sub, char_code2_sub, char_code2_sub, char_code2_sub, char_code2_sub, char_code2_sub
}};

template< typename Char, bool IsCharASCIICompatible, bool IsWCharASCIICompatible >
const simd_vector128< std::uint8_t > from_chars_simd_char_constants< Char, IsCharASCIICompatible, IsWCharASCIICompatible >::mm_char_code1_sub =
{{
    char_code1_sub, char_code1_sub, char_code1_sub, char_code1_sub, char_code1_sub, char_code1_sub, char_code1_sub, char_code1_sub,
    char_code1_sub, char_code1_sub, char_code1_sub, char_code1_sub, char_code1_sub, char_code1_sub, char_code1_sub, char_code1_sub
}};

template< typename Char, bool IsCharASCIICompatible, bool IsWCharASCIICompatible >
const simd_vector128< std::uint8_t > from_chars_simd_char_constants< Char, IsCharASCIICompatible, IsWCharASCIICompatible >::mm_char_code0_sub =
{{
    char_code0_sub, char_code0_sub, char_code0_sub, char_code0_sub, char_code0_sub, char_code0_sub, char_code0_sub, char_code0_sub,
    char_code0_sub, char_code0_sub, char_code0_sub, char_code0_sub, char_code0_sub, char_code0_sub, char_code0_sub, char_code0_sub
}};

template< bool IsWCharASCIICompatible >
struct from_chars_simd_char_constants< char, false, IsWCharASCIICompatible >
{
    static_assert(static_cast< std::int8_t >('0') > -128 && static_cast< std::int8_t >('A') > -128 && static_cast< std::int8_t >('a') > -128,
        "Boost.UUID: Unsupported char encoding, hexadecimal character codes are expected to be greater than -128");

    static const simd_vector128< std::uint8_t > mm_expected_dashes1;
    static const simd_vector128< std::uint8_t > mm_expected_dashes2;

    static constexpr std::uint8_t char_code2 = static_cast< std::uint8_t >
    (
        static_cast< std::int8_t >('a') > static_cast< std::int8_t >('A') ?
        (
            static_cast< std::int8_t >('a') > static_cast< std::int8_t >('0') ? 'a' : '0'
        ) :
        (
            static_cast< std::int8_t >('A') > static_cast< std::int8_t >('0') ? 'A' : '0'
        )
    );
    static constexpr std::uint8_t char_code2_sub = char_code2 == static_cast< std::uint8_t >('0') ?
        static_cast< std::uint8_t >('0') : static_cast< std::uint8_t >(char_code2 - 10u);

    static constexpr std::uint8_t char_code1 = static_cast< std::uint8_t >
    (
        static_cast< std::int8_t >('a') > static_cast< std::int8_t >('A') ?
        (
            static_cast< std::int8_t >('a') < static_cast< std::int8_t >('0') ? 'a' : '0'
        ) :
        (
            static_cast< std::int8_t >('A') < static_cast< std::int8_t >('0') ? 'A' : '0'
        )
    );
    static constexpr std::uint8_t char_code1_sub = char_code1 == static_cast< std::uint8_t >('0') ?
        static_cast< std::uint8_t >('0') : static_cast< std::uint8_t >(char_code1 - 10u);

    static constexpr std::uint8_t char_code0 = static_cast< std::uint8_t >
    (
        static_cast< std::int8_t >('a') < static_cast< std::int8_t >('A') ?
        (
            static_cast< std::int8_t >('a') < static_cast< std::int8_t >('0') ? 'a' : '0'
        ) :
        (
            static_cast< std::int8_t >('A') < static_cast< std::int8_t >('0') ? 'A' : '0'
        )
    );
    static constexpr std::uint8_t char_code0_sub = char_code0 == static_cast< std::uint8_t >('0') ?
        static_cast< std::uint8_t >('0') : static_cast< std::uint8_t >(char_code0 - 10u);

    static constexpr std::uint32_t char_code_sub =
        (static_cast< std::uint32_t >(char_code0_sub) << 16u) | (static_cast< std::uint32_t >(char_code1_sub) << 8u) | char_code2_sub;

    static const simd_vector128< std::uint8_t > mm_char_code2_cmp;
    static const simd_vector128< std::uint8_t > mm_char_code1_cmp;

    static const simd_vector128< std::uint8_t > mm_char_code2_sub;
    static const simd_vector128< std::uint8_t > mm_char_code1_sub;
    static const simd_vector128< std::uint8_t > mm_char_code0_sub;
};

template< bool IsWCharASCIICompatible >
const simd_vector128< std::uint8_t > from_chars_simd_char_constants< char, false, IsWCharASCIICompatible >::mm_expected_dashes1 =
{{
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    static_cast< std::uint8_t >('-'), 0x00, 0x00, 0x00, 0x00, static_cast< std::uint8_t >('-'), 0x00, 0x00
}};

template< bool IsWCharASCIICompatible >
const simd_vector128< std::uint8_t > from_chars_simd_char_constants< char, false, IsWCharASCIICompatible >::mm_expected_dashes2 =
{{
    0x00, 0x00, static_cast< std::uint8_t >('-'), 0x00, 0x00, 0x00, 0x00, static_cast< std::uint8_t >('-'),
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
}};

template< bool IsWCharASCIICompatible >
const simd_vector128< std::uint8_t > from_chars_simd_char_constants< char, false, IsWCharASCIICompatible >::mm_char_code2_cmp =
{{
    static_cast< std::uint8_t >(char_code2 - 1u), static_cast< std::uint8_t >(char_code2 - 1u),
    static_cast< std::uint8_t >(char_code2 - 1u), static_cast< std::uint8_t >(char_code2 - 1u),
    static_cast< std::uint8_t >(char_code2 - 1u), static_cast< std::uint8_t >(char_code2 - 1u),
    static_cast< std::uint8_t >(char_code2 - 1u), static_cast< std::uint8_t >(char_code2 - 1u),
    static_cast< std::uint8_t >(char_code2 - 1u), static_cast< std::uint8_t >(char_code2 - 1u),
    static_cast< std::uint8_t >(char_code2 - 1u), static_cast< std::uint8_t >(char_code2 - 1u),
    static_cast< std::uint8_t >(char_code2 - 1u), static_cast< std::uint8_t >(char_code2 - 1u),
    static_cast< std::uint8_t >(char_code2 - 1u), static_cast< std::uint8_t >(char_code2 - 1u)
}};

template< bool IsWCharASCIICompatible >
const simd_vector128< std::uint8_t > from_chars_simd_char_constants< char, false, IsWCharASCIICompatible >::mm_char_code1_cmp =
{{
    static_cast< std::uint8_t >(char_code1 - 1u), static_cast< std::uint8_t >(char_code1 - 1u),
    static_cast< std::uint8_t >(char_code1 - 1u), static_cast< std::uint8_t >(char_code1 - 1u),
    static_cast< std::uint8_t >(char_code1 - 1u), static_cast< std::uint8_t >(char_code1 - 1u),
    static_cast< std::uint8_t >(char_code1 - 1u), static_cast< std::uint8_t >(char_code1 - 1u),
    static_cast< std::uint8_t >(char_code1 - 1u), static_cast< std::uint8_t >(char_code1 - 1u),
    static_cast< std::uint8_t >(char_code1 - 1u), static_cast< std::uint8_t >(char_code1 - 1u),
    static_cast< std::uint8_t >(char_code1 - 1u), static_cast< std::uint8_t >(char_code1 - 1u),
    static_cast< std::uint8_t >(char_code1 - 1u), static_cast< std::uint8_t >(char_code1 - 1u)
}};

template< bool IsWCharASCIICompatible >
const simd_vector128< std::uint8_t > from_chars_simd_char_constants< char, false, IsWCharASCIICompatible >::mm_char_code2_sub =
{{
    char_code2_sub, char_code2_sub, char_code2_sub, char_code2_sub, char_code2_sub, char_code2_sub, char_code2_sub, char_code2_sub,
    char_code2_sub, char_code2_sub, char_code2_sub, char_code2_sub, char_code2_sub, char_code2_sub, char_code2_sub, char_code2_sub
}};

template< bool IsWCharASCIICompatible >
const simd_vector128< std::uint8_t > from_chars_simd_char_constants< char, false, IsWCharASCIICompatible >::mm_char_code1_sub =
{{
    char_code1_sub, char_code1_sub, char_code1_sub, char_code1_sub, char_code1_sub, char_code1_sub, char_code1_sub, char_code1_sub,
    char_code1_sub, char_code1_sub, char_code1_sub, char_code1_sub, char_code1_sub, char_code1_sub, char_code1_sub, char_code1_sub
}};

template< bool IsWCharASCIICompatible >
const simd_vector128< std::uint8_t > from_chars_simd_char_constants< char, false, IsWCharASCIICompatible >::mm_char_code0_sub =
{{
    char_code0_sub, char_code0_sub, char_code0_sub, char_code0_sub, char_code0_sub, char_code0_sub, char_code0_sub, char_code0_sub,
    char_code0_sub, char_code0_sub, char_code0_sub, char_code0_sub, char_code0_sub, char_code0_sub, char_code0_sub, char_code0_sub
}};

template< bool IsCharASCIICompatible >
struct from_chars_simd_char_constants< wchar_t, IsCharASCIICompatible, false >
{
    static_assert(static_cast< wchar_t >(static_cast< std::uint8_t >(L'0')) == L'0' && static_cast< wchar_t >(static_cast< std::uint8_t >(L'9')) == L'9' &&
        static_cast< wchar_t >(static_cast< std::uint8_t >(L'a')) == L'a' && static_cast< wchar_t >(static_cast< std::uint8_t >(L'f')) == L'f' &&
        static_cast< wchar_t >(static_cast< std::uint8_t >(L'-')) == L'-',
        "Boost.UUID: Unsupported wchar_t encoding, hexadecimal and dash character codes are expected to be representable by a single byte");

    static_assert(static_cast< std::int8_t >(L'0') > -128 && static_cast< std::int8_t >(L'A') > -128 && static_cast< std::int8_t >(L'a') > -128,
        "Boost.UUID: Unsupported wchar_t encoding, hexadecimal character codes are expected to be greater than -128");

    static const simd_vector128< std::uint8_t > mm_expected_dashes1;
    static const simd_vector128< std::uint8_t > mm_expected_dashes2;

    static constexpr std::uint8_t char_code2 = static_cast< std::uint8_t >
    (
        static_cast< std::int8_t >(L'a') > static_cast< std::int8_t >(L'A') ?
        (
            static_cast< std::int8_t >(L'a') > static_cast< std::int8_t >(L'0') ? L'a' : L'0'
        ) :
        (
            static_cast< std::int8_t >(L'A') > static_cast< std::int8_t >(L'0') ? L'A' : L'0'
        )
    );
    static constexpr std::uint8_t char_code2_sub = char_code2 == static_cast< std::uint8_t >(L'0') ?
        static_cast< std::uint8_t >(L'0') : static_cast< std::uint8_t >(char_code2 - 10u);

    static constexpr std::uint8_t char_code1 = static_cast< std::uint8_t >
    (
        static_cast< std::int8_t >(L'a') > static_cast< std::int8_t >(L'A') ?
        (
            static_cast< std::int8_t >(L'a') < static_cast< std::int8_t >(L'0') ? L'a' : L'0'
        ) :
        (
            static_cast< std::int8_t >(L'A') < static_cast< std::int8_t >(L'0') ? L'A' : L'0'
        )
    );
    static constexpr std::uint8_t char_code1_sub = char_code1 == static_cast< std::uint8_t >(L'0') ?
        static_cast< std::uint8_t >(L'0') : static_cast< std::uint8_t >(char_code1 - 10u);

    static constexpr std::uint8_t char_code0 = static_cast< std::uint8_t >
    (
        static_cast< std::int8_t >(L'a') < static_cast< std::int8_t >(L'A') ?
        (
            static_cast< std::int8_t >(L'a') < static_cast< std::int8_t >(L'0') ? L'a' : L'0'
        ) :
        (
            static_cast< std::int8_t >(L'A') < static_cast< std::int8_t >(L'0') ? L'A' : L'0'
        )
    );
    static constexpr std::uint8_t char_code0_sub = char_code0 == static_cast< std::uint8_t >(L'0') ?
        static_cast< std::uint8_t >(L'0') : static_cast< std::uint8_t >(char_code0 - 10u);

    static constexpr std::uint32_t char_code_sub =
        (static_cast< std::uint32_t >(char_code0_sub) << 16u) | (static_cast< std::uint32_t >(char_code1_sub) << 8u) | char_code2_sub;

    static const simd_vector128< std::uint8_t > mm_char_code2_cmp;
    static const simd_vector128< std::uint8_t > mm_char_code1_cmp;

    static const simd_vector128< std::uint8_t > mm_char_code2_sub;
    static const simd_vector128< std::uint8_t > mm_char_code1_sub;
    static const simd_vector128< std::uint8_t > mm_char_code0_sub;
};

template< bool IsCharASCIICompatible >
const simd_vector128< std::uint8_t > from_chars_simd_char_constants< wchar_t, IsCharASCIICompatible, false >::mm_expected_dashes1 =
{{
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    static_cast< std::uint8_t >(L'-'), 0x00, 0x00, 0x00, 0x00, static_cast< std::uint8_t >(L'-'), 0x00, 0x00
}};

template< bool IsCharASCIICompatible >
const simd_vector128< std::uint8_t > from_chars_simd_char_constants< wchar_t, IsCharASCIICompatible, false >::mm_expected_dashes2 =
{{
    0x00, 0x00, static_cast< std::uint8_t >(L'-'), 0x00, 0x00, 0x00, 0x00, static_cast< std::uint8_t >(L'-'),
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
}};

template< bool IsCharASCIICompatible >
const simd_vector128< std::uint8_t > from_chars_simd_char_constants< wchar_t, IsCharASCIICompatible, false >::mm_char_code2_cmp =
{{
    static_cast< std::uint8_t >(char_code2 - 1u), static_cast< std::uint8_t >(char_code2 - 1u),
    static_cast< std::uint8_t >(char_code2 - 1u), static_cast< std::uint8_t >(char_code2 - 1u),
    static_cast< std::uint8_t >(char_code2 - 1u), static_cast< std::uint8_t >(char_code2 - 1u),
    static_cast< std::uint8_t >(char_code2 - 1u), static_cast< std::uint8_t >(char_code2 - 1u),
    static_cast< std::uint8_t >(char_code2 - 1u), static_cast< std::uint8_t >(char_code2 - 1u),
    static_cast< std::uint8_t >(char_code2 - 1u), static_cast< std::uint8_t >(char_code2 - 1u),
    static_cast< std::uint8_t >(char_code2 - 1u), static_cast< std::uint8_t >(char_code2 - 1u),
    static_cast< std::uint8_t >(char_code2 - 1u), static_cast< std::uint8_t >(char_code2 - 1u)
}};

template< bool IsCharASCIICompatible >
const simd_vector128< std::uint8_t > from_chars_simd_char_constants< wchar_t, IsCharASCIICompatible, false >::mm_char_code1_cmp =
{{
    static_cast< std::uint8_t >(char_code1 - 1u), static_cast< std::uint8_t >(char_code1 - 1u),
    static_cast< std::uint8_t >(char_code1 - 1u), static_cast< std::uint8_t >(char_code1 - 1u),
    static_cast< std::uint8_t >(char_code1 - 1u), static_cast< std::uint8_t >(char_code1 - 1u),
    static_cast< std::uint8_t >(char_code1 - 1u), static_cast< std::uint8_t >(char_code1 - 1u),
    static_cast< std::uint8_t >(char_code1 - 1u), static_cast< std::uint8_t >(char_code1 - 1u),
    static_cast< std::uint8_t >(char_code1 - 1u), static_cast< std::uint8_t >(char_code1 - 1u),
    static_cast< std::uint8_t >(char_code1 - 1u), static_cast< std::uint8_t >(char_code1 - 1u),
    static_cast< std::uint8_t >(char_code1 - 1u), static_cast< std::uint8_t >(char_code1 - 1u)
}};

template< bool IsCharASCIICompatible >
const simd_vector128< std::uint8_t > from_chars_simd_char_constants< wchar_t, IsCharASCIICompatible, false >::mm_char_code2_sub =
{{
    char_code2_sub, char_code2_sub, char_code2_sub, char_code2_sub, char_code2_sub, char_code2_sub, char_code2_sub, char_code2_sub,
    char_code2_sub, char_code2_sub, char_code2_sub, char_code2_sub, char_code2_sub, char_code2_sub, char_code2_sub, char_code2_sub
}};

template< bool IsCharASCIICompatible >
const simd_vector128< std::uint8_t > from_chars_simd_char_constants< wchar_t, IsCharASCIICompatible, false >::mm_char_code1_sub =
{{
    char_code1_sub, char_code1_sub, char_code1_sub, char_code1_sub, char_code1_sub, char_code1_sub, char_code1_sub, char_code1_sub,
    char_code1_sub, char_code1_sub, char_code1_sub, char_code1_sub, char_code1_sub, char_code1_sub, char_code1_sub, char_code1_sub
}};

template< bool IsCharASCIICompatible >
const simd_vector128< std::uint8_t > from_chars_simd_char_constants< wchar_t, IsCharASCIICompatible, false >::mm_char_code0_sub =
{{
    char_code0_sub, char_code0_sub, char_code0_sub, char_code0_sub, char_code0_sub, char_code0_sub, char_code0_sub, char_code0_sub,
    char_code0_sub, char_code0_sub, char_code0_sub, char_code0_sub, char_code0_sub, char_code0_sub, char_code0_sub, char_code0_sub
}};


template< typename >
struct from_chars_simd_constants
{
    static const simd_vector128< std::uint8_t > mm_F0;

    // Gather patterns for deinterleaving even and odd positioned hex digits from the three input vectors.
    // Each index is a position within the source vector (chars1, chars2 or chars3).
    static const simd_vector128< std::uint8_t > mm_upper_idx_chars1;
    static const simd_vector128< std::uint8_t > mm_upper_idx_chars2;
    static const simd_vector128< std::uint8_t > mm_upper_idx_chars3;

    static const simd_vector128< std::uint8_t > mm_lower_idx_chars1;
    static const simd_vector128< std::uint8_t > mm_lower_idx_chars2;
    static const simd_vector128< std::uint8_t > mm_lower_idx_chars3;
};

template< typename T >
const simd_vector128< std::uint8_t > from_chars_simd_constants< T >::mm_F0 =
    {{ 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0 }};

// Even-positioned hex digits in the original 36-character string become the high nibbles of the output bytes.
template< typename T >
const simd_vector128< std::uint8_t > from_chars_simd_constants< T >::mm_upper_idx_chars1 =
    {{ 0x00, 0x02, 0x04, 0x06, 0x09, 0x0B, 0x0E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }};

template< typename T >
const simd_vector128< std::uint8_t > from_chars_simd_constants< T >::mm_upper_idx_chars2 =
    {{ 0x00, 0x03, 0x05, 0x08, 0x0A, 0x0C, 0x0E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }};

template< typename T >
const simd_vector128< std::uint8_t > from_chars_simd_constants< T >::mm_upper_idx_chars3 =
    {{ 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }};

// Odd-positioned hex digits in the original 36-character string become the low nibbles of the output bytes.
template< typename T >
const simd_vector128< std::uint8_t > from_chars_simd_constants< T >::mm_lower_idx_chars1 =
    {{ 0x01, 0x03, 0x05, 0x07, 0x0A, 0x0C, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }};

template< typename T >
const simd_vector128< std::uint8_t > from_chars_simd_constants< T >::mm_lower_idx_chars2 =
    {{ 0x01, 0x04, 0x06, 0x09, 0x0B, 0x0D, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }};

template< typename T >
const simd_vector128< std::uint8_t > from_chars_simd_constants< T >::mm_lower_idx_chars3 =
    {{ 0x01, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }};


BOOST_FORCEINLINE vuint8m1_t load_vector128(const simd_vector128< std::uint8_t >& v, std::size_t vl) noexcept
{
    return __riscv_vle8_v_u8m1(v.bytes, vl);
}

template< typename Char, std::size_t Size = sizeof(Char) >
struct from_chars_simd_load_traits;

template< typename Char >
struct from_chars_simd_load_traits< Char, 1u >
{
    static BOOST_FORCEINLINE vuint8m1_t load_packed_16(const Char* p, std::size_t vl) noexcept
    {
        return __riscv_vle8_v_u8m1(reinterpret_cast< const std::uint8_t* >(p), vl);
    }

    static BOOST_FORCEINLINE vuint8m1_t load_packed_4(const Char* p, std::size_t vl) noexcept
    {
        (void)vl;
        return __riscv_vle8_v_u8m1(reinterpret_cast< const std::uint8_t* >(p), 4);
    }

    static BOOST_FORCEINLINE vuint8m1_t load_packed_n(const Char* p, unsigned int n, std::size_t vl) noexcept
    {
        alignas(16) std::uint8_t buf[16] = {};
        detail::memcpy(buf, p, n);
        return __riscv_vle8_v_u8m1(buf, vl);
    }
};

template< typename Char >
struct from_chars_simd_load_traits< Char, 2u >
{
    static BOOST_FORCEINLINE vuint8m1_t load_packed_16(const Char* p, std::size_t vl) noexcept
    {
        (void)vl;
        std::size_t vl16 = __riscv_vsetvl_e16m2(16);
        vuint16m2_t v16 = __riscv_vle16_v_u16m2(reinterpret_cast< const std::uint16_t* >(p), vl16);
        return __riscv_vnsrl_wx_u8m1(v16, 0, 16);
    }

    static BOOST_FORCEINLINE vuint8m1_t load_packed_4(const Char* p, std::size_t vl) noexcept
    {
        (void)vl;
        vuint16m1_t v16 = __riscv_vle16_v_u16m1(reinterpret_cast< const std::uint16_t* >(p), 4);
        vuint16m2_t v16_w = __riscv_vlmul_ext_v_u16m1_u16m2(v16);
        return __riscv_vnsrl_wx_u8m1(v16_w, 0, 4);
    }

    static BOOST_FORCEINLINE vuint8m1_t load_packed_n(const Char* p, unsigned int n, std::size_t vl) noexcept
    {
        alignas(16) std::uint8_t buf[16] = {};
        for (unsigned int i = 0u; i < n; ++i)
        {
            buf[i] = static_cast< std::uint8_t >(p[i]);
        }
        return __riscv_vle8_v_u8m1(buf, vl);
    }
};

template< typename Char >
struct from_chars_simd_load_traits< Char, 4u >
{
    static BOOST_FORCEINLINE vuint8m1_t load_packed_16(const Char* p, std::size_t vl) noexcept
    {
        (void)vl;
        std::size_t vl32 = __riscv_vsetvl_e32m4(16);
        vuint32m4_t v32 = __riscv_vle32_v_u32m4(reinterpret_cast< const std::uint32_t* >(p), vl32);
        vuint16m2_t v16 = __riscv_vnsrl_wx_u16m2(v32, 0, 16);
        return __riscv_vnsrl_wx_u8m1(v16, 0, 16);
    }

    static BOOST_FORCEINLINE vuint8m1_t load_packed_4(const Char* p, std::size_t vl) noexcept
    {
        (void)vl;
        vuint32m1_t v32 = __riscv_vle32_v_u32m1(reinterpret_cast< const std::uint32_t* >(p), 4);
        vuint32m4_t v32_w = __riscv_vlmul_ext_v_u32m1_u32m4(v32);
        vuint16m2_t v16 = __riscv_vnsrl_wx_u16m2(v32_w, 0, 4);
        return __riscv_vnsrl_wx_u8m1(v16, 0, 4);
    }

    static BOOST_FORCEINLINE vuint8m1_t load_packed_n(const Char* p, unsigned int n, std::size_t vl) noexcept
    {
        alignas(16) std::uint8_t buf[16] = {};
        for (unsigned int i = 0u; i < n; ++i)
        {
            buf[i] = static_cast< std::uint8_t >(p[i]);
        }
        return __riscv_vle8_v_u8m1(buf, vl);
    }
};


/*!
 * Converts a string of 36 hexadecimal UUID characters in `chars1`/`chars2`/`chars3` to a 16-byte binary value and,
 * if successful, stores it into `data`. If not successful, stores the failure character position to `end_pos`
 * and error code to `ec`.
 */
BOOST_FORCEINLINE void from_chars_simd_core
(
    vuint8m1_t chars1, vuint8m1_t chars2, vuint8m1_t chars3,
    vuint8m1_t const& expected_dashes1,
    vuint8m1_t const& expected_dashes2,
    vuint8m1_t const& char_code1_cmp,
    vuint8m1_t const& char_code2_cmp,
    vuint8m1_t const& char_code0_sub,
    vuint8m1_t const& char_code1_sub,
    vuint8m1_t const& char_code2_sub,
    std::uint8_t* data, unsigned int& end_pos, from_chars_error& ec, std::size_t vl
)
{
    using constants = uuids::detail::from_chars_simd_constants< void >;

    // Check if dashes are in the expected positions
    {
        const vuint8m1_t dash_char = __riscv_vmv_v_x_u8m1(static_cast< std::uint8_t >('-'), vl);

        const vbool8_t expected1 = __riscv_vmsgtu_vx_u8m1_b8(expected_dashes1, 0, vl);
        const vbool8_t expected2 = __riscv_vmsgtu_vx_u8m1_b8(expected_dashes2, 0, vl);

        const vbool8_t is_dash1 = __riscv_vmseq_vv_u8m1_b8(chars1, dash_char, vl);
        const vbool8_t is_dash2 = __riscv_vmseq_vv_u8m1_b8(chars2, dash_char, vl);

        const vbool8_t missing1 = __riscv_vmandn_mm_b8(expected1, is_dash1, vl);
        const vbool8_t missing2 = __riscv_vmandn_mm_b8(expected2, is_dash2, vl);

        const long first_missing1 = __riscv_vfirst_m_b8(missing1, vl);
        const long first_missing2 = __riscv_vfirst_m_b8(missing2, vl);

        unsigned int dash_pos = 0xFFFFFFFFu;
        if (first_missing1 >= 0)
        {
            dash_pos = static_cast< unsigned int >(first_missing1);
        }
        if (first_missing2 >= 0)
        {
            const unsigned int pos2 = static_cast< unsigned int >(first_missing2) + 16u;
            if (pos2 < dash_pos)
                dash_pos = pos2;
        }

        if (BOOST_UNLIKELY(dash_pos < end_pos))
        {
            end_pos = dash_pos;
            ec = from_chars_error::dash_expected;
        }
    }

    // Deinterleave even and odd positioned hex digits into upper and lower nibbles
    vuint8m1_t upper = __riscv_vmv_v_x_u8m1(0, vl);
    vuint8m1_t lower = __riscv_vmv_v_x_u8m1(0, vl);

    {
        const vuint8m1_t upper_idx1 = load_vector128(constants::mm_upper_idx_chars1, vl);
        const vuint8m1_t upper_idx2 = load_vector128(constants::mm_upper_idx_chars2, vl);
        const vuint8m1_t upper_idx3 = load_vector128(constants::mm_upper_idx_chars3, vl);

        const vuint8m1_t upper1 = __riscv_vrgather_vv_u8m1(chars1, upper_idx1, vl);
        const vuint8m1_t upper2 = __riscv_vrgather_vv_u8m1(chars2, upper_idx2, vl);
        const vuint8m1_t upper3 = __riscv_vrgather_vv_u8m1(chars3, upper_idx3, vl);

        upper = __riscv_vslideup_vx_u8m1(upper, upper1, 0, vl);
        upper = __riscv_vslideup_vx_u8m1(upper, upper2, 7, vl);
        upper = __riscv_vslideup_vx_u8m1(upper, upper3, 14, vl);
    }

    {
        const vuint8m1_t lower_idx1 = load_vector128(constants::mm_lower_idx_chars1, vl);
        const vuint8m1_t lower_idx2 = load_vector128(constants::mm_lower_idx_chars2, vl);
        const vuint8m1_t lower_idx3 = load_vector128(constants::mm_lower_idx_chars3, vl);

        const vuint8m1_t lower1 = __riscv_vrgather_vv_u8m1(chars1, lower_idx1, vl);
        const vuint8m1_t lower2 = __riscv_vrgather_vv_u8m1(chars2, lower_idx2, vl);
        const vuint8m1_t lower3 = __riscv_vrgather_vv_u8m1(chars3, lower_idx3, vl);

        lower = __riscv_vslideup_vx_u8m1(lower, lower1, 0, vl);
        lower = __riscv_vslideup_vx_u8m1(lower, lower2, 7, vl);
        lower = __riscv_vslideup_vx_u8m1(lower, lower3, 14, vl);
    }

    // Convert characters to 8-bit integers. See the comment in from_chars_x86.hpp for the algorithm description.
    const vint8m1_t upper_i8 = __riscv_vreinterpret_v_u8m1_i8m1(upper);
    const vint8m1_t lower_i8 = __riscv_vreinterpret_v_u8m1_i8m1(lower);
    const vint8m1_t char_code1_cmp_i8 = __riscv_vreinterpret_v_u8m1_i8m1(char_code1_cmp);
    const vint8m1_t char_code2_cmp_i8 = __riscv_vreinterpret_v_u8m1_i8m1(char_code2_cmp);

    const vbool8_t upper_code2_mask = __riscv_vmsgt_vv_i8m1_b8(upper_i8, char_code2_cmp_i8, vl);
    const vbool8_t upper_code1_mask = __riscv_vmsgt_vv_i8m1_b8(upper_i8, char_code1_cmp_i8, vl);
    const vbool8_t lower_code2_mask = __riscv_vmsgt_vv_i8m1_b8(lower_i8, char_code2_cmp_i8, vl);
    const vbool8_t lower_code1_mask = __riscv_vmsgt_vv_i8m1_b8(lower_i8, char_code1_cmp_i8, vl);

    vuint8m1_t upper_sub = __riscv_vmerge_vvm_u8m1(char_code1_sub, char_code2_sub, upper_code2_mask, vl);
    upper_sub = __riscv_vmerge_vvm_u8m1(char_code0_sub, upper_sub, upper_code1_mask, vl);

    vuint8m1_t lower_sub = __riscv_vmerge_vvm_u8m1(char_code1_sub, char_code2_sub, lower_code2_mask, vl);
    lower_sub = __riscv_vmerge_vvm_u8m1(char_code0_sub, lower_sub, lower_code1_mask, vl);

    upper = __riscv_vsub_vv_u8m1(upper, upper_sub, vl);
    lower = __riscv_vsub_vv_u8m1(lower, lower_sub, vl);

    // Check hexadecimal character validity. Values outside 0-15 will have non-zero upper 4 bits.
    const vuint8m1_t mm_F0 = load_vector128(constants::mm_F0, vl);
    const vuint8m1_t upper_hi_bits = __riscv_vand_vv_u8m1(upper, mm_F0, vl);
    const vuint8m1_t lower_hi_bits = __riscv_vand_vv_u8m1(lower, mm_F0, vl);

    const vbool8_t upper_valid = __riscv_vmseq_vx_u8m1_b8(upper_hi_bits, 0, vl);
    const vbool8_t lower_valid = __riscv_vmseq_vx_u8m1_b8(lower_hi_bits, 0, vl);

    const vbool8_t upper_invalid = __riscv_vmnand_mm_b8(upper_valid, upper_valid, vl);
    const vbool8_t lower_invalid = __riscv_vmnand_mm_b8(lower_valid, lower_valid, vl);

    const long first_invalid_upper = __riscv_vfirst_m_b8(upper_invalid, vl);
    const long first_invalid_lower = __riscv_vfirst_m_b8(lower_invalid, vl);

    if (BOOST_UNLIKELY(first_invalid_upper >= 0 || first_invalid_lower >= 0))
    {
        unsigned int pos_upper = 0xFFFFFFFFu;
        unsigned int pos_lower = 0xFFFFFFFFu;
        if (first_invalid_upper >= 0)
        {
            pos_upper = static_cast< unsigned int >(first_invalid_upper) * 2u;
        }
        if (first_invalid_lower >= 0)
        {
            pos_lower = static_cast< unsigned int >(first_invalid_lower) * 2u + 1u;
        }
        unsigned int pos = pos_upper < pos_lower ? pos_upper : pos_lower;
        if (pos >= 8u)
        {
            unsigned int dash_count = (pos - 4u) / 4u;
            if (dash_count > 4u)
                dash_count = 4u;
            pos += dash_count;
        }

        if (pos < end_pos)
        {
            end_pos = pos;
            ec = from_chars_error::hex_digit_expected;
        }
    }
    else if (BOOST_LIKELY(ec == from_chars_error::none))
    {
        const vuint8m1_t result = __riscv_vor_vv_u8m1(
            __riscv_vsll_vx_u8m1(upper, 4, vl),
            lower,
            vl);
        __riscv_vse8_v_u8m1(data, result, vl);
    }
}

template< typename Char >
BOOST_FORCEINLINE from_chars_result< Char > from_chars_simd(const Char* begin, const Char* end, uuid& u) noexcept
{
    static_assert(sizeof(Char) == 1u || sizeof(Char) == 2u || sizeof(Char) == 4u, "Boost.UUID: Unsupported input character type for from_chars");

    using char_constants = uuids::detail::from_chars_simd_char_constants< Char >;

    const std::size_t vl = __riscv_vsetvl_e8m1(16);

    unsigned int end_pos = 36u;
    from_chars_error ec = from_chars_error::none;
    vuint8m1_t chars1, chars2, chars3;

    if (BOOST_LIKELY((end - begin) >= 36))
    {
        chars1 = from_chars_simd_load_traits< Char >::load_packed_16(begin, vl);
        chars2 = from_chars_simd_load_traits< Char >::load_packed_16(begin + 16, vl);
        chars3 = from_chars_simd_load_traits< Char >::load_packed_4(begin + 32, vl);
    }
    else
    {
        end_pos = static_cast< unsigned int >(end - begin);
        ec = from_chars_error::unexpected_end_of_input;

        const Char* p = begin;
        unsigned int n = static_cast< unsigned int >(end - begin);
        if (n >= 16u)
        {
            chars1 = from_chars_simd_load_traits< Char >::load_packed_16(p, vl);
            p += 16;
            n -= 16u;
        }
        else
        {
            chars1 = from_chars_simd_load_traits< Char >::load_packed_n(p, n, vl);
            p += n;
            n = 0u;
        }

        if (n >= 16u)
        {
            chars2 = from_chars_simd_load_traits< Char >::load_packed_16(p, vl);
            p += 16;
            n -= 16u;
        }
        else
        {
            chars2 = from_chars_simd_load_traits< Char >::load_packed_n(p, n, vl);
            p += n;
            n = 0u;
        }

        chars3 = from_chars_simd_load_traits< Char >::load_packed_n(p, n, vl);
    }

    from_chars_simd_core
    (
        chars1, chars2, chars3,
        load_vector128(char_constants::mm_expected_dashes1, vl),
        load_vector128(char_constants::mm_expected_dashes2, vl),
        load_vector128(char_constants::mm_char_code1_cmp, vl),
        load_vector128(char_constants::mm_char_code2_cmp, vl),
        load_vector128(char_constants::mm_char_code0_sub, vl),
        load_vector128(char_constants::mm_char_code1_sub, vl),
        load_vector128(char_constants::mm_char_code2_sub, vl),
        u.data(), end_pos, ec, vl
    );

    return { begin + end_pos, ec };
}

} // namespace detail
} // namespace uuids
} // namespace boost

#endif // defined(BOOST_UUID_USE_RISCV_V)

#endif // BOOST_UUID_DETAIL_FROM_CHARS_RISCV_HPP_INCLUDED

