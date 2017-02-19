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
#include <memory>
#include <thread>
#include <vector>
#include <Chaos/Concurrent/BoundedBlockingQueue.h>
#include <Chaos/Concurrent/CountdownLatch.h>
#include <Chaos/Logging/Logging.h>
#include <Chaos/Unittest/TestHarness.h>

CHAOS_TEST(BoundedBlockingQueue, Chaos::FakeTester) {
  {
    const int COUNT = 4;
    Chaos::CountdownLatch latch(COUNT);
    Chaos::BoundedBlockingQueue<int> queue(20);
    std::vector<std::unique_ptr<std::thread>> threads;

    char buf[32]{};
    for (auto i = 0; i < COUNT; ++i) {
      snprintf(buf, sizeof(buf), "THREAD#%d", i + 1);
      std::string name(buf);
      threads.emplace_back(new std::thread([&latch, &queue, name] {
              Chaos::CurrentThread::cached_tid();
              CHAOSLOG_INFO << "Chaos::BoundedBlockingQueue unitest - thread [" << name << "] running ...";

              latch.countdown();
              bool running = true;
              while (running) {
                auto x(queue.fetch_out());
                CHAOSLOG_INFO << "Chaos::BoundedBlockingQueue unittest - fetch_out @x=" << x;
                running = (x != 0);
              }
              CHAOSLOG_INFO << "Chaos::BoundedBlockingQueue unittest - thread [" << name << "] stoping ...";
            }));
    }

    latch.wait();
    for (auto i = 0; i < 100; ++i) {
      queue.put_in(i + 1);
      CHAOSLOG_INFO << "Chaos::BoundedBlockingQueue unittest - @size=" << queue.size();
    }

    for (auto i = 0; i < COUNT; ++i)
      queue.put_in(0);
    for (auto& t : threads)
      t->join();
  }
}
