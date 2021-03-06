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
#pragma once

#include <vector>
#include <Chaos/Base/Platform.hh>
#include <Chaos/Base/UnCopyable.hh>

namespace Chaos {

struct MemoryBlock;

// An easy memory pool without thread-safe.
class MemoryPool : private UnCopyable {
  enum {
#if defined(CHAOS_ARCH32)
    ALIGNMENT = 4, // must be 2^N
    ALIGNMENT_SHIFT = 2,
#elif defined(CHAOS_ARCH64)
    ALIGNMENT = 8, // must be 2^N
    ALIGNMENT_SHIFT = 3,
#endif
    SMALL_REQUEST_THRESHOLD = 512,
    NB_SMALL_SIZE_CLASSES = (SMALL_REQUEST_THRESHOLD / ALIGNMENT),
    SYSTEM_PAGE_SIZE = (4 << 10),
    SYSTEM_PAGE_SIZE_MASK = (SYSTEM_PAGE_SIZE - 1),
    POOL_SIZE = (256 << 10), // default one pool size (256K)
    PAGE_COUNT = (POOL_SIZE / SYSTEM_PAGE_SIZE),
  };

  MemoryBlock* freeblocks_[NB_SMALL_SIZE_CLASSES]{};
  std::vector<void*> blocks_;

  MemoryPool(void);
  ~MemoryPool(void);

  inline std::size_t index2bytes(int index) {
    return static_cast<std::size_t>((index + 1) << ALIGNMENT_SHIFT);
  }

  inline int bytes2index(std::size_t nbytes) {
    return static_cast<int>((nbytes - 1) >> ALIGNMENT_SHIFT);
  }

  MemoryBlock* new_block(int index);
public:
  static MemoryPool& get_instance(void) {
    static MemoryPool ins;
    return ins;
  }

  void* alloc(std::size_t nbytes);
  void dealloc(void* p);
};

}
