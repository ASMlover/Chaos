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

file(GLOB_RECURSE CHAOS_TEST_HEADERS *.h)
file(GLOB_RECURSE CHAOS_TEST_SOURCES *.cc)
message(STATUS "`${PROJECT_NAME}` Tests CHAOS_TEST_HEADERS: ${CHAOS_TEST_HEADERS}")
message(STATUS "`${PROJECT_NAME}` Tests CHAOS_TEST_SOURCES: ${CHAOS_TEST_SOURCES}")

if (CMAKE_SYSTEM_NAME MATCHES "Linux")
  set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")
endif()
if (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
  set(CHAOS_TEST_ALL_FILES ${CHAOS_TEST_HEADERS} ${CHAOS_TEST_SOURCES})
  source_group_by_dir(CHAOS_TEST_ALL_FILES)
  add_executable(chaos.test ${CHAOS_TEST_ALL_FILES})
else()
  add_executable(chaos.test ${CHAOS_TEST_HEADERS} ${CHAOS_TEST_SOURCES})
endif()
target_link_libraries(chaos.test chaos)
