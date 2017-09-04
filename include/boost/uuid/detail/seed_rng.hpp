// Boost seed_rng.hpp header file  ----------------------------------------------//

// Copyright 2007 Andy Tompkins.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// Revision History
//  09 Nov 2007 - Initial Revision
//  25 Feb 2008 - moved to namespace boost::uuids::detail
//  28 Nov 2009 - disabled deprecated warnings for MSVC
//  28 Jul 2014 - fixed valgrind warnings and better entropy sources for MSVC
//  04 Sep 2017 - removed implementation in favor of boost::random_device to
//                ensure compatibility with all platforms

#ifndef BOOST_UUID_SEED_RNG_HPP
#define BOOST_UUID_SEED_RNG_HPP

#include <algorithm>
#include <boost/config.hpp>
#include <boost/random/random_device.hpp>
#include <boost/random/seed_seq.hpp>
#include <vector>

namespace boost {
namespace uuids {
namespace detail {

// seed() seeds a random number generator with good seed values
template <typename UniformRandomNumberGenerator>
inline void seed(UniformRandomNumberGenerator& rng)
{
    BOOST_STATIC_CONSTEXPR size_t entropy_len = 32; // 256 bits of entropy
    boost::random::random_device rd;
    std::vector<unsigned int> entropy(entropy_len);
    std::generate(entropy.begin(), entropy.end(), std::ref(rd));
    boost::random::seed_seq seq(entropy.begin(), entropy.end());
    rng.seed(seq);
}

// random_device itself does not / can not be seeded
template <>
inline void seed<boost::random::random_device>(boost::random::random_device&) {}

}}} //namespace boost::uuids::detail

#endif
