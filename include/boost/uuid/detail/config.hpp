/*
 *          Copyright Andrey Semashev 2013, 2022.
 * Distributed under the Boost Software License, Version 1.0.
 *    (See accompanying file LICENSE_1_0.txt or copy at
 *          https://www.boost.org/LICENSE_1_0.txt)
 */
/*!
 * \file   uuid/detail/config.hpp
 *
 * \brief  This header defines configuration macros for Boost.UUID.
 */

#ifndef BOOST_UUID_DETAIL_CONFIG_HPP_INCLUDED_
#define BOOST_UUID_DETAIL_CONFIG_HPP_INCLUDED_

#include <boost/config.hpp>

#ifdef BOOST_HAS_PRAGMA_ONCE
#pragma once
#endif

#if !defined(BOOST_UUID_NO_SIMD)

#if defined(__GNUC__) && defined(__SSE2__)

// GCC and its pretenders go here
#ifndef BOOST_UUID_USE_SSE2
#define BOOST_UUID_USE_SSE2
#endif

#if defined(__SSE3__) && !defined(BOOST_UUID_USE_SSE3)
#define BOOST_UUID_USE_SSE3
#endif

#if defined(__SSE4_1__) && !defined(BOOST_UUID_USE_SSE41)
#define BOOST_UUID_USE_SSE41
#endif

#if defined(__AVX__) && !defined(BOOST_UUID_USE_AVX)
#define BOOST_UUID_USE_AVX
#endif

#elif defined(_MSC_VER)

#if (defined(_M_X64) || (defined(_M_IX86) && defined(_M_IX86_FP) && _M_IX86_FP >= 2)) && !defined(BOOST_UUID_USE_SSE2)
#define BOOST_UUID_USE_SSE2
#endif

#if defined(__AVX__)
#if !defined(BOOST_UUID_USE_AVX)
#define BOOST_UUID_USE_AVX
#endif
#if !defined(BOOST_UUID_USE_SSE41)
#define BOOST_UUID_USE_SSE41
#endif
#if !defined(BOOST_UUID_USE_SSE3)
#define BOOST_UUID_USE_SSE3
#endif
#if !defined(BOOST_UUID_USE_SSE2)
#define BOOST_UUID_USE_SSE2
#endif
#endif

#endif

// More advanced ISA extensions imply less advanced are also available
#if !defined(BOOST_UUID_USE_SSE41) && defined(BOOST_UUID_USE_AVX)
#define BOOST_UUID_USE_SSE41
#endif

#if !defined(BOOST_UUID_USE_SSE3) && defined(BOOST_UUID_USE_SSE41)
#define BOOST_UUID_USE_SSE3
#endif

#if !defined(BOOST_UUID_USE_SSE2) && defined(BOOST_UUID_USE_SSE3)
#define BOOST_UUID_USE_SSE2
#endif

#if !defined(BOOST_UUID_NO_SIMD) && \
    !defined(BOOST_UUID_USE_AVX) && \
    !defined(BOOST_UUID_USE_SSE41) && \
    !defined(BOOST_UUID_USE_SSE3) && \
    !defined(BOOST_UUID_USE_SSE2)
#define BOOST_UUID_NO_SIMD
#endif

#if !defined(BOOST_UUID_THREEWAY_COMPARE)
#if __cpp_impl_three_way_comparison >= 201907L
#define BOOST_UUID_THREEWAY_COMPARE 1
#else
#define BOOST_UUID_THREEWAY_COMPARE 0
#endif
#endif

#endif // !defined(BOOST_UUID_NO_SIMD)

#endif // BOOST_UUID_DETAIL_CONFIG_HPP_INCLUDED_
