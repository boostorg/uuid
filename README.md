Uuid, part of collection of the [Boost C++ Libraries](http://github.com/boostorg), provides a C++ wrapper around [RFC-4122](http://www.ietf.org/rfc/rfc4122.txt) UUIDs.

### License

Distributed under the [Boost Software License, Version 1.0](http://www.boost.org/LICENSE_1_0.txt).

### Properties

* c++03
* header-only

### Build Status

@               | Build         | Coverity | Regression Tests
--------------- | ------------- | -------- | ----------------
[develop branch](https://github.com/boostorg/uuid/tree/develop): | [![Build Status](https://travis-ci.org/boostorg/uuid.svg?branch=develop)](https://travis-ci.org/boostorg/uuid) [![Build status](https://ci.appveyor.com/api/projects/status/nuihr6s92fjb9gwy/branch/develop?svg=true)](https://ci.appveyor.com/project/boostorg/uuid/branch/develop) | [![Coverity Scan Build Status](https://scan.coverity.com/projects/13982/badge.svg)](https://scan.coverity.com/projects/boostorg-uuid) | [Enter the Matrix](http://www.boost.org/development/tests/develop/developer/uuid.html)
[master branch](https://github.com/boostorg/uuid/tree/master):  | [![Build Status](https://travis-ci.org/boostorg/uuid.svg?branch=master)](https://travis-ci.org/boostorg/uuid) [![Build status](https://ci.appveyor.com/api/projects/status/nuihr6s92fjb9gwy?svg=true)](https://ci.appveyor.com/project/boostorg/uuid/branch/master) | N/A (header-only) | [Enter the Matrix](http://www.boost.org/development/tests/master/developer/uuid.html)

### Example

    // Copyright 2017 James E. King, III
    // Distributed under the Boost Software License, Version 1.0. 
    // (See http://www.boost.org/LICENSE_1_0.txt)
    // mkuuid.cpp example
    
    #include <boost/lexical_cast.hpp>
    #include <boost/uuid/random_generator.hpp>
    #include <boost/uuid/uuid_io.hpp>
    #include <iostream>
    
    int main(void)
    {
        boost::uuids::random_generator gen;
        std::cout << boost::lexical_cast<std::string>(gen()) << std::endl;
        return 0;
    }
    
    ----
    
    $ clang++ -ansi -Wall -Wextra -std=c++03 -O3 mkuuid.cpp -o mkuuid
    $ ./mkuuid
    2c186eb0-89cf-4a3c-9b97-86db1670d5f4
    $ ./mkuuid
    a9d3fbb9-0383-4389-a8a8-61f6629f90b6

### Directories

* **doc** - Dcumentation
* **include** - Interface headers of Uuid
* **test** - Uuid unit tests

### More information

* [Documentation](http://boost.org/libs/uuid)
* [Ask questions](http://stackoverflow.com/questions/ask?tags=c%2B%2B,boost,boost-uuid)
* [Report bugs](https://github.com/boostorg/uuid/issues): Be sure to mention Boost version, platform and compiler you're using. A small compilable code sample to reproduce the problem is always good as well.
* Submit your patches as pull requests against **develop** branch. Note that by submitting patches you agree to license your modifications under the [Boost Software License, Version 1.0](http://www.boost.org/LICENSE_1_0.txt).
* Discussions about the library are held on the [Boost developers mailing list](http://www.boost.org/community/groups.html#main). Be sure to read the [discussion policy](http://www.boost.org/community/policy.html) before posting and add the `[uuid]` tag at the beginning of the subject line.

