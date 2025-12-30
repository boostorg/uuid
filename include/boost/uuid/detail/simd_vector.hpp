#ifndef BOOST_UUID_DETAIL_SIMD_VECTOR_HPP_INCLUDED
#define BOOST_UUID_DETAIL_SIMD_VECTOR_HPP_INCLUDED

// Copyright 2025 Andrey Semashev
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

#include <type_traits>
#include <boost/config.hpp>

namespace boost {
namespace uuids {
namespace detail {

//! A helper to define vector constants
template< typename T, unsigned int ByteSize >
union simd_vector
{
    static_assert(((ByteSize - 1u) & ByteSize) == 0u, "Boost.UUID: ByteSize must be a power of two in simd_vector");

    BOOST_MAY_ALIAS T elements[ByteSize / sizeof(T)];
    alignas(ByteSize) unsigned char bytes[ByteSize];

    template<
        typename Vector,
        typename = typename std::enable_if< sizeof(Vector) <= ByteSize >::type
    >
    BOOST_FORCEINLINE operator Vector () const noexcept { return get< Vector >(); }

    template< typename Vector >
    BOOST_FORCEINLINE typename std::enable_if< sizeof(Vector) <= ByteSize, Vector >::type get() const noexcept
    {
        using vector_type = typename std::remove_cv< typename std::remove_reference< Vector >::type >::type;
        return *reinterpret_cast< const vector_type* >(bytes);
    }
};

template< typename T >
using simd_vector128 = simd_vector< T, 16u >;
template< typename T >
using simd_vector256 = simd_vector< T, 32u >;
template< typename T >
using simd_vector512 = simd_vector< T, 64u >;

} // namespace detail
} // namespace uuids
} // namespace boost

#endif // BOOST_UUID_DETAIL_SIMD_VECTOR_HPP_INCLUDED
