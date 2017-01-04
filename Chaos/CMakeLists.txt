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
set(CHAOS_SOURCES

  Codecs/Base16.cc
  Codecs/Base32.cc
  Codecs/Base64.cc
  Codecs/Crc32c.cc
  Concurrent/CountdownLatch.cc
  Concurrent/Thread.cc
  Concurrent/ThreadPool.cc
  Container/StringPiece.cc
  Datetime/Date.cc
  Datetime/Timestamp.cc
  Datetime/Timezone.cc
  Except/Exception.cc
  File/FileUtil.cc
  IO/ColorIO.cc
  Logging/Logging.cc
  Logging/LogStream.cc
  Unittest/TestHarness.cc
  )

if (CMAKE_SYSTEM_NAME MATCHES "Windows")
  set(CHAOS_SOURCES ${CHAOS_SOURCES}

    Concurrent/Windows/CurrentThread.cc
    IO/Windows/ColorIO.cc
    Kern/Windows/KernCommon.cc
    )
else()
  set(CHAOS_SOURCES ${CHAOS_SOURCES}

    Concurrent/Posix/CurrentThread.cc
    IO/Posix/ColorIO.cc
    Kern/Posix/KernCommon.cc
    )
  if (CMAKE_SYSTEM_NAME MATCHES "Linux")
    set(CHAOS_SOURCES ${CHAOS_SOURCES} Concurrent/Linux/CurrentThread.cc)
  elseif (CMAKE_SYSTEM_NAME MATCHES "Darwin")
    set(CHAOS_SOURCES ${CHAOS_SOURCES} Concurrent/Darwin/CurrentThread.cc)
  endif()
endif()

add_library(chaos STATIC ${CHAOS_SOURCES})
if (CMAKE_SYSTEM_NAME MATCHES "Windows")
  target_link_libraries(${CHAOSLIB_NAME} winmm.lib Dbghelp.lib)
elseif (CMAKE_SYSTEM_NAME MATCHES "Linux")
  target_link_libraries(${CHAOSLIB_NAME} pthread rt unwind)
elseif (CMAKE_SYSTEM_NAME MATCHES "Darwin")
  target_link_libraries(${CHAOSLIB_NAME} pthread c)
endif()