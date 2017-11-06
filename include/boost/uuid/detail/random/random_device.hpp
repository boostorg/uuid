/* boost uuid/detail/random/random_device.hpp header file
 *
 * Copyright Jens Maurer 2000
 * Copyright 2007 Andy Tompkins.
 * Copyright Steven Watanabe 2010-2011
 * Copyright 2017 James E. King, III
 *
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * $Id$
 *
 * Revision history
 *  2000-02-18  Portability fixes (thanks to Beman Dawes)
 *  2017-09-04  Made header-only, configurable entropy size, UWP compatible
 *  2017-11-04  Moved into uuid/detail/random
 */

//
// This header-only refactoring of boost::random_device was not approved for
// inclusion in boost::random so it lives in boost::uuids::detail to satisfy
// the header-only nature of Boost.Uuid and replaces the older, more complex
// seed_rng.hpp implementation that was here.  Specifically:
//
// seed_rng was too complex in generating a seed, stuffing lots of information
// into a hash and then extracting a value, rather than relying on the random
// device that the platform provides
//
// seed_rng was not checking return codes on system calls which could lead to
// silect failures
//
// seed_rng did not support Universal Windows Platform (store family)
//

#ifndef BOOST_UUID_RANDOM_DEVICE_HPP
#define BOOST_UUID_RANDOM_DEVICE_HPP

#include <boost/config.hpp>
#include <boost/limits.hpp>
#include <string>

#include <boost/uuid/detail/random/random_device_bcrypt.hpp>
#include <boost/uuid/detail/random/random_device_cloudabi.hpp>
#include <boost/uuid/detail/random/random_device_file.hpp>
#include <boost/uuid/detail/random/random_device_wincrypt.hpp>

namespace boost {
namespace uuids {
namespace detail {
namespace random {

/**
 * Class \random_device models a \nondeterministic_random_number_generator.
 * It uses one or more implementation-defined stochastic processes to
 * generate a sequence of uniformly distributed non-deterministic random
 * numbers. For those environments where a non-deterministic random number
 * generator is not available, class random_device must not be implemented. See
 *
 *  @blockquote
 *  "Randomness Recommendations for Security", D. Eastlake, S. Crocker,
 *  J. Schiller, Network Working Group, RFC 1750, December 1994
 *  @endblockquote
 *
 * for further discussions. 
 *
 * @xmlnote
 * Some operating systems abstract the computer hardware enough
 * to make it difficult to non-intrusively monitor stochastic processes.
 * However, several do provide a special device for exactly this purpose.
 * It seems to be impossible to emulate the functionality using Standard
 * C++ only, so users should be aware that this class may not be available
 * on all platforms.
 * @endxmlnote
 *
 * <b>Implementation Note for Unix</b>
 *
 * \random_device uses the /dev/urandom pseudo-device for entropy
 *
 * <b>Implementation Note for Windows</b>
 * 
 * When targeting Windows Desktop or System applications with UWP, or when
 * not using UWP (older SDKs), the traditional wincrypt provider is used to
 * get entropy.
 *
 * For all other windows targets, for example Windows Store UWP targets, 
 * bcrypt is used to acquire entropy.
 *
 * There are some platform/SDK combinations where it is not possible to have
 * a random device on Windows - for example Windows SDK 8.x with a non-desktop
 * target, since bcrypt is specified as desktop-only in SDK 8.x and expanded to
 * other partitions in SDK 10.x.
 *
 * <b>Compile-Time Definitions</b>
 *
 * [Windows] To force use of wincrypt over bcrypt on desktop platforms,
 * define BOOST_UUID_RANDOM_DEVICE_FORCE_WINCRYPT
 *
 * [Windows] To disable automatic link libraries being added by providers,
 * define BOOST_UUID_RANDOM_DEVICE_NO_LIB
 *
 * <b>Exceptions</b>
 *
 * Errors will result in a boost::uuids::entropy_error exception.
 *
 * <b>Concepts</b>
 *
 * A RandomDeviceProvider must:
 *   - provide a result_type type definition
 *   - provide an operator() implementation returning a result_type
 *   - provide a std::string name() const method
 *
 * A UniformRandomNumberGenerator must (as documented):
 *   - provide a min()
 *   - provide a max()
 *   - provide an operator()
 *
 * The common parts of a UniformRandomNumberGenerator are provided by
 * basic_random_device.  The operator() is provided by the platform-
 * specific RandomDeviceProvider (bcrypt, file, wincrypt).
 */
template<class RandomDeviceProvider>
class basic_random_device : public RandomDeviceProvider
{
public:
    basic_random_device() 
        : RandomDeviceProvider()
    {
    }

    typedef typename RandomDeviceProvider::result_type result_type;

    /** Returns the smallest value that the \random_device can produce. */
    static result_type min BOOST_PREVENT_MACRO_SUBSTITUTION () 
    { 
        return (std::numeric_limits<result_type>::min)(); 
    }

    /** Returns the largest value that the \random_device can produce. */
    static result_type max BOOST_PREVENT_MACRO_SUBSTITUTION() 
    { 
        return (std::numeric_limits<result_type>::max)();
    }
};

} // namespace random
} // namespace detail
} // namespace uuids
} // namespace boost

//
// Define the macro BOOST_UUID_DETAIL_MAKE_RANDOM_DEVICE_TYPE which allows the caller
// to create a type definition for a random device across supported platforms using a 
// configurable entropy size.
//

#if BOOST_PLAT_CLOUDABI
# define BOOST_UUID_DETAIL_MAKE_RANDOM_DEVICE_TYPE(EntropyType, DeviceName) \
         typedef boost::uuids::detail::random::basic_random_device \
            <boost::uuids::detail::random::random_device_cloudabi<EntropyType> > DeviceName
#elif defined(BOOST_WINDOWS)
#if defined(BOOST_UUID_RANDOM_DEVICE_BCRYPT_AVAILABLE) && !defined(BOOST_UUID_RANDOM_DEVICE_FORCE_WINCRYPT)
#  define BOOST_UUID_DETAIL_MAKE_RANDOM_DEVICE_TYPE(EntropyType, DeviceName) \
          typedef boost::uuids::detail::random::basic_random_device \
            <boost::uuids::detail::random::random_device_bcrypt<EntropyType> > DeviceName
# else
#  define BOOST_UUID_DETAIL_MAKE_RANDOM_DEVICE_TYPE(EntropyType, DeviceName) \
          typedef boost::uuids::detail::random::basic_random_device \
            <boost::uuids::detail::random::random_device_wincrypt<EntropyType> > DeviceName
# endif
#else
# define BOOST_UUID_DETAIL_MAKE_RANDOM_DEVICE_TYPE(EntropyType, DeviceName) \
         typedef boost::uuids::detail::random::basic_random_device \
            <boost::uuids::detail::random::random_device_file<EntropyType> > DeviceName
#endif

#endif /* BOOST_UUID_RANDOM_DEVICE_HPP */
