// Boost random_generator.hpp header file  ----------------------------------------------//

// Copyright 2010 Andy Tompkins.
// Copyright 2017 James E. King III
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
//   http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_UUID_RANDOM_GENERATOR_HPP
#define BOOST_UUID_RANDOM_GENERATOR_HPP

#include <boost/assert.hpp>
#include <boost/core/enable_if.hpp>
#include <boost/core/null_deleter.hpp>
#include <boost/iterator/function_input_iterator.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/tti/has_member_function.hpp>
#include <boost/uuid/detail/random/random_device.hpp>
#include <boost/uuid/uuid.hpp>
#include <limits>

namespace boost {
namespace uuids {

// put the TTI defined functions that drive
// PseudoRandomNumberGenerator seeding into detail
namespace detail {
    BOOST_TTI_HAS_MEMBER_FUNCTION(seed)
}

//! \brief generates a random-based uuid
//! \param[in]  UniformRandomNumberGenerator  concept defined in the Boost.Random documentation
template <typename UniformRandomNumberGenerator>
class basic_random_generator {
public:
    typedef uuid result_type;

    //! Default constructor acquires a scoped RNG and manages the lifetime of it.
    //! \post if the RNG is a PseudoRandomNumberGenerator with a seed method
    //!       then it is properly seeded.
    basic_random_generator()
        : pURNG(new UniformRandomNumberGenerator)
    {
        seed(*pURNG);
    }

    //! Use a provided RNG by reference.
    //! \param[in]  gen  the provided RNG
    explicit basic_random_generator(UniformRandomNumberGenerator& gen)
        : pURNG(&gen, boost::null_deleter())
    {}

    //! Use a provided RNG by pointer.
    //! \param[in]  pGen  the provided RNG
    explicit basic_random_generator(UniformRandomNumberGenerator* pGen)
        : pURNG(pGen, boost::null_deleter())
    {
        BOOST_ASSERT(pURNG);
    }

    //! Generate a random uuid.
    //! \returns a random uuid
    uuid operator()() const
    {
        uuid u;
        uuid::iterator dest = u.begin();
        do
        {
            typename UniformRandomNumberGenerator::result_type random_value = (*pURNG)();
            std::size_t bytes = sizeof(typename UniformRandomNumberGenerator::result_type);
            std::size_t left = u.end() - dest;
            if (bytes > left)
            {
                bytes = left;
            }
            memcpy(dest, &random_value, bytes);
            dest += bytes;
        } while (dest != u.end());

        // set the uuid variant
        // must be 0b10xxxxxx
        *(u.begin()+8) &= 0xBF;
        *(u.begin()+8) |= 0x80;

        // set the uuid version
        // must be 0b0100xxxx
        *(u.begin()+6) &= 0x4F; //0b01001111
        *(u.begin()+6) |= 0x40; //0b01000000

        return u;
    }

private:

    // Detect whether UniformRandomNumberGenerator has a seed() method which indicates that
    // it is a PseudoRandomNumberGenerator and needs a seed to initialize it.  This allows
    // basic_random_generator to take any type of UniformRandomNumberGenerator and still
    // meet the post-conditions for the default constructor.

    template<class MaybePseudoRandomNumberGenerator>
    typename boost::enable_if<detail::has_member_function_seed<MaybePseudoRandomNumberGenerator, void> >::type
    seed(MaybePseudoRandomNumberGenerator& rng)
    {
        BOOST_UUID_DETAIL_MAKE_RANDOM_DEVICE_TYPE(unsigned int, seed_random_device);

        using boost::function_input_iterator;
        using boost::make_function_input_iterator;

        boost::infinite inf;
        seed_random_device egen;
        function_input_iterator<seed_random_device, boost::infinite> beg = make_function_input_iterator(egen, inf);
        function_input_iterator<seed_random_device, boost::infinite> end = make_function_input_iterator(egen, inf);
        rng.seed(beg, end);
    }

    template<class MaybePseudoRandomNumberGenerator>
    typename boost::disable_if<detail::has_member_function_seed<MaybePseudoRandomNumberGenerator, void> >::type
    seed(MaybePseudoRandomNumberGenerator&)
    {
    }

    shared_ptr<UniformRandomNumberGenerator> pURNG;
};

BOOST_UUID_DETAIL_MAKE_RANDOM_DEVICE_TYPE(uuid, uuid_random_device);

//
// The default random_generator uses a non-deterministic 
// UniformRandomNumberGenerator provided by the operating system.  
// It is the most secure, and fastest random uuid generator for creating a
// small number of uuids with a single generator because it does not need
// expensive seeding to be effective like a PseudoRandomNumberGenerator
// does.
//
typedef basic_random_generator<uuid_random_device> random_generator;

//
// The mt19937 generator uses a mersenne twister to make random 
// uuid generation of many uuids from a single generator more efficient, 
// however the shortcomings of using a PseudoRandomNumberGenerator over
// a non-deterministic UniformRandomNumberGenerator apply.
//
typedef basic_random_generator<mt19937> random_generator_mt19937;

}} // namespace boost::uuids

#endif // BOOST_UUID_RANDOM_GENERATOR_HPP
