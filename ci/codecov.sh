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

set -ex

ci/build.sh
for filename in `find . -type f -regex '.*..pp$'`; 
do 
      gcov -n -o . $filename > /dev/null; 
done
curl -s https://codecov.io/bash > .codecov
chmod +x .codecov
./.codecov || echo "Codecov did not collect coverage reports"
