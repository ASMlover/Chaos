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

template <typename T>
inline std::unique_ptr<T, std::default_delete<T>> to_unique(T* p) noexcept {
  return std::unique_ptr<T>(p);
}

template <typename T>
inline T* move_ptr(T*& p) noexcept {
  T* moved = p;
  p = nullptr;
  return moved;
}

template <typename T, typename... Args>
inline ScopedPtr<T> make_scoped_ptr(Args&&... args) {
  return ScopedPtr<T>(new T(std::forward<Args>(args)...));
}

template <typename T>
inline ScopedPtr<T> to_scoped_ptr(T* p) noexcept {
  return ScopedPtr<T>(p);
}

}

#endif // CHAOS_UTILITY_MEMORY_H