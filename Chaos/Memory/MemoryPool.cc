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
#include <Chaos/Memory/MemoryPool.h>

namespace Chaos {

struct MemoryBlock {
  MemoryBlock* nextblock;
};

MemoryPool::MemoryPool(void) {
}

MemoryPool::~MemoryPool(void) {
  for (auto* pool : pools_)
    free(pool);
  pools_.clear();
}

void* MemoryPool::alloc(size_t bytes) {
  void* p;

  if (bytes <= SMALL_REQUEST_THRESHOLD) {
    size_t index = bytes_to_index(bytes);
    if (freeblocks_[index] == nullptr)
      alloc_new_pool(index);

    p = freeblocks_[index];
    freeblocks_[index] = freeblocks_[index]->nextblock;
  }
  else {
    p = malloc(bytes);
  }

  return p;
}

void MemoryPool::dealloc(void* p, size_t bytes) {
  if (bytes <= SMALL_REQUEST_THRESHOLD) {
    size_t index = bytes_to_index(bytes);
    auto* free_block = reinterpret_cast<MemoryBlock*>(p);
    free_block->nextblock = freeblocks_[index];
    freeblocks_[index] = free_block;
  }
  else {
    free(p);
  }
}

MemoryBlock* MemoryPool::alloc_new_pool(size_t index) {
  size_t block_bytes = index_to_bytes(index);

  if (freeblocks_[index] == nullptr) {
    auto* new_pool = (MemoryBlock*)malloc(POOL_SIZE);
    if (new_pool == nullptr)
      return nullptr;
    pools_.push_back(new_pool);

    auto* block = freeblocks_[index];
    for (size_t i = 0; i < POOL_SIZE - block_bytes; i += block_bytes)
      block = block->nextblock = block + block_bytes / sizeof(MemoryBlock);
    block->nextblock = nullptr;
  }

  return freeblocks_[index];
}

}
