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
#include <sys/timeb.h>
#include <stdio.h>
#include <time.h>
#include <Chaos/Types.h>
#include <Chaos/OS/OS.h>
#include <Chaos/Datetime/Time.h>

namespace Chaos {

namespace Time {
  std::string Time::to_string(void) const {
    char buf[32];
    snprintf(buf,
        sizeof(buf),
        "%04d%02d%02d %02d:%02d:%02d.%06d",
        year,
        month,
        day,
        hour,
        minute,
        second,
        millitm);

    return buf;
  }

  Time get_localtime(void) {
    struct timeb tb;
    ftime(&tb);
    struct tm* now = localtime(&tb.time);

    Time local;
    local.year = static_cast<int16_t>(now->tm_year + 1900);
    local.month = static_cast<int8_t>(now->tm_mon + 1);
    local.day = static_cast<int8_t>(now->tm_mday);
    local.hour = static_cast<int8_t>(now->tm_hour);
    local.minute = static_cast<int8_t>(now->tm_min);
    local.second = static_cast<int8_t>(now->tm_sec);
    local.millitm = static_cast<int16_t>(tb.millitm);

    return local;
  }

  int64_t get_clock(void) {
    struct timeval tv;
    if (0 == Chaos::kern_gettimeofday(&tv, nullptr))
      return (((tv.tv_sec - CHAOS_NANOSEC) * 1000) + (tv.tv_usec / 1000));
    return 0;
  }
}

}
