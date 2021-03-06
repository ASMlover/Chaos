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

#include <type_traits>

namespace Chaos {

// add reference `traits`
template <typename T>
struct AddRefImpl {
  using type = T&;
};

template <typename T>
struct AddRefImpl<T&&> {
  using type = T&&;
};

template <typename T>
struct AddRef {
  using type = typename AddRefImpl<T>::type;
};

template <typename T>
struct AddRef<T&> {
  using type = T&;
};

// disable_if `traits`
template <bool B, typename T = void>
struct DisableIf {
  using type = T;
};

template <typename T>
struct DisableIf<true, T> {};

template <typename T>
using AddRef_t = typename AddRef<T>::type;

template <bool B, typename T = void>
using DisableIf_t = typename DisableIf<B, T>::type;

#if __cplusplus < 201402L
  template <typename T>
  using AddPtr_t = typename std::add_pointer<T>::type;

  template <typename T>
  using RemovePtr_t = typename std::remove_pointer<T>::type;

  template <typename T>
  using RemoveRef_t = typename std::remove_reference<T>::type;

  template <bool B, typename T = void>
  using EnableIf_t = typename std::enable_if<B, T>::type;

  template <typename T>
  using RemoveCV_t = typename std::remove_cv<T>::type;
#else
  template <typename T>
  using AddPtr_t = std::add_pointer_t<T>;

  template <typename T>
  using RemovePtr_t = std::remove_pointer_t<T>;

  template <typename T>
  using RemoveRef_t = std::remove_reference_t<T>;

  template <bool B, typename T = void>
  using EnableIf_t = std::enable_if_t<B, T>;

  template <typename T>
  using RemoveCV_t = std::remove_cv_t<T>;
#endif

}
