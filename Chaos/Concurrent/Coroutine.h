// Copyright (c) 2017 ASMlover. All rights reserved.
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
#ifndef CHAOS_CONCURRENT_COROUTINE_H
#define CHAOS_CONCURRENT_COROUTINE_H

namespace Chaos { namespace Stackless {

class Coroutine {
  int value_{};

  friend class CoroutineRef;
public:
  Coroutine(void) {
  }

  bool is_child(void) const {
    return value_ < 0;
  }

  bool is_parent(void) const {
    return !is_child();
  }

  bool is_complete(void) const {
    return value_ == -1;
  }
};

class CoroutineRef {
  int& value_;
  bool modified_{};
public:
  CoroutineRef(Coroutine& c)
    : value_(c.value_) {
  }

  CoroutineRef(Coroutine* c)
    : value_(c->value_) {
  }

  ~CoroutineRef(void) {
    if (!modified_)
      value_ = -1;
  }

  operator int(void) const {
    return value_;
  }

  int& operator=(int value) {
    modified_ = true;
    return value_ = value;
  }
};

}}

#define CHAOS_CO_REENTER(c)\
  switch (Chaos::Stackless::CoroutineRef _co_value = c)\
    case -1: if (_co_value) {\
      goto _TerminateCoroutine;\
    _TerminateCoroutine:\
      _co_value = -1;\
      goto _BailOutOfCoroutine;\
    _BailOutOfCoroutine:\
      break;\
    }\
    else case 0:

#define CHAOS_CO_YIELD(n)\
  for (_co_value = (n);;)\
    if (_co_value == 0) {\
      case (n):;\
      break;\
    }\
    else\
      switch (_co_value ? 0 : 1)\
        for (;;)\
          case -1: if (_co_value)\
            goto _TerminateCoroutine;\
          case for (;;)\
            case 1: if (_co_value)\
              goto _BailOutOfCoroutine;\
            else case 0:

#include <Chaos/Base/Platform.h>

#if !defined(chaos_reenter)
# define chaos_reenter(c) CHAOS_CO_REENTER(c)
#endif

#if !defined(chaos_yield)
# if defined(CHAOS_WINDOWS)
#   define chaos_yield CHAOS_CO_YIELD(__COUNTER__ + 1)
# else
#   define chaos_yield CHAOS_CO_YIELD(__LINE__)
# endif
#endif

#endif // CHAOS_CONCURRENT_COROUTINE_H
