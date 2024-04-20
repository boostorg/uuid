# Copyright 2018, 2019, 2021 Peter Dimov
# Distributed under the Boost Software License, Version 1.0.
# See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt

cmake_minimum_required(VERSION 3.5...3.20)

project(cmake_subdir_test LANGUAGES CXX)

add_subdirectory(../.. boostorg/uuid)

# `boostdep --brief uuid`

set(deps

# Primary dependencies

assert
config
container_hash
io
predef
random
throw_exception
type_traits
winapi

# Secondary dependencies

describe
mp11
array
core
dynamic_bitset
integer
range
static_assert
system
utility
move
concept_check
conversion
detail
iterator
mpl
optional
preprocessor
regex
tuple
variant2
smart_ptr
function_types
fusion
functional
typeof
function
bind
)

foreach(dep IN LISTS deps)

  add_subdirectory(../../../${dep} boostorg/${dep})

endforeach()

add_executable(quick ../quick.cpp)
target_link_libraries(quick Boost::uuid)

enable_testing()
add_test(quick quick)

add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure -C $<CONFIG>)
