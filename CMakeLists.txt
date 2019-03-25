# Copyright (c) 2017 ASMlover. All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
#
#  * Redistributions of source code must retain the above copyright
#    notice, this list ofconditions and the following disclaimer.
#
#  * Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in
#    the documentation and/or other materialsprovided with the
#    distribution.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
# FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
# COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
# INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
# BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
# LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
# CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
# LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
# ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.

cmake_minimum_required(VERSION 3.0)
project(ChaosPROJ CXX)

if (POLICY CMP0025)
  cmake_policy(SET CMP0025 NEW)
endif()
if (POLICY CMP0054)
  cmake_policy(SET CMP0054 NEW)
endif()

# set ChaosPROJ version
set(CHAOS_MAJOR_VERSION "0")
set(CHAOS_MINOR_VERSION "1")
set(CHAOS_PATCH_VERSION "0")
set(CHAOS_VERSION_STRING "${CHAOS_MAJOR_VERSION}.${CHAOS_MINOR_VERSION}.${CHAOS_PATCH_VERSION}")

# compile in release with debug info mode by default
if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release" CACHE STRING "choose build type [Debug|Release|RelWithDebInfo|MinSizeRel]" FORCE)
endif()
message(STATUS "`${PROJECT_NAME}` CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")

# build all binaries into a specified directory
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# set building options
option(CHAOS_BUILD_TEST "build Chaos examples and unittests" ON)
option(CHAOS_USE_CPP11 "implementation some module use cpp11 library" OFF)

# set building CMAKE_CXX_FLAGS
if (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
  if (MSVC_VERSION LESS 1910)
    message(FATAL_ERROR "you need Visual Studio 2017 or later")
    return()
  endif()
  if (CHAOS_USE_CPP11)
    add_compile_options(/DCHAOS_USE_CPP11)
  endif()

  add_definitions(
    /DUNICODE
    /D_UNICODE
    /D_CRT_SECURE_NO_WARNINGS
    /D_CRT_NONSTDC_NO_WARNINGS
    /wd4091
    /wd4244
    /wd4800
    /wd4996)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W3 /GF /GS /Gs /Zi /EHsc /std:c++latest")
  set(CMAKE_CXX_FLAGS_DEBUG "/D_DEBUG /MDd /Od")
  set(CMAKE_CXX_FLAGS_RELEASE "/DNDEBUG /MD /Ox")
else()
  if (CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS "7.1.0")
      message(FATAL_ERROR "you need GCC-7.1.0 or later")
      return()
    endif()
  elseif (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS "5.0")
      message(FATAL_ERROR "you need Clang-5.0 or later")
      return()
    endif()
  endif()
  if (CHAOS_USE_CPP11)
    add_compile_options(-DCHAOS_USE_CPP11)
  endif()

  add_definitions(
    -Wno-format-truncation)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1z -Wall -Werror -march=native")
  set(CMAKE_CXX_FLAGS_DEBUG "-g -ggdb -O0")
  set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")
endif()
string(TOUPPER ${CMAKE_BUILD_TYPE} BUILD_TYPE)
message(STATUS "`${PROJECT_NAME}` CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_${BUILD_TYPE}}")

macro(source_group_by_dir source_files)
  # only call for MSVC
  set(current_dir ${CMAKE_CURRENT_SOURCE_DIR})
  foreach(fname ${${source_files}})
    string(REGEX REPLACE ${current_dir}/\(.*\) \\1 fpath ${fname})
    string(REGEX REPLACE "\(.*\)/.*" \\1 group_name ${fpath})
    string(COMPARE EQUAL ${fpath} ${group_name} nogroup)
    string(REPLACE "/" "\\" group_name ${group_name})
    if (nogroup)
      set(group_name "\\")
    endif()
    source_group(${group_name} FILES ${fname})
  endforeach()
endmacro()

include_directories(${PROJECT_SOURCE_DIR})
add_subdirectory(Chaos)
if (CHAOS_BUILD_TEST)
  add_subdirectory(Tests)
endif()
