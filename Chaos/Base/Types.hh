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

#include <cstdint>
#include <memory>
#include <Chaos/Base/Copyable.hh>
#include <Chaos/Base/UnCopyable.hh>

#if !defined(CHAOS_UNUSED)
# define CHAOS_UNUSED(x) ((void)x)
#endif

#if !defined(CHAOS_NANOSEC)
# define CHAOS_NANOSEC ((uint64_t)1e9)
#endif

#if !defined(CHAOS_CHECK)
# include <cstdlib>
# include <Chaos/IO/ColorIO.h>
# define CHAOS_CHECK(condition, message) do {\
    if (!(condition)) {\
      Chaos::ColorIO::fprintf(stderr,\
          Chaos::ColorIO::ColorType::COLORTYPE_FG_RED,\
          "[%s:%d] CHECKING FAILED `%s()` - %s\n",\
          __FILE__,\
          __LINE__,\
          __func__,\
          (message));\
      std::fflush(stderr);\
      std::abort();\
    }\
  } while (0)
#endif

#if !defined(CHAOS_COUNTOF)
# define CHAOS_COUNTOF(array) sizeof(Chaos::__countof_impl(array).elements)
#endif

using byte_t = std::uint8_t;

namespace Chaos {

template <typename T>
inline const T& chaos_min(const T& a, const T& b) noexcept {
  return a < b ? a : b;
}

template <typename T>
inline const T& chaos_max(const T& a, const T& b) noexcept {
  return a > b ? a : b;
}

template <typename T>
inline T* get_pointer(const std::unique_ptr<T>& p) noexcept {
  return p.get();
}

template <typename T>
inline T* get_pointer(const std::shared_ptr<T>& p) noexcept {
  return p.get();
}

template <typename T> struct Identity {
  using Type = T;
};

template <typename T>
inline T implicit_cast(typename Identity<T>::Type x) noexcept {
  return x;
}

template <typename Target, typename Source>
inline Target down_cast(Source& x) noexcept {
  return static_cast<Target>(x);
}

template <typename Target, typename Source>
inline Target down_cast(Source* x) noexcept {
  return static_cast<Target>(x);
}

template <typename Target, typename Source>
inline std::shared_ptr<Target>
down_cast_ptr(const std::shared_ptr<Source>& p) noexcept {
  return std::static_pointer_cast<Target>(p);
}

template <std::ptrdiff_t N> struct Sizer { char elements[N]; };
template <typename T, std::ptrdiff_t N> Sizer<N> __countof_impl(T (&array)[N]);

}
