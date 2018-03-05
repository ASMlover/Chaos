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

set(CHAOSLIB_NAME "chaos")

file(GLOB CHAOS_SOURCES
  *.cc
  Codecs/*.cc
  Concurrent/*.cc
  Concurrent/Cpp11/*.cc
  Container/*.cc
  Datetime/*.cc
  Except/*.cc
  File/*.cc
  IO/*.cc
  Logging/*.cc
  Memory/*.cc
  Unittest/*.cc
  )
if (CMAKE_SYSTEM_NAME MATCHES "Windows")
  file(GLOB_RECURSE CHAOS_WINDOWS_SOURCES
    Concurrent/Windows/*.cc
    IO/Windows/*.cc
    Kern/Windows/*.cc
    )
  set(CHAOS_SOURCES ${CHAOS_SOURCES} ${CHAOS_WINDOWS_SOURCES})
else()
  file(GLOB_RECURSE CHAOS_POSIX_SOURCES
    Concurrent/Posix/*.cc
    IO/Posix/*.cc
    Kern/Posix/*.cc
    )
  set(CHAOS_SOURCES ${CHAOS_SOURCES} ${CHAOS_POSIX_SOURCES})
  if (CMAKE_SYSTEM_NAME MATCHES "Linux")
    file(GLOB_RECURSE CHAOS_LINUX_SOURCES Concurrent/Linux/*.cc)
    set(CHAOS_SOURCES ${CHAOS_SOURCES} ${CHAOS_LINUX_SOURCES})
  elseif (CMAKE_SYSTEM_NAME MATCHES "Darwin")
    file(GLOB_RECURSE CHAOS_DARWIN_SOURCES Concurrent/Darwin/*.cc)
    set(CHAOS_SOURCES ${CHAOS_SOURCES} ${CHAOS_DARWIN_SOURCES})
  endif()
endif()
message(STATUS "`${PROJECT_NAME}` CHAOS_SOURCES: ${CHAOS_SOURCES}")

add_library(chaos STATIC ${CHAOS_SOURCES})
if (CMAKE_SYSTEM_NAME MATCHES "Windows")
  target_link_libraries(${CHAOSLIB_NAME} winmm.lib Dbghelp.lib)
elseif (CMAKE_SYSTEM_NAME MATCHES "Linux")
  target_link_libraries(${CHAOSLIB_NAME} pthread rt unwind)
elseif (CMAKE_SYSTEM_NAME MATCHES "Darwin")
  target_link_libraries(${CHAOSLIB_NAME} pthread c)
endif()
