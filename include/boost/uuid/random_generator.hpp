#ifndef BOOST_UUID_RANDOM_GENERATOR_HPP_INCLUDED
#define BOOST_UUID_RANDOM_GENERATOR_HPP_INCLUDED

// Boost random_generator.hpp header file  ----------------------------------------------//

// Copyright 2010 Andy Tompkins.
// Copyright 2017 James E. King III
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// https://www.boost.org/LICENSE_1_0.txt)

#include <boost/uuid/basic_random_generator.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/detail/random_provider.hpp>
#include <memory>
#include <cstring>
#include <cstdint>

namespace boost {
namespace uuids {

//! \brief a far less complex random generator that uses
//!        operating system provided entropy which will
//!        satisfy the majority of use cases
class random_generator_pure
{
private:

    std::unique_ptr<detail::random_provider> prov_;

public:

    typedef uuid result_type;

    random_generator_pure(): prov_( new detail::random_provider )
    {
    }

    random_generator_pure(random_generator_pure&& that) = default;
    random_generator_pure& operator= (random_generator_pure&& that) = default;

    //! \returns a random, valid uuid
    //! \throws entropy_error
    result_type operator()()
    {
        result_type result;

        std::uint32_t tmp[ 4 ];
        prov_->generate( tmp + 0, tmp + 4 );

        std::memcpy( result.data, tmp, 16 );

        // set variant
        // must be 0b10xxxxxx
        *(result.begin() + 8) &= 0xBF;
        *(result.begin() + 8) |= 0x80;

        // set version
        // must be 0b0100xxxx
        *(result.begin() + 6) &= 0x4F; //0b01001111
        *(result.begin() + 6) |= 0x40; //0b01000000

        return result;
    }
};

#if defined(BOOST_UUID_RANDOM_GENERATOR_COMPAT)
typedef basic_random_generator<std::mt19937> random_generator;
#else
typedef random_generator_pure random_generator;
typedef basic_random_generator<std::mt19937> random_generator_mt19937;
#endif

}} // namespace boost::uuids

#endif // BOOST_UUID_RANDOM_GENERATOR_HPP_INCLUDED
