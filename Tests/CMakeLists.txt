# Copyright (c) 2016 ASMlover. All rights reserved.
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

project(chaos.test CXX)

if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif()

if (CMAKE_SIZEOF_VOID_P EQUAL 8)
  set(ARCH_DIR "64-bit")
elseif (CMAKE_SIZEOF_VOID_P EQUAL 4)
  set(ARCH_DIR "32-bit")
else()
  message(FATAL_ERROR "unsupport architecture, CMake will exit")
  return()
endif()

set(CMAKE_DEBUG_POSTFIX "_d" CACHE STRING "debug postfix, usually `_d`" FORCE)
set(CMAKE_RELEASE_POSTFIX "" CACHE STRING "release postfix, usually empty" FORCE)

if (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
  if (MSVC_VERSION LESS 1900)
    message(FATAL_ERROR "you need Visual Studio 2015 or later")
  endif()
  add_definitions(
    -D_UNICODE
    -D_CRT_SECURE_NO_WARNINGS
    -D_CRT_NONSTDC_NO_WARNINGS
    -wd4091
    -wd4244
    -wd4996)
  set(CMAKE_CXX_FLAGS "-W3 -GF -GS -Gs -Zi -EHsc")
elseif (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  add_compile_options(-std=c++11)
  set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-g -O2 -Wall")
endif()

include_directories(${PROJECT_SOURCE_DIR}/../)

file(GLOB_RECURSE TEST_SOURCES ${PROJECT_SOURCE_DIR}/*.cc)
file(GLOB_RECURSE COMMON_SOURCES
  ${PROJECT_SOURCE_DIR}/../Chaos/Codecs/*.cc
  ${PROJECT_SOURCE_DIR}/../Chaos/Container/*.cc
  ${PROJECT_SOURCE_DIR}/../Chaos/Datetime/*.cc
  ${PROJECT_SOURCE_DIR}/../Chaos/Except/*.cc
  ${PROJECT_SOURCE_DIR}/../Chaos/File/*.cc
  ${PROJECT_SOURCE_DIR}/../Chaos/Logging/*.cc
  ${PROJECT_SOURCE_DIR}/../Chaos/Memory/*.cc
  ${PROJECT_SOURCE_DIR}/../Chaos/Unittest/*.cc
  ${PROJECT_SOURCE_DIR}/../Chaos/Utility/*.cc
  )
file(GLOB CROCESS_SOURCES
  ${PROJECT_SOURCE_DIR}/../Chaos/Concurrent/*.cc
  ${PROJECT_SOURCE_DIR}/../Chaos/Concurrent/Unexposed/*.cc
  ${PROJECT_SOURCE_DIR}/../Chaos/IO/*.cc
  ${PROJECT_SOURCE_DIR}/../Chaos/Kern/*.cc
  )
set(ALL_SOURCES ${TEST_SOURCES} ${COMMON_SOURCES} ${CROCESS_SOURCES})
if (CMAKE_SYSTEM_NAME MATCHES "Windows")
  file(GLOB_RECURSE WINDOWS_SOURCES
    ${PROJECT_SOURCE_DIR}/../Chaos/Concurrent/Windows/*.cc
    ${PROJECT_SOURCE_DIR}/../Chaos/IO/Windows/*.cc
    ${PROJECT_SOURCE_DIR}/../Chaos/Kern/Windows/*.cc)
  set(ALL_SOURCES ${ALL_SOURCES} ${WINDOWS_SOURCES})
else()
  file(GLOB_RECURSE POSIX_SOURCES
    ${PROJECT_SOURCE_DIR}/../Chaos/Concurrent/Posix/*.cc
    ${PROJECT_SOURCE_DIR}/../Chaos/IO/Posix/*.cc
    ${PROJECT_SOURCE_DIR}/../Chaos/Kern/Posix/*.cc)
  set(ALL_SOURCES ${ALL_SOURCES} ${POSIX_SOURCES})
  if (CMAKE_SYSTEM_NAME MATCHES "Linux")
    file(GLOB_RECURSE LINUX_SOURCES
      ${PROJECT_SOURCE_DIR}/../Chaos/Concurrent/Linux/*.cc
      ${PROJECT_SOURCE_DIR}/../Chaos/Kern/Linux/*.cc)
    set(ALL_SOURCES ${ALL_SOURCES} ${LINUX_SOURCES})
  elseif (CMAKE_SYSTEM_NAME MATCHES "Darwin")
    file(GLOB_RECURSE DARWIN_SOURCES
      ${PROJECT_SOURCE_DIR}/../Chaos/Concurrent/Darwin/*.cc
      ${PROJECT_SOURCE_DIR}/../Chaos/Kern/Darwin/*.cc)
    set(ALL_SOURCES ${ALL_SOURCES} ${DARWIN_SOURCES})
  endif()
endif()
add_executable(chaos.test ${ALL_SOURCES})

if (CMAKE_SYSTEM_NAME MATCHES "Windows")
  target_link_libraries(chaos.test winmm.lib Dbghelp.lib)
elseif (CMAKE_SYSTEM_NAME MATCHES "Linux")
  target_link_libraries(chaos.test pthread unwind)
elseif (CMAKE_SYSTEM_NAME MATCHES "Darwin")
  target_link_libraries(chaos.test c pthread)
endif()
