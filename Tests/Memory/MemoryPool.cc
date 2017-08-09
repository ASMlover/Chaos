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
#include <Chaos/Datetime/Timestamp.h>
#include <Chaos/Memory/MemoryPool.h>
#include <Chaos/Unittest/TestHarness.h>

CHAOS_TEST(MemoryPool, Chaos::FakeTester) {
  static constexpr int ALLOC_COUNT = 100000;
  static int alloc_bytes_list[ALLOC_COUNT]{};

  std::srand(Chaos::Timestamp::now().msec_since_epoch());
  for (auto i = 0; i < ALLOC_COUNT; ++i) {
    alloc_bytes_list[i] = rand() % 512;
    if (alloc_bytes_list[i] == 0)
      alloc_bytes_list[i] = 1;
  }

  std::int64_t beg, end;
  void* p{};

  {
    beg = Chaos::Timestamp::now().msec_since_epoch();
    for (auto sz : alloc_bytes_list) {
      p = std::malloc(sz);
      std::free(p);
    }
    end = Chaos::Timestamp::now().msec_since_epoch();
    std::cout
      << "Chaos::MemoryPool unittest - [system C allocator] use: "
      << end - beg << std::endl;
  }

  {
    Chaos::MemoryPool& pool = Chaos::MemoryPool::get_instance();
    beg = Chaos::Timestamp::now().msec_since_epoch();
    for (auto sz : alloc_bytes_list) {
      p = pool.alloc(sz);
      pool.dealloc(p);
    }
    end = Chaos::Timestamp::now().msec_since_epoch();
    std::cout
      << "Chaos::MemoryPool unittest - [memort pool allocator] use: "
      << end - beg << std::endl;
  }
}
