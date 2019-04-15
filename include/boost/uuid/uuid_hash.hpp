//
// Copyright (c) 2018 James E. King III
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//   http://www.boost.org/LICENCE_1_0.txt)
//
// std::hash support for uuid
//

#ifndef BOOST_UUID_HASH_HPP
#define BOOST_UUID_HASH_HPP

#include <boost/config.hpp>

#ifdef BOOST_HAS_PRAGMA_ONCE
#pragma once
#endif

#ifndef BOOST_NO_CXX11_HDR_FUNCTIONAL

#include <cstddef>
#include <functional>

namespace std
{
    template<>
    struct hash<boost::uuids::uuid>
    {
        std::size_t operator () (const boost::uuids::uuid& value) const BOOST_NOEXCEPT
        {
            std::size_t seed = 0;
            for(uuid::const_iterator i=u.begin(), e=u.end(); i != e; ++i)
            {
                seed ^= static_cast<std::size_t>(*i) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
            }

            return seed;
        }
    };
}

#endif /* !BOOST_NO_CXX11_HDR_FUNCTIONAL */
#endif /* !BOOST_UUID_HASH_HPP */
