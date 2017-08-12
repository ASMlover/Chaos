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
#include <cstdlib>
#include <Chaos/Types.h>
#include <Chaos/Memory/MemoryPool.h>

namespace Chaos {

struct BlockHeader {
  std::uintptr_t address;
  int index;
  int szindex;
};

struct MemoryBlock {
  MemoryBlock* next;
};

MemoryPool::MemoryPool(void) {
}

MemoryPool::~MemoryPool(void) {
  for (auto* p : blocks_)
    std::free(p);
  blocks_.clear();
}

void* MemoryPool::alloc(std::size_t nbytes) {
  void* p{};

  if ((nbytes - 1) < SMALL_REQUEST_THRESHOLD) {
    auto index = bytes2index(nbytes);
    if (freeblocks_[index] == nullptr) {
      auto* bp = new_block(index);
      CHAOS_CHECK(bp != nullptr, "create new block failed ...");
    }

    p = freeblocks_[index];
    if (p != nullptr)
      freeblocks_[index] = freeblocks_[index]->next;
  }
  else {
    p = std::malloc(nbytes);
  }

  return p;
}

void MemoryPool::dealloc(void* p) {
  if (p == nullptr)
    return;

  auto* header = reinterpret_cast<BlockHeader*>(
      (std::uintptr_t)p & ~SYSTEM_PAGE_SIZE_MASK);
  if (header->index < PAGE_COUNT
      && header->address != 0
      && (std::uintptr_t)p - header->address < POOL_SIZE) {
    auto index = header->szindex;
    auto* reclaim_block = reinterpret_cast<MemoryBlock*>(p);
    reclaim_block->next = freeblocks_[index];
    freeblocks_[index] = reclaim_block;
  }
  else {
    std::free(p);
  }
}

MemoryBlock* MemoryPool::new_block(int index) {
  auto* address = reinterpret_cast<byte_t*>(std::malloc(POOL_SIZE));
  if (address == nullptr)
    return nullptr;
  blocks_.push_back(address);

  std::size_t excess = static_cast<std::size_t>(
      (std::uintptr_t)address & SYSTEM_PAGE_SIZE_MASK);
  byte_t* alignment_address = address;
  std::size_t alignment_count = PAGE_COUNT;
  std::size_t alignment_bytes = POOL_SIZE;
  if (excess != 0) {
    alignment_address = address + SYSTEM_PAGE_SIZE - excess;
    alignment_count = PAGE_COUNT - 1;
    alignment_bytes = alignment_count * SYSTEM_PAGE_SIZE;
  }
  auto headsz = sizeof(BlockHeader);
  freeblocks_[index] =
    reinterpret_cast<MemoryBlock*>(alignment_address + headsz);

  auto nbytes = index2bytes(index);
  for (auto i = 0; i < static_cast<int>(alignment_count); ++i) {
    auto* header = reinterpret_cast<BlockHeader*>(alignment_address);
    header->address = (std::uintptr_t)address;
    header->index = i;
    header->szindex = index;

    auto* block = reinterpret_cast<MemoryBlock*>(alignment_address + headsz);
    auto block_bytes = SYSTEM_PAGE_SIZE - headsz - nbytes;
    for (std::size_t sz = 0u; sz < block_bytes; sz += nbytes)
      block = block->next = block + nbytes / sizeof(MemoryBlock);
    block->next = nullptr;

    alignment_address += SYSTEM_PAGE_SIZE;
  }

  return freeblocks_[index];
}

}
