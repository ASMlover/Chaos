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

#include <Windows.h>
#include <Chaos/Base/Types.hh>
#include <iostream>

namespace Chaos {

class MutexBase : private UnCopyable {
  using MutexType = CRITICAL_SECTION;

  mutable MutexType m_;
  int tid_{-1};
  int count_{};
private:
  enum MutexResult {
    MUTEX_SUCCESS,
    MUTEX_TIMEDOUT,
    MUTEX_BUSY,
    MUTEX_ERROR,
  };

  inline int CHECK_RETURN(int ret, int desired = MUTEX_SUCCESS) {
    CHAOS_CHECK((ret == MUTEX_SUCCESS || ret == desired),
        "Mutex::CHECK_RETURN: mutex operation failed");
    return ret;
  }

  int do_lock(bool try_lock = false) {
    int tid = static_cast<int>(GetCurrentThreadId());
    int r = WAIT_TIMEOUT;
    if (!try_lock) {
      if (tid_ != tid)
        EnterCriticalSection(&m_);
      r = WAIT_OBJECT_0;
    }
    else {
      if (tid_ != tid)
        r = TRUE == TryEnterCriticalSection(&m_) ? WAIT_OBJECT_0 : WAIT_TIMEOUT;
      else
        r = WAIT_OBJECT_0;
    }

    if (r != WAIT_OBJECT_0 && r != WAIT_ABANDONED)
      ;
    else if (1 < ++count_)
      r = (--count_, WAIT_TIMEOUT);
    else
      tid_ = tid;

    switch (r) {
    case WAIT_OBJECT_0:
    case WAIT_ABANDONED:
      return MUTEX_SUCCESS;
    case WAIT_TIMEOUT:
      return MUTEX_BUSY;
    }
    return MUTEX_ERROR;
  }

  void clear_owner(void) {
    tid_ = -1;
    --count_;
  }

  void reset_owner(void) {
    tid_ = static_cast<int>(GetCurrentThreadId());
    ++count_;
  }

  class UnassignOwnerGuard : private UnCopyable {
    MutexBase& m_;
  public:
    explicit UnassignOwnerGuard(MutexBase& m)
      : m_(m) {
      m_.clear_owner();
    }

    ~UnassignOwnerGuard(void) {
      m_.reset_owner();
    }
  };
  friend class Condition;
public:
  MutexBase(void) {
    InitializeCriticalSection(&m_);
  }

  ~MutexBase(void) {
    DeleteCriticalSection(&m_);
  }

  void lock(void) {
    CHECK_RETURN(do_lock());
  }

  bool try_lock(void) {
    return CHECK_RETURN(do_lock(true), MUTEX_BUSY) == MUTEX_SUCCESS;
  }

  void unlock(void) {
    if (0 == --count_) {
      tid_ = -1;
      LeaveCriticalSection(&m_);
    }
  }

  MutexType* get_mutex(void) {
    return &m_;
  }
};

}
