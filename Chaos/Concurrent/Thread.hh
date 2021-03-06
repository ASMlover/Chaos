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

#include <atomic>
#include <functional>
#include <memory>
#include <string>
#include <Chaos/Base/Types.hh>
#include <Chaos/Kern/KernCommon.hh>

namespace Chaos {

class Thread final : private UnCopyable {
  using ThreadCallback = std::function<void (void)>;

  bool started_{};
  bool joined_{};
  Chaos::_Thread_t thread_{};
  std::shared_ptr<pid_t> tid_;
  ThreadCallback routine_;
  std::string name_;

  static std::atomic<std::int32_t> num_created_;
private:
  void set_default_name(void);
public:
  explicit Thread(
      const ThreadCallback& fn, const std::string& name = std::string());
  explicit Thread(ThreadCallback&& fn, const std::string& name = std::string());

  ~Thread(void) {
    if (started_ && !joined_)
      Chaos::kern_thread_detach(thread_);
  }

  void start(void);
  bool join(void);

  bool is_started(void) const {
    return started_;
  }

  bool is_joined(void) const {
    return joined_;
  }

  pid_t get_tid(void) const {
    return *tid_;
  }

  const std::string& get_name(void) const {
    return name_;
  }

  static std::int32_t get_num_created(void) {
    return num_created_.load();
  }
};

}
