#ifndef BOOST_UUID_DETAIL_RANDOM_PROVIDER_HPP_INCLUDED
#define BOOST_UUID_DETAIL_RANDOM_PROVIDER_HPP_INCLUDED

// Copyright (c) 2017 James E. King III
// Copyright (c) 2024 James E. King III
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

#include <random>
#include <memory>
#include <cstdint>

namespace boost {
namespace uuids {
namespace detail {

class random_provider
{
private:

    std::unique_ptr< std::random_device > pdev_;

public:

    random_provider(): pdev_( new std::random_device )
    {
    }

    random_provider( random_provider&& that ) = default;
    random_provider& operator=( random_provider&& that ) = default;

    typedef std::uint32_t result_type;

    // Leverage the provider as a SeedSeq for
    // PseudoRandomNumberGeneration seeding.

    template<class Iter>
    void generate(Iter first, Iter last)
    {
        std::uniform_int_distribution<std::uint32_t> dist( 0, UINT32_MAX );

        for( ; first != last; ++first )
        {
            *first = dist( *pdev_ );
        }
    }

    const char * name() const
    {
        return "std::random_device";
    }
};

} // detail
} // uuids
} // boost

#endif // BOOST_UUID_DETAIL_RANDOM_PROVIDER_HPP_INCLUDED
