// Copyright (c) 2017 ASMlover. All rights reserved.
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
#include <iostream>
#include <string>
#include <thread>
#include <Chaos/Base/UnCopyable.hh>
#include <Chaos/Concurrent/CurrentThread.hh>
#include <Chaos/Unittest/TestHarness.hh>
#include <Chaos/Utility/Singleton.hh>

namespace cc = ::Chaos::CurrentThread;

class _Useless : private Chaos::UnCopyable {
  std::string message_;
public:
  _Useless(void) {
    std::cout
      << "Chaos::Singleton unittest  - _Useless::_Useless @tid="
      << cc::get_tid() << std::endl;
  }

  ~_Useless(void) {
    std::cout
      << "Chaos::Singleton unittest - _Useless::~_Useless @tid="
      << cc::get_tid() << ", @message=" << message_ << std::endl;
  }

  void set_message(const std::string& message) {
    message_ = message;
  }

  const std::string& get_message(void) const {
    return message_;
  }
};

CHAOS_TEST(Singleton, Chaos::FakeTester) {
  {
    Chaos::Singleton<_Useless>::get_instance().set_message("_Useless.Main");
    std::thread t([] {
        std::cout
          << "Chaos::Singleton unittest - @tid=" << cc::get_tid()
          << ", @message="
          << Chaos::Singleton<_Useless>::get_instance().get_message()
          << std::endl;
        Chaos::Singleton<_Useless>::get_instance().set_message("_Useless.Changed");
        });
    t.join();
    std::cout
      << "Chaos::Singleton unittest - @tid=" << cc::get_tid() << ", @message="
      << Chaos::Singleton<_Useless>::get_instance().get_message() << std::endl;
  }
}
