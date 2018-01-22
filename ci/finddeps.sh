# A script to find direct depenedencies (for documentation)
#
#  Copyright (c) 2018 James E. King III
#
#  Use, modification, and distribution are subject to the
#  Boost Software License, Version 1.0. (See accompanying file
#  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#!/bin/bash

set -e

# takes one or more arguments - subdirectories to calculate depenedencies for
function finddeps
{
  local hdrs=`find $* -type f -exec grep 'include' {} \; | grep 'boost/' | grep -v 'date_time' | sed 's/</\"/g' | sed 's/>/\"/g' | cut -d\" -f2 | sort | uniq`
  local deps=`for hdr in $hdrs; do local hdronly=${hdr##*/}; find .. -name $hdronly -print | grep $hdr | cut -d'/' -f2; done`
  echo $deps | xargs -n1 | sort -u
}

finddeps $*
