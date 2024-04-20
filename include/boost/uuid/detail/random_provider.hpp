//
// Copyright (c) 2017 James E. King III
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//   https://www.boost.org/LICENSE_1_0.txt)
//
// Platform-specific random entropy provider
//

#ifndef BOOST_UUID_DETAIL_RANDOM_PROVIDER_HPP
#define BOOST_UUID_DETAIL_RANDOM_PROVIDER_HPP

#include <boost/uuid/entropy_error.hpp>
#include <boost/uuid/detail/static_assert.hpp>
#include <boost/config.hpp>
#include <iterator>
#include <type_traits>
#include <limits>
#include <cstdint>
#include <climits>

// Detection of the platform is separated from inclusion of the correct
// header to facilitate mock testing of the provider implementations.

#include <boost/uuid/detail/random_provider_detect_platform.hpp>
#include <boost/uuid/detail/random_provider_include_platform.hpp>


namespace boost {
namespace uuids {
namespace detail {

//! \brief Contains code common to all random_provider implementations.
//! \note  random_provider_base is required to provide this method:
//!        void get_random_bytes(void *buf, size_t siz);
//! \note  noncopyable because of some base implementations so
//!        this makes it uniform across platforms to avoid any  
//!        porting surprises
class random_provider :
    public detail::random_provider_base
{
public:

    random_provider() = default;

    random_provider(random_provider&& that) = default;
    random_provider& operator= (random_provider&& that) = default;

    //! Leverage the provider as a SeedSeq for
    //! PseudoRandomNumberGeneration seeing.
    //! \note: See Boost.Random documentation for more details
    template<class Iter>
    void generate(Iter first, Iter last)
    {
        typedef typename std::iterator_traits<Iter>::value_type value_type;
        BOOST_UUID_STATIC_ASSERT(std::is_integral<value_type>::value);
        BOOST_UUID_STATIC_ASSERT(std::is_unsigned<value_type>::value);
        BOOST_UUID_STATIC_ASSERT(sizeof(value_type) * CHAR_BIT >= 32);

        for (; first != last; ++first)
        {
            get_random_bytes(&*first, sizeof(*first));
            *first &= (std::numeric_limits<boost::uint32_t>::max)();
        }
    }

    //! Return the name of the selected provider
    const char * name() const
    {
        return BOOST_UUID_RANDOM_PROVIDER_STRINGIFY(BOOST_UUID_RANDOM_PROVIDER_NAME);
    }
};

} // detail
} // uuids
} // boost

#endif // BOOST_UUID_DETAIL_RANDOM_PROVIDER_HPP
