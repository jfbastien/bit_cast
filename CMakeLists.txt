cmake_minimum_required(VERSION 2.8.11)
project(bit_cast CXX)

# Boilerplate #################################################################

if(NOT CMAKE_BUILD_TYPE)
  message(STATUS "No build type selected, default to Release")
  set(CMAKE_BUILD_TYPE "Release")
endif()
string(TOUPPER "${CMAKE_BUILD_TYPE}" uppercase_CMAKE_BUILD_TYPE)
if(CMAKE_BUILD_TYPE AND
    NOT uppercase_CMAKE_BUILD_TYPE MATCHES "^(DEBUG|RELEASE)$")
  message(FATAL_ERROR "Invalid value for CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")
endif()

function(ADD_COMPILE_FLAG value)
  message(STATUS "Building with ${value}")
  foreach(variable CMAKE_C_FLAGS CMAKE_CXX_FLAGS)
    set(${variable} "${${variable}} ${value}" PARENT_SCOPE)
  endforeach(variable)
endfunction()

function(ADD_LINK_FLAG value)
  message(STATUS "Linking with ${value}")
  foreach(variable CMAKE_EXE_LINKER_FLAGS)
    set(${variable} "${${variable}} ${value}" PARENT_SCOPE)
  endforeach(variable)
endfunction()

# clang doesn't print colored diagnostics when invoked from Ninja.
if(UNIX AND
   CMAKE_CXX_COMPILER_ID STREQUAL "Clang" AND
   CMAKE_GENERATOR STREQUAL "Ninja")
  add_compile_flag("-fcolor-diagnostics")
endif()

include(CheckCXXCompilerFlag)

# Configuration ###############################################################

add_compile_flag("-Wall")
add_compile_flag("-Werror")
add_compile_flag("-Wextra")
if(uppercase_CMAKE_BUILD_TYPE STREQUAL "DEBUG")
  add_compile_flag("-O0")
  add_compile_flag("-g3")
else()
  add_compile_flag("-O2")
endif()

add_compile_flag("-std=c++1z")
add_compile_flag("-DBIT_CAST_USE_ENABLE_IF=1")
add_compile_flag("-DBIT_CAST_USE_STATIC_ASSERT=1")

# Build / test ################################################################

add_executable(test "test.cc")
add_test(test test)
