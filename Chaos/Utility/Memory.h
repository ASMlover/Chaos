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
#ifndef CHAOS_UTILITY_MEMORY_H
#define CHAOS_UTILITY_MEMORY_H

#include <memory>
#include <Chaos/Memory/Memory.h>

namespace Chaos {

template <typename T, typename... Args>
inline std::unique_ptr<T, std::default_delete<T>> make_unique(Args&&... args) {
  return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

template <typename T, typename Y = void>
inline std::unique_ptr<T, std::default_delete<T>> as_unique(Y* p) noexcept {
  return std::unique_ptr<T>(static_cast<T*>(p));
}

template <typename T>
inline T* move_ptr(T*& p) noexcept {
  T* moved = p;
  p = nullptr;
  return moved;
}

template <typename T, typename... Args>
inline ScopedPtr<T> make_scoped(Args&&... args) {
  return ScopedPtr<T>(new T(std::forward<Args>(args)...));
}

template <typename T, typename Y = void>
inline ScopedPtr<T> as_scoped(Y* p) noexcept {
  return ScopedPtr<T>(static_cast<T*>(p));
}

template <typename T>
struct is_smart_ptr : std::false_type {};

template <typename T>
struct is_smart_ptr<std::unique_ptr<T>> : std::true_type {};

template <typename T>
struct is_smart_ptr<std::shared_ptr<T>> : std::true_type {};

template <typename T>
struct is_smart_ptr<ScopedPtr<T>> : std::true_type {};

template <typename T>
struct is_smart_ptr<SharedPtr<T>> : std::true_type {};

}

#endif // CHAOS_UTILITY_MEMORY_H
