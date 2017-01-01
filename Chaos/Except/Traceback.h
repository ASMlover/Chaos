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
#ifndef CHAOS_EXCEPT_TRACEBACK_H
#define CHAOS_EXCEPT_TRACEBACK_H

#include <Chaos/Platform.h>
#if !defined(CHAOS_LINUX)
# error "Only support for linux"
#endif

#define UNW_LOCAL_ONLY
#include <libunwind.h>

namespace Chaos {

namespace Traceback {
  // invokes `fn` for each frame passing return `IP` and `SP` as arguments
  template <typename Function>
  void traceback(Function&& fn) noexcept(noexcept(fn(0, 0))) {
    unw_context_t context;
    if (unw_getcontext(&context) < 0)
      return;

    unw_cursor_t cursor;
    if (unw_init_local(&cursor, &context) < 0)
      return;

    while (unw_step(&cursor) > 0) {
      unw_word_t ip;
      unw_word_t sp;
      unw_get_reg(&cursor, UNW_REG_IP, &ip);
      unw_get_reg(&cursor, UNW_REG_SP, &sp);
      fn(ip, sp);
    }
  }
}

}

#endif // CHAOS_EXCEPT_TRACEBACK_H
