// Copyright 2024 Peter Dimov
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

#include <boost/uuid/name_generator_sha1.hpp>
#include <boost/uuid/string_generator.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/core/lightweight_test.hpp>

int main()
{
    using namespace boost::uuids;

    // https://stackoverflow.com/questions/5515880/test-vectors-for-uuid-version-5-converting-hash-into-guid-generation-algorith
    // https://de.wikipedia.org/wiki/Universally_Unique_Identifier#Namensbasierte_UUIDs_.28Version_3_und_5.29

    {
        name_generator_sha1 gen( ns::dns() );

        char const* name = "www.example.org";

        uuid u0 = string_generator()( "74738ff5-5367-5958-9aee-98fffdcd1876" );

        uuid u1 = gen( name );
        BOOST_TEST_EQ( u1, u0 );

        uuid u2 = gen( std::string( name ) );
        BOOST_TEST_EQ( u2, u0 );

        uuid u3 = gen( name, std::strlen( name ) );
        BOOST_TEST_EQ( u3, u0 );
    }

    // RFC 4122bis Section A.4

    {
        name_generator_sha1 gen( ns::dns() );

        char const* name = "www.example.com";

        uuid u0 = string_generator()( "2ed6657d-e927-568b-95e1-2665a8aea6a2" );

        uuid u1 = gen( name );
        BOOST_TEST_EQ( u1, u0 );

        uuid u2 = gen( std::string( name ) );
        BOOST_TEST_EQ( u2, u0 );

        uuid u3 = gen( name, std::strlen( name ) );
        BOOST_TEST_EQ( u3, u0 );
    }

    // https://uuid.ramsey.dev/en/stable/rfc4122/version5.html

    {
        name_generator_sha1 gen( ns::url() );

        char const* name = "https://www.php.net";

        uuid u0 = string_generator()( "a8f6ae40-d8a7-58f0-be05-a22f94eca9ec" );

        uuid u1 = gen( name );
        BOOST_TEST_EQ( u1, u0 );

        uuid u2 = gen( std::string( name ) );
        BOOST_TEST_EQ( u2, u0 );

        uuid u3 = gen( name, std::strlen( name ) );
        BOOST_TEST_EQ( u3, u0 );
    }

    // test wide strings

    {
        uuid nmsp = string_generator()( "70a4abc5-80ab-4176-8e11-bc5836b6fef9" );

        name_generator_sha1 gen( nmsp );

        wchar_t const name[] = { 1, 2, 3, 4, 0 };
        unsigned char const name2[] = { 1, 0, 0, 0, 2, 0, 0, 0, 3, 0, 0, 0, 4, 0, 0, 0 };

        uuid u1 = gen( name );
        uuid u2 = gen( name2, sizeof( name2 ) );

        BOOST_TEST_EQ( u1, u2 );
    }

    return boost::report_errors();
}
