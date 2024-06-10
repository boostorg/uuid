// Copyright 2024 Peter Dimov
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

#include <boost/uuid/uuid.hpp>
#include <boost/core/lightweight_test.hpp>
#include <cstddef>

using namespace boost::uuids;

int main()
{
    BOOST_TEST_LE( alignof(uuid), alignof(std::max_align_t) );

#if defined(__STDCPP_DEFAULT_NEW_ALIGNMENT__)

    BOOST_TEST_LE( alignof(uuid), __STDCPP_DEFAULT_NEW_ALIGNMENT__ );

#endif

    return boost::report_errors();
}
