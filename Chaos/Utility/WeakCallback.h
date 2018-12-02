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

#include <functional>
#include <memory>

namespace Chaos {

template <typename Class, typename... Args>
class WeakCallback {
  std::weak_ptr<Class> wk_obj_;
  std::function<void (Class*, Args...)> fn_;
public:
  WeakCallback(
      const std::weak_ptr<Class>& obj,
      const std::function<void (Class*, Args...)>& fn)
    : wk_obj_(obj)
    , fn_(fn) {
  }

  void operator()(Args&&... args) const {
    std::shared_ptr<Class> obj(wk_obj_.lock());
    if (obj)
      fn_(obj.get(), std::forward<Args>(args)...);
  }
};

template <typename Class, typename... Args>
inline WeakCallback<Class, Args...> make_weak_callback(
    const std::shared_ptr<Class>& obj, void (Class::*fn)(Args...)) {
  return WeakCallback<Class, Args...>(obj, fn);
}

template <typename Class, typename... Args>
inline WeakCallback<Class, Args...> make_weak_callback(
    const std::shared_ptr<Class>& obj, void (Class::*fn)(Args...) const) {
  return WeakCallback<Class, Args...>(obj, fn);
}

}
