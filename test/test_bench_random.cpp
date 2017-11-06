/* boost uuid/detail/random benchmark tests
 *
 * Copyright (c) 2010 Steven Watanabe
 * Copyright (c) 2017 James E. King III
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENCE_1_0.txt)
 *
 * $Id$
 */

#include <boost/core/ignore_unused.hpp>
#include <boost/timer/timer.hpp>
#include <boost/predef/os.h>
#include <boost/uuid/detail/random/random_device.hpp>
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <iostream>
#include <limits>

#if defined(BOOST_UUID_LIMITED_BENCH)
// must be a Valgrind, UBsan, or other stressful check job
#define AVG_LOOPS 3
#define GEN_LOOPS 100
#define REUSE_LOOPS 10000
#else
#define AVG_LOOPS 25
#define GEN_LOOPS 10000
#define REUSE_LOOPS 1000000
#endif

template<class Generator>
void auto_timed_generator_ctordtor(size_t count)
{
    boost::timer::auto_cpu_timer t;
    for (size_t i = 0; i < count; ++i)
    {
        Generator gen;
        boost::ignore_unused(gen);
    }
}

template<class Generator>
void auto_timed_generator_novel(size_t count)
{
    boost::timer::auto_cpu_timer t;
    for (size_t i = 0; i < count; ++i)
    {
        Generator gen;
        boost::uuids::uuid u = gen();
        boost::ignore_unused(u);
    }
}

template<class Generator>
void auto_timed_generator_reuse(size_t count)
{
    Generator gen;
    {
        boost::timer::auto_cpu_timer t;
        for (size_t i = 0; i < count; ++i)
        {
            boost::uuids::uuid u = gen();
            boost::ignore_unused(u);
        }
    }
}

template<class Generator>
boost::timer::cpu_times timed_generator(size_t count)
{
    boost::timer::cpu_timer t;
    Generator gen;
    for (size_t i = 0; i < count; ++i)
    {
        boost::uuids::uuid u = gen();
        boost::ignore_unused(u);
    }
    return t.elapsed();
}

int main(int, char*[])
{
    BOOST_UUID_DETAIL_MAKE_RANDOM_DEVICE_TYPE(unsigned int, seed_random_device);
    std::cout << "Selected operating system entropy provider: "
              << seed_random_device().name() << std::endl;

    //
    // Determine the cutoff point where it is more wall-clock efficient to
    // use the bulk generator over the standard one.
    //

    std::cout << "Calculating the number of operator() calls where random_generator" << std::endl;
    std::cout << "is more efficient than random_generator_mt19937..." << std::endl;
    std::cout << "at ";
    size_t minn = (std::numeric_limits<size_t>::max)();
    size_t summ = 0;
    size_t maxx = 0;
    for (size_t i = 0; i < AVG_LOOPS + 1; ++i)  // the first loop is thrown away, see below
    {
        size_t answer = 0;
        for (size_t count = 1; !answer; ++count)
        {
            boost::timer::cpu_times standard = timed_generator<boost::uuids::random_generator>(count);
            boost::timer::cpu_times pseudo = timed_generator<boost::uuids::random_generator_mt19937>(count);
            if (standard.wall > pseudo.wall)
            {
                answer = count;
            }
        }

        // throw away the first answer in case it contains time related to loading
        // or initializing the crypto library being used
        if (i > 0)
        {
            if (minn > answer) minn = answer;
            if (maxx < answer) maxx = answer;
            summ += answer;
            std::cout << answer << " ";
        }
    }
    std::cout << "calls to operator()" << std::endl;
    size_t answer = summ / AVG_LOOPS;
    std::cout << "For this platform, random_generator_mt19937 outperforms "
        << "random_generator after " << answer << " generations (min " << minn << " / max " << maxx << ")."
        << std::endl;
    std::cout << std::endl;

    //
    // Measure ctor/dtor of both
    //
    std::cout << "Construction/destruction time for random_generator "
        << "(" << GEN_LOOPS << " iterations): " << std::endl;
    auto_timed_generator_ctordtor<boost::uuids::random_generator>(GEN_LOOPS);
    std::cout << std::endl;

    std::cout << "Construction/destruction time for random_generator_mt19937 "
        << "(" << GEN_LOOPS << " iterations): " << std::endl;
    auto_timed_generator_ctordtor<boost::uuids::random_generator_mt19937>(GEN_LOOPS);
    std::cout << std::endl;

    //
    // Two common use cases:
    //
    // Use an OS provided RNG which has no seed code but is slower to reuse
    // Use a PRNG which is expensive to seed once but fast to reuse
    //
    // Measure the default selections of the library
    //

    std::cout << "Benchmark boost::uuids::random_generator "
              << "(reused for " << REUSE_LOOPS << " loops):" << std::endl;
    auto_timed_generator_reuse<boost::uuids::random_generator>(REUSE_LOOPS);
    std::cout << std::endl;

    std::cout << "Benchmark boost::uuids::random_generator_mt19937 "
        << "(reused for " << REUSE_LOOPS << " loops):" << std::endl;
    auto_timed_generator_reuse<boost::uuids::random_generator_mt19937>(REUSE_LOOPS);
    std::cout << std::endl;

    std::cout << "Benchmark boost::uuids::random_generator "
              << "(new generator each loop for " << GEN_LOOPS << " loops):" << std::endl;
    auto_timed_generator_novel<boost::uuids::random_generator>(GEN_LOOPS);
    std::cout << std::endl;

    std::cout << "Benchmark boost::uuids::random_generator_mt19937 "
              << "(new generator each loop for " << GEN_LOOPS << " loops):" << std::endl;
    auto_timed_generator_novel<boost::uuids::random_generator_mt19937>(GEN_LOOPS);
    std::cout << std::endl;

    return 0;
}
