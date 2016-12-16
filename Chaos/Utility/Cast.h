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
#ifndef CHAOS_UTILITY_CAST_H
#define CHAOS_UTILITY_CAST_H

#include <stddef.h>
#include <memory>
#include <type_traits>
#include <Chaos/Platform.h>

#if defined(CHAOS_ARCH64)
# define CHAOS_CAST_NOOFFSET (0x7fffffffffffffffLL)
#else
# define CHAOS_CAST_NOOFFSET (0x7fffffffLL)
#endif

namespace Chaos {

namespace Unexposed {
#if __cplusplus < 201402L
  template <typename T>
  using RemoveCV_t = typename std::remove_cv<T>::type;

  template <typename T>
  using RemoveRef_t = typename std::remove_reference<T>::type;

  template <typename T>
  using RemovePtr_t = typename std::remove_pointer<T>::type;

  template <bool B, typename T = void>
  using EnableIf_t = typename std::enable_if<B, T>::type;

  template <typename T>
  using AddPtr_t = typename std::add_pointer<T>::type;
#else
  using RemoveCV_t = std::remove_cv_t;
  using RemoveRef_t = std::remove_reference_t;
  using RemovePtr_t = std::remove_pointer_t;
  using EnableIf_t = std::enable_if_t;
#endif
}

using ObjectPtr = const uintptr_t*;
using VTablePtr = const uintptr_t*;

template <typename T>
inline VTablePtr get_vtable(const T* p) {
  return reinterpret_cast<VTablePtr>(*reinterpret_cast<ObjectPtr>(p));
}

template <typename T>
using CleanType = typename Unexposed::RemoveCV_t<Unexposed::RemoveRef_t<Unexposed::RemovePtr_t<T>>>;

template <typename Target, typename Source>
inline Target fast_dynamic_cast(Source* p) {
  if (nullptr == p)
    return nullptr;

  static __chaos_tl ptrdiff_t offset = CHAOS_CAST_NOOFFSET;
  static __chaos_tl VTablePtr src_vtable_ptr;

  VTablePtr this_vtable = get_vtable(p);
  if (offset != CHAOS_CAST_NOOFFSET && src_vtable_ptr == this_vtable) {
    char* new_ptr = reinterpret_cast<char*>(p) + offset;
    return reinterpret_cast<Target>(new_ptr);
  }
  else {
    auto result = dynamic_cast<Target>(p);
    if (nullptr == result)
      return nullptr;

    src_vtable_ptr = this_vtable;
    offset = reinterpret_cast<const char*>(result) - reinterpret_cast<const char*>(p);
    return result;
  }
}

template <typename Target, typename Source>
inline const Target fast_dynamic_cast(const Source* p) {
  Source* nonconst_ptr = const_cast<Source*>(p);
  Target casted_ptr = fast_dynamic_cast<Target>(nonconst_ptr);
  return const_cast<const Target>(casted_ptr);
}

template <typename Target, typename Source,
         typename = Unexposed::EnableIf_t<!std::is_same<CleanType<Target>, CleanType<Source&>>::value>>
inline Target fast_dynamic_cast(Source& ref) {
  using TargetPtr = Unexposed::AddPtr_t<Unexposed::RemoveRef_t<Target>>;
  auto casted_ptr = fast_dynamic_cast<TargetPtr>(&ref);
  if (nullptr == casted_ptr)
    throw std::bad_cast{};
  return *casted_ptr;
}

template <typename Target, typename Source,
         typename = Unexposed::EnableIf_t<!std::is_same<CleanType<Target>, CleanType<Source&>>::value>>
inline Target fast_dynamic_cast(const Source& ref) {
  using TargetPtr = Unexposed::AddPtr_t<Unexposed::RemoveRef_t<Source>>;
  auto casted_ptr = fast_dynamic_cast<TargetPtr>(const_cast<Source*>(&ref));
  if (nullptr == casted_ptr)
    throw std::bad_cast{};
  return *casted_ptr;
}

template <typename Target, typename Source>
inline std::shared_ptr<Target> fast_dynamic_ptr_cast(const std::shared_ptr<Source>& p) {
  return std::shared_ptr<Target>(p, fast_dynamic_cast<Target*>(p.get()));
}

template <typename T>
inline T fast_dynamic_cast(T p) {
  return p;
}

}

#endif // CHAOS_UTILITY_CAST_H
