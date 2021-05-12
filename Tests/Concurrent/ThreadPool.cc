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
#include <Chaos/Unittest/TestHarness.h>
#include <Chaos/Concurrent/ThreadPool.h>
#include <Chaos/Concurrent/CurrentThread.h>
#include <Chaos/Concurrent/CountdownLatch.hh>
#include <Chaos/Logging/Logging.h>
#include <iostream>

static void do_print(void) {
  std::cout
    << "Chaos::ThreadPool unittest, @tid=" << Chaos::CurrentThread::get_tid()
    << std::endl;
}

static void do_test(int max_count) {
  CHAOSLOG_INFO << "Chaos::ThreadPool unittest, @max_count=" << max_count;
  Chaos::ThreadPool pool("TestThreadPool");
  pool.set_tasks_capacity(max_count);
  pool.start(5);

  pool.run(do_print);
  pool.run(do_print);
  pool.run(do_print);

  for (int i = 0; i < 10; ++i) {
    char buf[32];
    snprintf(buf, sizeof(buf), "task#%d", i + 1);
    pool.run([buf] {
      CHAOSLOG_INFO << "Chaos::ThreadPool unittest, @buf=" << buf;
      Chaos::CurrentThread::sleep_microsec(1000 * 100);
    });
  }

  Chaos::CountdownLatch latch(1);
  pool.run(std::bind(&Chaos::CountdownLatch::countdown, &latch));
  latch.wait();

  pool.stop();
}

CHAOS_TEST(ThreadPool, Chaos::FakeTester) {
  do_test(0);
  do_test(1);
  do_test(4);
  do_test(8);
}
