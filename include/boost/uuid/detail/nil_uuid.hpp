#ifndef BOOST_UUID_DETAIL_NIL_UUID_INCLUDED
#define BOOST_UUID_DETAIL_NIL_UUID_INCLUDED

// Copyright 2025 Peter Dimov
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

#include <boost/uuid/uuid.hpp>

namespace boost {
namespace uuids {

constexpr uuid nil_uuid() noexcept
{
    return {};
}

}} // namespace boost::uuids

#endif // #ifndef BOOST_UUID_DETAIL_NIL_UUID_INCLUDED
