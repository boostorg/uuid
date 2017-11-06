//  (C) Copyright Andy Tompkins 2010. Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.

// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  libs/uuid/test/test_random_generator.cpp  -------------------------------//

#include <boost/core/ignore_unused.hpp>
#include <boost/detail/lightweight_test.hpp>
#include <boost/random.hpp>
#include <boost/uuid/detail/random/random_device_wincrypt.hpp>
#include <boost/uuid/entropy_error.hpp>
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid.hpp>

template <typename RandomUuidGenerator>
void check_random_generator(RandomUuidGenerator& uuid_gen)
{
    boost::uuids::uuid u1 = uuid_gen();
    boost::uuids::uuid u2 = uuid_gen();

    BOOST_TEST_NE(u1, u2);

    // check variant
    BOOST_TEST_EQ(u1.variant(), boost::uuids::uuid::variant_rfc_4122);

    // version
    BOOST_TEST_EQ(u1.version(), boost::uuids::uuid::version_random_number_based);
}

// This is the example block from the documentation - ensure it works!
void test_examples()
{
    // the default behavior of random_generator favors creation
    // of a small number of uuids from a single instance of a
    // generator as it has a low setup cost:
    boost::uuids::uuid id = boost::uuids::random_generator()();

    // to make a large number of uuids in a more efficient but
    // less secure manner, use random_generator_mt19937 instead.
    // there is a test (test_bench_random) that determines the
    // cut-off point where it is more wall-time efficient to 
    // use mt19937 over standard.
    boost::uuids::random_generator_mt19937 bulkgen;
    for (size_t i = 0; i < 100; ++i)
    {
        boost::uuids::uuid u = bulkgen();
        // do something with u
        boost::ignore_unused(u);
    }

    // you can also use a different random number generator - pass
    // either a reference or a pointer to the random number generator
    boost::random::lagged_fibonacci607 ran; // seed it yourself
    boost::uuids::basic_random_generator
        <boost::random::lagged_fibonacci607> genfib(&ran);
    boost::uuids::uuid r = genfib();

    boost::ignore_unused(id);
    boost::ignore_unused(r);
}

int main(int, char*[])
{
    using namespace boost::uuids;

    // default random number generator
    random_generator uuid_gen1;
    check_random_generator(uuid_gen1);
    
    // specific random number generator
    basic_random_generator<boost::rand48> uuid_gen2;
    check_random_generator(uuid_gen2);
    
    // pass by reference
    boost::ecuyer1988 ecuyer1988_gen;
    basic_random_generator<boost::ecuyer1988> uuid_gen3(ecuyer1988_gen);
    check_random_generator(uuid_gen3);

    // pass by pointer
    boost::lagged_fibonacci607 lagged_fibonacci607_gen;
    basic_random_generator<boost::lagged_fibonacci607> uuid_gen4(&lagged_fibonacci607_gen);
    check_random_generator(uuid_gen4);

    // pseudo
    random_generator_mt19937 bulkgen;
    check_random_generator(bulkgen);

    // make sure default construction seeding is happening
    random_generator_mt19937 b1;
    random_generator_mt19937 b2;
    BOOST_TEST_NE(b1(), b2());

    // there was a bug in basic_random_generator where it did not
    // produce very random numbers.  This checks for that bug.
    uuid u = random_generator()();
    if ( (u.data[4] == u.data[12]) &&
         (u.data[5] == u.data[9] && u.data[5] == u.data[13]) &&
         (u.data[7] == u.data[11] && u.data[7] == u.data[15]) &&
         (u.data[10] == u.data[14]) )
    {
        BOOST_ERROR("basic_random_generator is not producing random uuids");
    }

    // The example code snippet in the documentation
    test_examples();

    // dump 10 of them to cout for observation
    for (size_t i = 0; i < 10; ++i)
    {
        std::cout << random_generator()() << std::endl;
    }

    return boost::report_errors();
}
