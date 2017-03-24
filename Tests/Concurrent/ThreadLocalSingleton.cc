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
#include <string>
#include <Chaos/UnCopyable.h>
#include <Chaos/Concurrent/CurrentThread.h>
#include <Chaos/Concurrent/Thread.h>
#include <Chaos/Concurrent/ThreadLocalSingleton.h>
#include <Chaos/Logging/Logging.h>
#include <Chaos/Unittest/TestHarness.h>

namespace cc = ::Chaos::CurrentThread;

class Useless : private Chaos::UnCopyable {
  std::string message_;
public:
  Useless(void) {
    CHAOSLOG_INFO << "Chaos::ThreadLocalSingleton unittest - Useless::Useless @tid=" << cc::get_tid();
  }

  ~Useless(void) {
    CHAOSLOG_INFO << "Chaos::ThreadLocalSingleton unittest - Useless::~Useless @tid=" << cc::get_tid();
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
    Chaos::ThreadLocalSingleton<Useless>::get_instance().set_message("Main#1");

#if !defined(CHAOS_DARWIN)
    Chaos::Thread t([] {
          CHAOSLOG_INFO << "Chaos::ThreadLocalSingleton unittest - @tid=" << cc::get_tid()
            << ", @message=" << Chaos::ThreadLocalSingleton<Useless>::get_instance().get_message();
          Chaos::ThreadLocalSingleton<Useless>::get_instance().set_message("Thread#1");
          CHAOSLOG_INFO << "Chaos::ThreadLocalSingleton unittest - @tid=" << cc::get_tid()
            << ", @message=" << Chaos::ThreadLocalSingleton<Useless>::get_instance().get_message();
        });
    t.start();
    t.join();
#endif

    CHAOSLOG_INFO << "Chaos::ThreadLocalSingleton unittest - @tid=" << cc::get_tid()
      << ", @message=" << Chaos::ThreadLocalSingleton<Useless>::get_instance().get_message();
  }
}
