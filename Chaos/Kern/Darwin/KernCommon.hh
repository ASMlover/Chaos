// Copyright (c) 2016 ASMlover. All rights reserved.
//
//  ____     __
// /\  _`\  /\ \
// \ \ \/\_\\ \ \___      __      ___     ____
//  \ \ \/_/_\ \  _ `\  /'__`\   / __`\  /',__\
//   \ \ \L\ \\ \ \ \ \/\ \L\.\_/\ \L\ \/\__, `\
//    \ \____/ \ \_\ \_\ \__/.\_\ \____/\/\____/
//     \/___/   \/_/\/_/\/__/\/_/\/___/  \/___/
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
//
//  * Redistributions of source code must retain the above copyright
//    notice, this list ofconditions and the following disclaimer.
//
//  * Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in
//    the documentation and/or other materialsprovided with the
//    distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
// FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
// COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
// INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
// BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
// LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
// ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
#pragma once

#include <mach/mach_time.h>
#include <sys/syscall.h>
#include <sys/time.h>
#include <pthread.h>
#include <unistd.h>
#include <cstdio>
#include <ctime>
#include <Chaos/Base/Types.hh>

namespace Chaos {

inline int kern_strerror(int errnum, char* buf, std::size_t buflen) {
  return strerror_r(errnum, buf, buflen);
}

inline int kern_gettimeofday(struct timeval* tv, struct timezone* tz) {
  return gettimeofday(tv, tz);
}

inline pid_t kern_gettid(void) {
  return static_cast<pid_t>(syscall(SYS_thread_selfid));
}

inline int kern_this_thread_setname(const char* name) {
  return pthread_setname_np(name);
}

inline int kern_gettime(struct timespec* timep) {
  mach_timebase_info_data_t info;
  if (mach_timebase_info(&info) != KERN_SUCCESS)
    abort();
  std::uint64_t realtime = mach_absolute_time() * info.numer / info.denom;
  timep->tv_sec = realtime / CHAOS_NANOSEC;
  timep->tv_nsec = realtime % CHAOS_NANOSEC;
  return 0;
}

namespace io {
  // Stream-IO methods wrapper
  inline std::size_t kern_fwrite_unlocked(
      const void* buf, std::size_t size, std::size_t count, std::FILE* stream) {
    return std::fwrite(buf, size, count, stream);
  }
}

namespace timer {
  // timerfd methods wrapper
  inline int kern_open(void) {
    return 0;
  }

  inline void kern_close(int /*timerfd*/) {
  }

  inline int kern_gettime(int /*timerfd*/, std::size_t /*len*/, void* /*buf*/) {
    return 0;
  }

  inline int kern_settime(int /*timerfd*/, std::int64_t /*msec*/) {
    return 0;
  }
}

}
