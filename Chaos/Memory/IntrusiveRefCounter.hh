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
#include <atomic>
#include <Chaos/Base/Copyable.hh>

namespace Chaos {

struct CountedThreadUnsafe {
  using type = std::uint32_t;

  static std::uint32_t load(std::uint32_t counter) noexcept {
    return counter;
  }

  static void increment(std::uint32_t& counter) noexcept {
    ++counter;
  }

  static std::uint32_t decrement(std::uint32_t& counter) noexcept {
    return --counter;
  }
};

struct CountedThreadSafe {
  using type = std::atomic<std::uint32_t>;

  static std::uint32_t load(
      const std::atomic<std::uint32_t>& counter) noexcept {
    return static_cast<std::uint32_t>(counter);
  }

  static void increment(std::atomic<std::uint32_t>& counter) noexcept {
    ++counter;
  }

  static std::uint32_t decrement(
      std::atomic<std::uint32_t>& counter) noexcept {
    return static_cast<std::uint32_t>(--counter);
  }
};

template <typename DerivedT, typename CounterPolicyT>
class IntrusiveRefCounter;

template <typename DerivedT, typename CounterPolicyT>
void intrusive_ptr_add_ref(
    const IntrusiveRefCounter<DerivedT, CounterPolicyT>* p);
template <typename DerivedT, typename CounterPolicyT>
void intrusive_ptr_del_ref(
    const IntrusiveRefCounter<DerivedT, CounterPolicyT>* p);

template <typename DerivedT, typename CounterPolicyT>
class IntrusiveRefCounter : public Copyable {
  using counter_type = typename CounterPolicyT::type;

  mutable counter_type ref_counter_{};
public:
  IntrusiveRefCounter& operator=(const IntrusiveRefCounter&) noexcept {
    return *this;
  }

  std::uint32_t use_count(void) const noexcept {
    return CounterPolicyT::load(ref_counter_);
  }
protected:
  template <typename _Derived, typename _CounterPolicy>
  friend void intrusive_ptr_add_ref(
      const IntrusiveRefCounter<_Derived, _CounterPolicy>* p);
  template <typename _Derived, typename _CounterPolicy>
  friend void intrusive_ptr_del_ref(
      const IntrusiveRefCounter<_Derived, _CounterPolicy>* p);
};

template <typename DerivedT, typename CounterPolicyT>
inline void intrusive_ptr_add_ref(
    const IntrusiveRefCounter<DerivedT, CounterPolicyT>* p) {
  CounterPolicyT::increment(p->ref_counter_);
}

template <typename DerivedT, typename CounterPolicyT>
inline void intrusive_ptr_del_ref(
    const IntrusiveRefCounter<DerivedT, CounterPolicyT>* p) {
  if (CounterPolicyT::decrement(p->ref_counter_) == 0)
    delete static_cast<const DerivedT*>(p);
}

}
