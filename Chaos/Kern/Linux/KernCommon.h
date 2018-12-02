// Copyright (c) 2016 ASMlover. All rights reserved.
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

#include <sys/prctl.h>
#include <sys/syscall.h>
#include <sys/time.h>
#include <sys/timerfd.h>
#include <unistd.h>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <ctime>

namespace Chaos {

inline char* kern_strerror(int errnum, char* buf, std::size_t buflen) {
  return strerror_r(errnum, buf, buflen);
}

inline int kern_gettimeofday(struct timeval* tv, struct timezone* tz) {
  return gettimeofday(tv, tz);
}

inline pid_t kern_gettid(void) {
  return static_cast<pid_t>(syscall(SYS_gettid));
}

inline int kern_this_thread_setname(const char* name) {
  return prctl(PR_SET_NAME, name);
}

inline int kern_gettime(struct timespec* timep) {
  return clock_gettime(CLOCK_REALTIME, timep);
}

namespace io {
  // Stream-IO methods wrapper
  inline std::size_t kern_fwrite_unlocked(
      const void* buf, std::size_t size, std::size_t count, std::FILE* stream) {
    return fwrite_unlocked(buf, size, count, stream);
  }
}

namespace timer {
  // timerfd methods wrapper
  inline int kern_open(void) {
    return timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK | TFD_CLOEXEC);
  }

  inline void kern_close(int timerfd) {
    close(timerfd);
  }

  inline int kern_gettime(int timerfd, std::size_t len, void* buf) {
    return static_cast<int>(read(timerfd, buf, len));
  }

  inline int kern_settime(int timerfd, std::int64_t msec) {
    struct itimerspec oldt{};
    struct itimerspec newt{};

    struct timespec ts;
    ts.tv_sec = static_cast<std::time_t>(msec / 1000000);
    ts.tv_nsec = static_cast<long>((msec % 1000000) * 1000);
    newt.it_value = ts;
    return timerfd_settime(timerfd, 0, &newt, &oldt);
  }
}

}
