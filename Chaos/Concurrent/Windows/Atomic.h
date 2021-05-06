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

#include <Windows.h>
#include <Chaos/Base/UnCopyable.hh>
#include <iostream>

namespace Chaos {

namespace Unexposed {
  // Atomic unexposed implementation on Windows
  template <int Bytes> struct ActomicImpl {
    LONG get_impl(const void* value) const volatile {
      return InterlockedCompareExchange((volatile LONG*)value, 0, 0);
    }

    LONG set_impl(const void* value, LONG arg) const volatile {
      return InterlockedExchange((volatile LONG*)value, arg);
    }

    LONG fetch_add_impl(const void* value, LONG arg) const volatile {
      return InterlockedExchangeAdd((volatile LONG*)value, arg);
    }

    LONG fetch_sub_impl(const void* value, LONG arg) const volatile {
      return InterlockedExchangeAdd((volatile LONG*)value, -arg);
    }

    LONG increment_impl(const void* value) const volatile {
      return InterlockedIncrement((volatile LONG*)value);
    }

    LONG decrement_impl(const void* value) const volatile {
      return InterlockedDecrement((volatile LONG*)value);
    }
  };

  template <> struct ActomicImpl<1> {
    char get_impl(const void* value) const volatile {
      return static_cast<char>(
          InterlockedCompareExchange((volatile LONG*)value, 0, 0));
    }

    char set_impl(const void* value, char arg) const volatile {
      return InterlockedExchange8((volatile char*)value, arg);
    }

    char fetch_add_impl(const void* value, char arg) const volatile {
      return static_cast<char>(
          InterlockedExchangeAdd((volatile LONG*)value, arg));
    }

    char fetch_sub_impl(const void* value, char arg) const volatile {
      return static_cast<char>(
          InterlockedExchangeAdd((volatile LONG*)value, -arg));
    }

    char increment_impl(const void* value) const volatile {
      return static_cast<char>(InterlockedIncrement((volatile LONG*)value));
    }

    char decrement_impl(const void* value) const volatile {
      return static_cast<char>(InterlockedDecrement((volatile LONG*)value));
    }
  };

  template <> struct ActomicImpl<2> {
    short get_impl(const void* value) const volatile {
      return InterlockedCompareExchange16((volatile short*)value, 0, 0);
    }

    short set_impl(const void* value, short arg) const volatile {
      return InterlockedExchange16((volatile short*)value, arg);
    }

    short fetch_add_impl(const void* value, short arg) const volatile {
      return static_cast<short>(
          InterlockedExchangeAdd((volatile LONG*)value, arg));
    }

    short fetch_sub_impl(const void* value, short arg) const volatile {
      return static_cast<short>(
          InterlockedExchangeAdd((volatile LONG*)value, -arg));
    }

    short increment_impl(const void* value) const volatile {
      return InterlockedIncrement16((volatile short*)value);
    }

    short decrement_impl(const void* value) const volatile {
      return static_cast<short>(InterlockedDecrement((volatile LONG*)value));
    }
  };

  template <> struct ActomicImpl<8> {
    LONGLONG get_impl(const void* value) const volatile {
      return InterlockedCompareExchange64((volatile LONGLONG*)value, 0, 0);
    }

    LONGLONG set_impl(const void* value, LONGLONG arg) const volatile {
      return InterlockedExchange64((volatile LONGLONG*)value, arg);
    }

    LONGLONG fetch_add_impl(const void* value, LONGLONG arg) const volatile {
      return InterlockedExchangeAdd64((volatile LONGLONG*)value, arg);
    }

    LONGLONG fetch_sub_impl(const void* value, LONGLONG arg) const volatile {
      return InterlockedExchangeAdd64((volatile LONGLONG*)value, -arg);
    }

    LONGLONG increment_impl(const void* value) const volatile {
      return InterlockedIncrement64((volatile LONGLONG*)value);
    }

    LONGLONG decrement_impl(const void* value) const volatile {
      return InterlockedDecrement64((volatile LONGLONG*)value);
    }
  };
}

template <typename T>
class Atomic final
  : public Unexposed::ActomicImpl<sizeof(T)> , private UnCopyable {
  using AtomicBase = Unexposed::ActomicImpl<sizeof(T)>;

  T value_{};
  static_assert(
      sizeof(T) == 1 || sizeof(T) == 2 || sizeof(T) == 4 || sizeof(T) == 8,
      "Chaos::Atomic value type's size must be `1`, `2`, `4` or `8`");
public:
  T get(void) {
    return static_cast<T>(AtomicBase::get_impl(&value_));
  }

  T set(T desired) {
    return static_cast<T>(AtomicBase::set_impl(&value_, desired));
  }

  T fetch_add(T arg) {
    return static_cast<T>(AtomicBase::fetch_add_impl(&value_, arg));
  }

  T fetch_sub(T arg) {
    return static_cast<T>(AtomicBase::fetch_sub_impl(&value_, arg));
  }

  T operator++(void) {
    return static_cast<T>(AtomicBase::increment_impl(&value_));
  }

  T operator--(void) {
    return static_cast<T>(AtomicBase::decrement_impl(&value_));
  }

  T operator++(int) {
    return fetch_add(1);
  }

  T operator--(int) {
    return fetch_sub(1);
  }

  T operator+=(T arg) {
    return fetch_add(arg) + arg;
  }

  T operator-=(T arg) {
    return fetch_sub(arg) - arg;
  }
};

}
