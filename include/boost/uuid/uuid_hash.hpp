//
// Copyright (c) 2018 James E. King III
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//   https://www.boost.org/LICENSE_1_0.txt)
//
// std::hash support for uuid
//

#ifndef BOOST_UUID_HASH_HPP_INCLUDED
#define BOOST_UUID_HASH_HPP_INCLUDED

#include <boost/uuid/uuid.hpp>
#include <boost/config.hpp>
#include <typeindex> // cheapest std::hash
#include <cstddef>

namespace std
{
    template<>
    struct hash<boost::uuids::uuid>
    {
        std::size_t operator () (const boost::uuids::uuid& value) const BOOST_NOEXCEPT
        {
            return boost::uuids::hash_value(value);
        }
    };
}

#endif // #ifndef BOOST_UUID_HASH_HPP_INCLUDED
