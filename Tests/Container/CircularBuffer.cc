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
#include <Chaos/Container/CircularBuffer.h>
#include <Chaos/Logging/Logging.h>
#include <Chaos/Unittest/TestHarness.h>

CHAOS_TEST(CircularBuffer, Chaos::FakeTester) {
  {
    Chaos::CircularBuffer<int> c;
    CHAOSLOG_INFO
      << "Chaos::CircularBuffer unittest - @size=" << c.size()
      << ", @capacity=" << c.capacity();
    CHAOS_CHECK_TRUE(!c);
    CHAOS_CHECK_TRUE(c.empty());
    CHAOS_CHECK_TRUE(c.full());
    CHAOS_CHECK_TRUE(c.size() == 0);
    CHAOS_CHECK_TRUE(c.capacity() == 0);
    CHAOS_CHECK_TRUE(c.data() == nullptr);
    CHAOS_CHECK_TRUE(c.begin() == c.end());
    CHAOS_CHECK_TRUE(c.rbegin() == c.rend());
  }

  {
    Chaos::CircularBuffer<int> c(5);
    CHAOSLOG_INFO
      << "Chaos::CircularBuffer unittest - @size=" << c.size()
      << ", @capacity=" << c.capacity();
    CHAOS_CHECK_TRUE(static_cast<bool>(c));
    CHAOS_CHECK_TRUE(c.empty());
    CHAOS_CHECK_TRUE(!c.full());
    CHAOS_CHECK_TRUE(c.size() == 0);
    CHAOS_CHECK_TRUE(c.capacity() == 5);
    CHAOS_CHECK_TRUE(c.data() != nullptr);
    CHAOS_CHECK_TRUE(c.begin() == c.end());
    CHAOS_CHECK_TRUE(c.rbegin() == c.rend());
  }

  {
    Chaos::CircularBuffer<int> c(5);
    CHAOSLOG_INFO
      << "Chaos::CircularBuffer unittest - @size=" << c.size()
      << ", @capacity=" << c.capacity();
    c.push_back(1);
    CHAOS_CHECK_TRUE(static_cast<bool>(c));
    CHAOS_CHECK_TRUE(!c.empty());
    CHAOS_CHECK_TRUE(!c.full());
    CHAOS_CHECK_TRUE(c.size() == 1);
    CHAOS_CHECK_TRUE(c.capacity() == 5);
    CHAOS_CHECK_TRUE(c.data() != nullptr);
    CHAOS_CHECK_TRUE(c.begin() != c.end());
    CHAOS_CHECK_TRUE(c.rbegin() != c.rend());
    CHAOS_CHECK_TRUE(c.front() == 1);
    CHAOS_CHECK_TRUE(c.at(0) == 1);

    c.push_back(2);
    CHAOS_CHECK_TRUE(c.size() == 2);
    CHAOS_CHECK_TRUE(c.front() == 1);
    CHAOS_CHECK_TRUE(c.back() == 2);
    CHAOS_CHECK_TRUE(c.at(0) == 1);
    CHAOS_CHECK_TRUE(c.at(1) == 2);

    c.push_back(3);
    c.push_back(4);
    c.push_back(5);
    CHAOS_CHECK_TRUE(c.full());
    CHAOS_CHECK_TRUE(c.size() == 5);
    CHAOS_CHECK_TRUE(c.front() == 1);
    CHAOS_CHECK_TRUE(c.back() == 5);
    CHAOS_CHECK_TRUE(c.at(0) == c[0] && c.at(0) == 1);
    CHAOS_CHECK_TRUE(c.at(1) == c[1] && c.at(1) == 2);
    CHAOS_CHECK_TRUE(c.at(2) == c[2] && c.at(2) == 3);
    CHAOS_CHECK_TRUE(c.at(3) == c[3] && c.at(3) == 4);
    CHAOS_CHECK_TRUE(c.at(4) == c[4] && c.at(4) == 5);

    c.push_back(100);
    CHAOS_CHECK_TRUE(c.full());
    CHAOS_CHECK_TRUE(c.size() == 5);
    CHAOS_CHECK_TRUE(c.front() == 2);
    CHAOS_CHECK_TRUE(c.back() == 100);

    c.pop_front();
    CHAOS_CHECK_TRUE(c.size() == 4);
    CHAOS_CHECK_TRUE(!c.full());
    CHAOS_CHECK_TRUE(c.front() == 3);

    c.clear();
    CHAOS_CHECK_TRUE(c.size() == 0);
    CHAOS_CHECK_TRUE(c.empty());
    CHAOS_CHECK_TRUE(!c.full());
  }
}
