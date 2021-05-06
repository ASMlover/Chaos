// Copyright (c) 2017 ASMlover. All rights reserved.
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

#include <utility>
#include <Chaos/Base/UnCopyable.hh>

namespace Chaos {

template <typename Object> class ObjectPool;

// Object need __prev() and __next() access methods implementation

class ObjectPoolAccess {
public:
  // default BaseObject type for ObjectPool
  template <typename DrivedObject> struct BaseObject {
    DrivedObject* __prevobj{};
    DrivedObject* __nextobj{};

    DrivedObject*& __prev(void) {
      return __prevobj;
    }

    DrivedObject*& __next(void) {
      return __nextobj;
    }
  };
public:
  template <typename Object>
  static Object* create(void) {
    return new Object();
  }

  template <typename Object, typename... Args>
  static Object* create(Args&&... args) {
    return new Object(std::forward<Args>(args)...);
  }

  template <typename Object>
  static void destroy(Object* o) {
    delete o;
  }

  template <typename Object>
  static Object*& prev(Object* o) {
    return o->__prev();
  }

  template <typename Object>
  static Object*& next(Object* o) {
    return o->__next();
  }
};

template <typename Object>
class ObjectPool : private UnCopyable {
  Object* live_list_{};
  Object* free_list_{};

  void destroy_list(Object* list) {
    while (list != nullptr) {
      auto* o = list;
      list = ObjectPoolAccess::next(o);
      ObjectPoolAccess::destroy(o);
    }
  }
public:
  ObjectPool(void) {
  }

  ~ObjectPool(void) {
    destroy_list(live_list_);
    destroy_list(free_list_);
  }

  Object* first(void) const {
    // get the object at the start of the live list
    return live_list_;
  }

  Object* alloc(void) {
    auto* o = free_list_;
    if (o != nullptr)
      free_list_ = ObjectPoolAccess::next(free_list_);
    else
      o = ObjectPoolAccess::create<Object>();

    ObjectPoolAccess::next(o) = live_list_;
    ObjectPoolAccess::prev(o) = nullptr;
    if (live_list_ != nullptr)
      ObjectPoolAccess::prev(live_list_) = nullptr;
    live_list_ = o;

    return o;
  }

  template <typename... Args>
  Object* alloc(Args&&... args) {
    auto* o = free_list_;
    if (o != nullptr)
      free_list_ = ObjectPoolAccess::next(free_list_);
    else
      o = ObjectPoolAccess::create<Object>(std::forward<Args>(args)...);

    ObjectPoolAccess::next(o) = live_list_;
    ObjectPoolAccess::prev(o) = nullptr;
    if (live_list_ != nullptr)
      ObjectPoolAccess::prev(live_list_) = nullptr;
    live_list_ = o;

    return o;
  }

  void dealloc(Object* o) {
    if (live_list_ == o)
      live_list_ = ObjectPoolAccess::next(o);

    if (ObjectPoolAccess::prev(o) != nullptr) {
      ObjectPoolAccess::next(ObjectPoolAccess::prev(o))
        = ObjectPoolAccess::next(o);
    }
    if (ObjectPoolAccess::next(o) != nullptr) {
      ObjectPoolAccess::prev(ObjectPoolAccess::next(o))
        = ObjectPoolAccess::prev(o);
    }

    ObjectPoolAccess::next(o) = free_list_;
    ObjectPoolAccess::prev(o) = nullptr;
    free_list_ = o;
  }
};

}
