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
#include <Chaos/Datetime/Timestamp.hh>
#include <Chaos/Memory/MemoryPool.h>
#include <Chaos/Unittest/TestHarness.h>

CHAOS_TEST(MemoryPool, Chaos::FakeTester) {
  static constexpr int ALLOC_COUNT = 100000;
  int* alloc_bytes_array = new int[ALLOC_COUNT];

  std::srand(Chaos::Timestamp::now().microsec_since_epoch());
  for (auto i = 0; i < ALLOC_COUNT; ++i) {
    alloc_bytes_array[i] = rand() % 512;
    if (alloc_bytes_array[i] == 0)
      alloc_bytes_array[i] = 1;
  }

  std::int64_t beg, end;
  void* p{};
  Chaos::MemoryPool& pool = Chaos::MemoryPool::get_instance();

  {
    // test fixed size
    std::size_t sz = 15;
    {
      beg = Chaos::Timestamp::now().microsec_since_epoch();
      for (auto i = 0; i < ALLOC_COUNT; ++i) {
        p = std::malloc(sz);
        std::free(p);
      }
      end = Chaos::Timestamp::now().microsec_since_epoch();
      std::cout
        << "Chaos::MemoryPool unittest - [system C allocator] fixed size"
        << "(free immediately) use: "
        << end - beg << " microseconds" << std::endl;
    }
    {
      beg = Chaos::Timestamp::now().microsec_since_epoch();
      for (auto i = 0; i < ALLOC_COUNT; ++i) {
        p = pool.alloc(sz);
        pool.dealloc(p);
      }
      end = Chaos::Timestamp::now().microsec_since_epoch();
      std::cout
        << "Chaos::MemoryPool unittest - [memory pool allocator] fixed size"
        << "(dealloc immediately) use: "
        << end - beg << " microseconds" << std::endl;
    }

    void** parray = new void*[ALLOC_COUNT];
    {
      beg = Chaos::Timestamp::now().microsec_since_epoch();
      for (auto i = 0; i < ALLOC_COUNT; ++i)
        parray[i] = std::malloc(sz);
      for (auto i = 0; i < ALLOC_COUNT; ++i)
        std::free(parray[i]);
      end = Chaos::Timestamp::now().microsec_since_epoch();
      std::cout
        << "Chaos::MemoryPool unittest - [system C allocator] fixed size use: "
        << end - beg << " microseconds" << std::endl;
    }
    {
      beg = Chaos::Timestamp::now().microsec_since_epoch();
      for (auto i = 0; i < ALLOC_COUNT; ++i)
        parray[i] = pool.alloc(sz);
      for (auto i = 0; i < ALLOC_COUNT; ++i)
        pool.dealloc(parray[i]);
      end = Chaos::Timestamp::now().microsec_since_epoch();
    }
    delete [] parray;
  }

  {
    // test random size
    {
      beg = Chaos::Timestamp::now().microsec_since_epoch();
      for (auto i = 0; i < ALLOC_COUNT; ++i) {
        p = std::malloc(alloc_bytes_array[i]);
        std::free(p);
      }
      end = Chaos::Timestamp::now().microsec_since_epoch();
      std::cout
        << "Chaos::MemoryPool unittest - [system C allocator] random size"
        << "(free immediately) use: "
        << end - beg << " microseconds" << std::endl;
    }
    {
      beg = Chaos::Timestamp::now().microsec_since_epoch();
      for (auto i = 0; i < ALLOC_COUNT; ++i) {
        p = pool.alloc(alloc_bytes_array[i]);
        pool.dealloc(p);
      }
      end = Chaos::Timestamp::now().microsec_since_epoch();
      std::cout
        << "Chaos::MemoryPool unittest - [memory pool allocator] random size"
        << "(dealloc immediately) use: "
        << end - beg << " microseconds" << std::endl;
    }

    void** parray = new void*[ALLOC_COUNT];
    {
      beg = Chaos::Timestamp::now().microsec_since_epoch();
      for (auto i = 0; i < ALLOC_COUNT; ++i)
        parray[i] = std::malloc(alloc_bytes_array[i]);
      for (auto i = 0; i < ALLOC_COUNT; ++i)
        std::free(parray[i]);
      end = Chaos::Timestamp::now().microsec_since_epoch();
      std::cout
        << "Chaos::MemoryPool unittest - [system C allocator] ramond size "
        << "use: " << end - beg << " microseconds" << std::endl;
    }
    {
      beg = Chaos::Timestamp::now().microsec_since_epoch();
      for (auto i = 0; i < ALLOC_COUNT; ++i)
        parray[i] = pool.alloc(alloc_bytes_array[i]);
      for (auto i = 0; i < ALLOC_COUNT; ++i)
        pool.dealloc(parray[i]);
      end = Chaos::Timestamp::now().microsec_since_epoch();
      std::cout
        << "Chaos::MemoryPool unittest - [memory pool allocator] ramond size "
        << "use: " << end - beg << " microseconds" << std::endl;
    }
    delete [] parray;
  }

  delete [] alloc_bytes_array;
}
