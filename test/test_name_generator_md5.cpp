// Copyright 2024 Peter Dimov
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

#include <boost/uuid/name_generator_md5.hpp>
#include <boost/uuid/string_generator.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/core/lightweight_test.hpp>

int main()
{
    using namespace boost::uuids;

    // RFC 4122 Appendix B + Errata 1352

    {
        name_generator_md5 gen( ns::dns() );

        char const* name = "www.widgets.com";

        uuid u0 = string_generator()( "3d813cbb-47fb-32ba-91df-831e1593ac29" );

        uuid u1 = gen( name );
        BOOST_TEST_EQ( u1, u0 );

        uuid u2 = gen( std::string( name ) );
        BOOST_TEST_EQ( u2, u0 );

        uuid u3 = gen( name, std::strlen( name ) );
        BOOST_TEST_EQ( u3, u0 );
    }

    // RFC 4122bis Section A.2

    {
        name_generator_md5 gen( ns::dns() );

        char const* name = "www.example.com";

        uuid u0 = string_generator()( "5df41881-3aed-3515-88a7-2f4a814cf09e" );

        uuid u1 = gen( name );
        BOOST_TEST_EQ( u1, u0 );

        uuid u2 = gen( std::string( name ) );
        BOOST_TEST_EQ( u2, u0 );

        uuid u3 = gen( name, std::strlen( name ) );
        BOOST_TEST_EQ( u3, u0 );
    }

    // https://uuid.ramsey.dev/en/stable/rfc4122/version3.html

    {
        name_generator_md5 gen( ns::url() );

        char const* name = "https://www.php.net";

        uuid u0 = string_generator()( "3f703955-aaba-3e70-a3cb-baff6aa3b28f" );

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

        name_generator_md5 gen( nmsp );

        wchar_t const name[] = { 1, 2, 3, 4, 0 };
        unsigned char const name2[] = { 1, 0, 0, 0, 2, 0, 0, 0, 3, 0, 0, 0, 4, 0, 0, 0 };

        uuid u1 = gen( name );
        uuid u2 = gen( name2, sizeof( name2 ) );

        BOOST_TEST_EQ( u1, u2 );
    }

    return boost::report_errors();
}
