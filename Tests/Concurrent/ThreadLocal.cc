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
#include <iostream>
#include <string>
#include <Chaos/Base/UnCopyable.h>
#include <Chaos/Concurrent/CurrentThread.h>
#include <Chaos/Concurrent/Thread.h>
#include <Chaos/Concurrent/ThreadLocal.h>
#include <Chaos/Unittest/TestHarness.h>

namespace cc = ::Chaos::CurrentThread;

class Useless : private Chaos::UnCopyable {
  std::string message_;
public:
  Useless(void) {
    std::cout
      << "Chaos::ThreadLocal unittest - Useless:Useless @tid="
      << cc::get_tid() << std::endl;
  }

  ~Useless(void) {
    std::cout
      << "Chaos::ThreadLocal unittest - Useless:~Useless @tid="
      << cc::get_tid() << std::endl;
  }

  void set_message(const std::string& msg) {
    message_ = msg;
  }

  const std::string& get_message(void) const {
    return message_;
  }
};

Chaos::ThreadLocal<Useless> _useless_obj1;
Chaos::ThreadLocal<Useless> _useless_obj2;

static void show_useless(void) {
  std::cout
    << "Chaos::ThreadLocal unittest - _useless_obj1 @message="
    << _useless_obj1.get_value().get_message() << ", @tid=" << cc::get_tid()
    << std::endl;
  std::cout
    << "Chaos::ThreadLocal unittest - _useless_obj2 @message="
    << _useless_obj2.get_value().get_message() << ", @tid=" << cc::get_tid()
    << std::endl;
}

CHAOS_TEST(ThreadLocal, Chaos::FakeTester) {
  _useless_obj1.get_value().set_message("Main.unittest#1");
  show_useless();

  Chaos::Thread t([] {
        show_useless();

        _useless_obj1.get_value().set_message("Thread1.1001");
        _useless_obj2.get_value().set_message("Thread2.1002");

        show_useless();
      });
  t.start();
  t.join();

  _useless_obj2.get_value().set_message("Main.unittest#2");
  show_useless();
}
