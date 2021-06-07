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
#include <memory>
#include <thread>
#include <vector>
#include <Chaos/Concurrent/BlockingQueue.hh>
#include <Chaos/Concurrent/CountdownLatch.hh>
#include <Chaos/Logging/Logging.hh>
#include <Chaos/Unittest/TestHarness.h>

CHAOS_TEST(BlockingQueue, Chaos::FakeTester) {
  {
    Chaos::BlockingQueue<int> queue;
    queue.put_in(42);
    auto x = queue.fetch_out();
    CHAOS_CHECK_EQ(x, 42);
  }

  {
    Chaos::BlockingQueue<std::unique_ptr<int>> queue;
    queue.put_in(std::unique_ptr<int>(new int(42)));
    auto x = queue.fetch_out();
    CHAOS_CHECK_EQ(42, *x);
    *x = 100;
    queue.put_in(std::move(x));
    auto y = queue.fetch_out();
    CHAOS_CHECK_EQ(100, *y);
  }

  {
    const int COUNT = 4;
    Chaos::CountdownLatch latch(COUNT);
    Chaos::BlockingQueue<int> queue;
    std::vector<std::unique_ptr<std::thread>> threads;

    char buf[32];
    for (auto i = 0; i < COUNT; ++i) {
      snprintf(buf, sizeof(buf), "THREAD#%d", i + 1);
      std::string name(buf);
      threads.emplace_back(new std::thread([&latch, &queue, name] {
              Chaos::CurrentThread::cached_tid();
              CHAOSLOG_INFO
                << "Chaos::BlockingQueue unittest - thread ["
                << name << "] running ...";

              latch.countdown();
              bool running = true;
              while (running) {
                auto x(queue.fetch_out());
                CHAOSLOG_INFO
                  << "Chaos::BlockingQueue unittest - fetch_out @x=" << x;
                running = (x != 0);
              }

              CHAOSLOG_INFO
                << "Chaos::BlockingQueue unittest - thread ["
                << name << "] stoping ...";
            }));
    }

    latch.wait();
    for (auto i = 0; i < 100; ++i) {
      queue.put_in(i + 1);
      CHAOSLOG_INFO << "Chaos::BlockingQueue unittest - @size=" << queue.size();
    }

    for (auto i = 0; i < COUNT; ++i)
      queue.put_in(0);
    for (auto& t : threads)
      t->join();
  }
}
