// Boost random_generator.hpp header file  ----------------------------------------------//

// Copyright 2010 Andy Tompkins.
// Copyright 2017 James E. King III
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// https://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_UUID_RANDOM_GENERATOR_HPP
#define BOOST_UUID_RANDOM_GENERATOR_HPP

#include <boost/uuid/detail/random_provider.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/assert.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int.hpp>
#include <boost/random/variate_generator.hpp>
#include <boost/config.hpp>
#include <limits>
#include <type_traits>

namespace boost {
namespace uuids {

namespace detail {
    template<class U>
    U& set_uuid_random_vv(U& u)
    {
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
}

//! generate a random-based uuid
//! \param[in]  UniformRandomNumberGenerator  see Boost.Random documentation
template <typename UniformRandomNumberGenerator>
class basic_random_generator
{
private:

    typedef uniform_int<unsigned long> distribution_type;
    typedef variate_generator<UniformRandomNumberGenerator*, distribution_type> generator_type;

    struct impl
    {
        generator_type generator;

        explicit impl(UniformRandomNumberGenerator* purng_arg) :
            generator(purng_arg, distribution_type((std::numeric_limits<unsigned long>::min)(), (std::numeric_limits<unsigned long>::max)()))
        {
        }

        virtual ~impl() {}

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

        int i=0;
        unsigned long random_value = m_impl->generator();
        for (uuid::iterator it = u.begin(), end = u.end(); it != end; ++it, ++i) {
            if (i==sizeof(unsigned long)) {
                random_value = m_impl->generator();
                i = 0;
            }

            // static_cast gets rid of warnings of converting unsigned long to boost::uint8_t
            *it = static_cast<uuid::value_type>((random_value >> (i*8)) & 0xFF);
        }

        return detail::set_uuid_random_vv(u);
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

//! \brief a far less complex random generator that uses
//!        operating system provided entropy which will
//!        satisfy the majority of use cases
class random_generator_pure
{
public:

    typedef uuid result_type;

    random_generator_pure() = default;

    random_generator_pure(random_generator_pure&& that) = default;
    random_generator_pure& operator= (random_generator_pure&& that) = default;

    //! \returns a random, valid uuid
    //! \throws entropy_error
    result_type operator()()
    {
        result_type result;
        prov_.get_random_bytes(&result, sizeof(result_type));
        return detail::set_uuid_random_vv(result);
    }

private:

    detail::random_provider prov_;
};

#if defined(BOOST_UUID_RANDOM_GENERATOR_COMPAT)
typedef basic_random_generator<mt19937> random_generator;
#else
typedef random_generator_pure random_generator;
typedef basic_random_generator<mt19937> random_generator_mt19937;
#endif

}} // namespace boost::uuids

#endif // BOOST_UUID_RANDOM_GENERATOR_HPP
