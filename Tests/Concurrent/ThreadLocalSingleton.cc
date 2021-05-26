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
#include <Chaos/Base/UnCopyable.hh>
#include <Chaos/Concurrent/CurrentThread.hh>
#include <Chaos/Concurrent/Thread.hh>
#include <Chaos/Concurrent/ThreadLocalSingleton.hh>
#include <Chaos/Unittest/TestHarness.h>

namespace cc = ::Chaos::CurrentThread;

class Useless : private Chaos::UnCopyable {
  std::string message_;
public:
  Useless(void) {
    std::cout
      << "Chaos::ThreadLocalSingleton unittest - Useless::Useless @tid="
      << cc::get_tid() << std::endl;
  }

  ~Useless(void) {
    std::cout
      << "Chaos::ThreadLocalSingleton unittest - Useless::~Useless @tid="
      << cc::get_tid() << std::endl;
  }

  void set_message(const std::string& msg) {
    message_ = msg;
  }

  const std::string& get_message(void) const {
    return message_;
  }
};

CHAOS_TEST(ThreadLocalSingleton, Chaos::FakeTester) {
  {
#define USELESS_INS Chaos::ThreadLocalSingleton<Useless>::get_instance()
    Chaos::ThreadLocalSingleton<Useless>::get_instance().set_message("Main#1");

    Chaos::Thread t([] {
          std::cout
            << "Chaos::ThreadLocalSingleton unittest - @tid=" << cc::get_tid()
            << ", @message=" << USELESS_INS.get_message() << std::endl;
          USELESS_INS.set_message("Thread#1");
          std::cout
            << "Chaos::ThreadLocalSingleton unittest - @tid=" << cc::get_tid()
            << ", @message=" << USELESS_INS.get_message() << std::endl;
        });
    t.start();
    t.join();

    std::cout
      << "Chaos::ThreadLocalSingleton unittest - @tid=" << cc::get_tid()
      << ", @message=" << USELESS_INS.get_message() << std::endl;
#undef USELESS_INS
  }
}
