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
#ifndef CHAOS_CONCURRENT_THREADLOCALSINGLETON_H
#define CHAOS_CONCURRENT_THREADLOCALSINGLETON_H

#include <Chaos/Base/Platform.h>
#include <Chaos/Base/Types.h>
#include <Chaos/Kern/KernCommon.h>

namespace Chaos {

template <typename T>
class ThreadLocalSingleton : private UnCopyable {
  class Deleter {
  public:
    Chaos::_Tls_t tls_;

    Deleter(void) {
      Chaos::kern_tls_create(&tls_, &ThreadLocalSingleton::destructor);
    }

    ~Deleter(void) {
      Chaos::kern_tls_delete(tls_);
    }

    void set_value(T* value) {
      CHAOS_CHECK(nullptr == Chaos::kern_tls_getspecific(tls_),
          "ThreadLocalSingleton::Deleter::set_value "
          "- current tls should be nullptr");
      Chaos::kern_tls_setspecific(tls_, value);
    }
  };

  static __chaos_tl T* t_value_;
  static Deleter deleter_;

  ThreadLocalSingleton(void) = delete;
  ~ThreadLocalSingleton(void) = delete;

  static void destructor(void* obj) {
    if (nullptr == t_value_ && nullptr != obj) {
      // FIXME: fixed calling in another thread
      T* del_obj = static_cast<T*>(obj);
      delete del_obj;
      return;
    }

    CHAOS_CHECK(obj == t_value_,
        "ThreadLocalSingleton::destructor "
        "- destruction obj should be `t_value_`");
    if (nullptr != t_value_) {
      delete t_value_;
      t_value_ = nullptr;
    }
  }
public:
  static T& get_instance(void) {
    if (nullptr == t_value_) {
      t_value_ = new T();
      deleter_.set_value(t_value_);
    }
    return *t_value_;
  }

  static T* get_pointer(void) {
    return t_value_;
  }
};

template <typename T>
__chaos_tl T* ThreadLocalSingleton<T>::t_value_ = nullptr;

template <typename T>
typename ThreadLocalSingleton<T>::Deleter ThreadLocalSingleton<T>::deleter_;

}

#endif // CHAOS_CONCURRENT_THREADLOCALSINGLETON_H
