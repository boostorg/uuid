#ifndef BOOST_UUID_MAX_GENERATOR_HPP_INCLUDED
#define BOOST_UUID_MAX_GENERATOR_HPP_INCLUDED

// Copyright 2025 James E. King III
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

// RFC 9562 section 5.10

#include <boost/uuid/uuid.hpp>

namespace boost {
namespace uuids {

// generate a max uuid
struct max_generator
{
    using result_type = uuid;
    
    uuid operator()() const noexcept
    {
        return {{ 
            0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
            0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
        }};
    }
};

inline uuid max_uuid() noexcept
{
    return {{ 
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
    }};
}

}} // namespace boost::uuids

#endif // BOOST_UUID_MAX_GENERATOR_HPP_INCLUDED
