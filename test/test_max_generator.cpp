// Copyright (C) 2025 James E. King III
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// https://www.boost.org/LICENSE_1_0.txt)

//  libs/uuid/test/test_max_generator.cpp  -------------------------------//

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/max_generator.hpp>
#include <boost/uuid/nil_generator.hpp>
#include <boost/detail/lightweight_test.hpp>

int main(int, char*[])
{
    using namespace boost::uuids;

    uuid uunil = nil_generator()();
    uuid uumax = max_generator()();
    uuid expected = {{ 
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
    }};
    BOOST_TEST_EQ(uumax, expected);
    BOOST_TEST_LT(uunil, uumax);

    uuid u3 = max_uuid();
    BOOST_TEST_EQ(u3, expected);

    return boost::report_errors();
}
