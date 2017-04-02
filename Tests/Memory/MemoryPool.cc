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
  static const int ALLOC_COUNT = 100000;

  int64_t beg, end;
  {
    beg = Chaos::Timestamp::now().msec_since_epoch();
    for (int i = 0; i < ALLOC_COUNT; ++i) {
      int* p = (int*)malloc(sizeof(int));
      free(p);
    }
    end = Chaos::Timestamp::now().msec_since_epoch();
    std::cout << "Chaos::MemoryPool unittest - [system C allocator] use: " << end - beg << std::endl;
  }

  {
    beg = Chaos::Timestamp::now().msec_since_epoch();
    for (int i = 0; i < ALLOC_COUNT; ++i) {
      int* p = new int;
      delete p;
    }
    end = Chaos::Timestamp::now().msec_since_epoch();
    std::cout << "Chaos::MemoryPool unittest - [system C++ allocator] use: " << end - beg << std::endl;
  }

  {
    Chaos::MemoryPool& pool = Chaos::MemoryPool::get_instace();
    beg = Chaos::Timestamp::now().msec_since_epoch();
    for (int i = 0; i < ALLOC_COUNT; ++i) {
      int* p = (int*)pool.alloc(sizeof(int));
      pool.dealloc(p, sizeof(int));
    }
    end = Chaos::Timestamp::now().msec_since_epoch();
    std::cout << "Chaos::MemoryPool unittest - [memort pool allocator] use: " << end - beg << std::endl;
  }
}
