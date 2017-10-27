#! /bin/bash
#
# Copyright 2017 James E. King, III
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at
#      http://www.boost.org/LICENSE_1_0.txt)
#
# Bash script to run in travis to perform codecov.io integration
#

# assumes a bjam variant of "profile"
# assumes an environment variable $SELF is the boost project name

set -ex

ci/build.sh

# coverage files are in ../../b2 from this location
lcov --base-directory `pwd` --directory ../../bin.v2 --capture --output-file all.info

# all.info contains all the coverage info for all projects - limit to ours
lcov --extract coverage.info '*/$SELF/*' --output-file coverage.info

# dump a summary just for grins
lcov --list coverage.info

# upload to codecov.io
curl -s https://codecov.io/bash > .codecov
chmod +x .codecov
./.codecov || echo "Codecov did not collect coverage reports"
