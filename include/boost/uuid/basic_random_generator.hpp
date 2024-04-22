#ifndef BOOST_UUID_BASIC_RANDOM_GENERATOR_HPP_INCLUDED
#define BOOST_UUID_BASIC_RANDOM_GENERATOR_HPP_INCLUDED

// Copyright 2010 Andy Tompkins
// Copyright 2017 James E. King III
// Copyright 2024 Peter Dimov
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

#include <boost/uuid/detail/random_provider.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/assert.hpp>
#include <boost/config.hpp>
#include <type_traits>
#include <random>
#include <limits>
#include <cstring>
#include <cstdint>

namespace boost {
namespace uuids {

//! generate a random-based uuid
//! \param[in]  UniformRandomNumberGenerator  see Boost.Random documentation
template <typename UniformRandomNumberGenerator>
class basic_random_generator
{
private:

    typedef std::uniform_int_distribution<unsigned long> distribution_type;

    struct impl
    {
        UniformRandomNumberGenerator* purng;
        distribution_type dist;

        explicit impl(UniformRandomNumberGenerator* purng_arg) :
            purng(purng_arg), dist((std::numeric_limits<unsigned long>::min)(), (std::numeric_limits<unsigned long>::max)())
        {
        }

        virtual ~impl() = default;

        impl(impl const&) = delete;
        impl& operator= (impl const&) = delete;
    };

    struct urng_holder
    {
        UniformRandomNumberGenerator urng;
    };

#if defined(BOOST_MSVC)
#pragma warning(push)
// 'this' : used in base member initializer list
#pragma warning(disable: 4355)
#endif

    struct self_contained_impl :
        public urng_holder,
        public impl
    {
        self_contained_impl() : impl(&this->urng_holder::urng)
        {
        }
    };

#if defined(BOOST_MSVC)
#pragma warning(pop)
#endif

public:

    typedef uuid result_type;

    // default constructor creates the random number generator and
    // if the UniformRandomNumberGenerator is a PseudoRandomNumberGenerator
    // then it gets seeded by a random_provider.
    basic_random_generator() : m_impl(new self_contained_impl())
    {
        // seed the random number generator if it is capable
        seed(static_cast< self_contained_impl* >(m_impl)->urng, 0);
    }

    // keep a reference to a random number generator
    // don't seed a given random number generator
    explicit basic_random_generator(UniformRandomNumberGenerator& gen) : m_impl(new impl(&gen))
    {
    }

    // keep a pointer to a random number generator
    // don't seed a given random number generator
    explicit basic_random_generator(UniformRandomNumberGenerator* gen) : m_impl(new impl(gen))
    {
        BOOST_ASSERT(!!gen);
    }

    basic_random_generator(basic_random_generator&& that) BOOST_NOEXCEPT : m_impl(that.m_impl)
    {
        that.m_impl = 0;
    }

    basic_random_generator& operator= (basic_random_generator&& that) BOOST_NOEXCEPT
    {
        if( this != &that )
        {
            delete m_impl;
            m_impl = that.m_impl;
            that.m_impl = 0;
        }

        return *this;
    }

    ~basic_random_generator()
    {
        delete m_impl;
    }

    result_type operator()()
    {
        result_type u;

        int i = 0;
        unsigned long random_value = m_impl->dist( *m_impl->purng );
        for (uuid::iterator it = u.begin(), end = u.end(); it != end; ++it, ++i) {
            if (i==sizeof(unsigned long)) {
                random_value = m_impl->dist( *m_impl->purng );
                i = 0;
            }

            // static_cast gets rid of warnings of converting unsigned long to boost::uint8_t
            *it = static_cast<uuid::value_type>((random_value >> (i*8)) & 0xFF);
        }

        // set variant
        // must be 0b10xxxxxx
        *(u.begin() + 8) &= 0xBF;
        *(u.begin() + 8) |= 0x80;

        // set version
        // must be 0b0100xxxx
        *(u.begin() + 6) &= 0x4F; //0b01001111
        *(u.begin() + 6) |= 0x40; //0b01000000

		return u;
    }

private:
    // Detect whether UniformRandomNumberGenerator has a seed() method which indicates that
    // it is a PseudoRandomNumberGenerator and needs a seed to initialize it.  This allows
    // basic_random_generator to take any type of UniformRandomNumberGenerator and still
    // meet the post-conditions for the default constructor.

    template<class MaybePseudoRandomNumberGenerator, class En = decltype( std::declval<MaybePseudoRandomNumberGenerator&>().seed() )>
    void seed(MaybePseudoRandomNumberGenerator& rng, int)
    {
        detail::random_provider seeder;
        rng.seed(seeder);
    }

    template<class MaybePseudoRandomNumberGenerator>
    void seed(MaybePseudoRandomNumberGenerator&, long)
    {
    }

    impl* m_impl;
};

}} // namespace boost::uuids

#endif // BOOST_UUID_BASIC_RANDOM_GENERATOR_HPP_INCLUDED
