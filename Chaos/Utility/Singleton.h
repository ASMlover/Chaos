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
#ifndef CHAOS_UTILITY_SINGLETON_H
#define CHAOS_UTILITY_SINGLETON_H

#include <cstdlib>
#include <Chaos/Platform.h>
#if defined(CHAOS_IMPL_WITH_STD)
# include <mutex>
#endif
#include <Chaos/Types.h>
#include <Chaos/Kern/KernCommon.h>

namespace Chaos {

namespace Unexposed {
  template <typename T>
  struct has_no_destroy {
    template <typename C> static char check(decltype(&C::no_destroy));
    template <typename C> static std::int32_t check(...);
    static const bool value = sizeof(check<T>(0)) == 1;
  };
}

template <typename Object>
class Singleton : private UnCopyable {
  static Chaos::_Once_t once_;
  static Object* value_;
public:
  static Object& get_instance(void) {
    Chaos::kern_once(&once_, &Singleton::init_routine);
    CHAOS_CHECK(nullptr != value_, "Singleton(default) init must be success");

    return *value_;
  }

#if defined(CHAOS_IMPL_WITH_STD)
  template <typename... Args>
  static Object& get_instance(Args&&... args) {
    static std::once_flag s_once;
    std::call_once(s_once, [](Args&&... args) {
          value_ = new Object(std::forward<Args>(args)...);
          if (!Unexposed::has_no_destroy<Object>::value)
            atexit(destroy_routine);
        }, std::forward<Args>(args)...);
    CHAOS_CHECK(nullptr != value_, "Singleton(Args...) init must be success");

    return *value_;
  }
#endif
private:
  Singleton(void) = delete;
  ~Singleton(void) = delete;

  static void init_routine(void) {
    value_ = new Object();
    if (!Unexposed::has_no_destroy<Object>::value)
      atexit(destroy_routine);
  }

  static void destroy_routine(void) {
    static_assert(sizeof(Object) != 0, "size of `Object` must not be zero");

    delete value_;
    value_ = nullptr;
  }
};

template <typename Object>
Chaos::_Once_t Singleton<Object>::once_ = Chaos::kInitOnceValue;

template <typename Object>
Object* Singleton<Object>::value_ = nullptr;

}

#endif // CHAOS_UTILITY_SINGLETON_H
