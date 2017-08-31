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
#include <Chaos/Unittest/TestHarness.h>
#include <Chaos/Concurrent/CurrentThread.h>
#include <Chaos/Datetime/Timestamp.h>
#include <iostream>

  // void cached_tid(void);
  // int get_tid(void); // get tid of current thread
  // const char* get_strftid(void); // string format tid
  // int get_strftid_length(void); // length of string format tid
  // const char* get_name(void); // get name of current thread
  // bool is_main_thread(void);
  // void sleep_microsec(std::uint64_t microsec);
CHAOS_TEST(CurrentThread, Chaos::FakeTester) {
  Chaos::CurrentThread::cached_tid();
  std::cout
    << "Chaos::CurrentThread unittest, "
    << "@tid=" << Chaos::CurrentThread::get_tid() << ", "
    << "@tid.string=" << Chaos::CurrentThread::get_strftid() << ", "
    << "@tid.string.len=" << Chaos::CurrentThread::get_strftid_length() << ", "
    << "@name=" << Chaos::CurrentThread::get_name()
    << "@is_main_thread=" << Chaos::CurrentThread::is_main_thread()
    << std::endl;

  std::cout
    << "Chaos::CurrentThread unittest, @beg.time="
    << Chaos::get_microsec() << std::endl;
  Chaos::CurrentThread::sleep_microsec(100);
  std::cout
    << "Chaos::CurrentThread unittest, @end.time="
    << Chaos::get_microsec() << std::endl;
}
