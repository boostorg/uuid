#ifndef BOOST_UUID_NAME_GENERATOR_HPP_INCLUDED
#define BOOST_UUID_NAME_GENERATOR_HPP_INCLUDED

// Boost name_generator.hpp header file  -----------------------------//

// Copyright 2010 Andy Tompkins.
// Copyright 2017 James E. King III

// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
//  https://www.boost.org/LICENSE_1_0.txt)

#include <boost/config.hpp>
#include <boost/uuid/name_generator_sha1.hpp>

namespace boost {
namespace uuids {

// The default name generator
typedef name_generator_sha1 name_generator;

// Only provided for compatibility with 1.84 and earlier
typedef name_generator_sha1 name_generator_latest;

} // uuids
} // boost

#endif // BOOST_UUID_NAME_GENERATOR_HPP_INCLUDED
