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
#include <Chaos/Memory/IntrusivePtr.h>
#include <Chaos/Unittest/TestHarness.h>

class _IntrusiveDemo :
  public Chaos::IntrusiveRefCounter<_IntrusiveDemo, Chaos::CountedThreadSafe> {
  int nid_{};
public:
  _IntrusiveDemo(int nid = 0)
    : nid_(nid) {
    std::cout
      << "Chaos::IntrusivePtr unittest - _IntrusiveDemo::_IntrusiveDemo "
      << "@this=" << this << ", @nid=" << nid_ << std::endl;
  }

  ~_IntrusiveDemo(void) {
    std::cout
      << "Chaos::IntrusivePtr unittest - _IntrusiveDemo::~_IntrusiveDemo "
      << "@this=" << this << ", @nid=" << nid_ << std::endl;
  }

  int get_nid(void) const {
    return nid_;
  }
};

CHAOS_TEST(IntrusivePtr, Chaos::FakeTester) {
  auto ip = Chaos::IntrusivePtr<_IntrusiveDemo>();
  CHAOS_CHECK_TRUE(nullptr == ip);

  auto p1 = new _IntrusiveDemo(111);
  auto i1 = Chaos::IntrusivePtr<_IntrusiveDemo>(p1);
  CHAOS_CHECK_TRUE(p1 == i1.get() && 111 == i1->get_nid());
  CHAOS_CHECK_TRUE(1 == i1->use_count());

  auto p2 = new _IntrusiveDemo(222);
  auto i2 = Chaos::IntrusivePtr<_IntrusiveDemo>(p2);
  CHAOS_CHECK_TRUE(p2 == i2.get() && 222 == i2->get_nid());
  CHAOS_CHECK_TRUE(1 == i2->use_count());

  Chaos::IntrusivePtr<_IntrusiveDemo> i1x(i1);
  Chaos::IntrusivePtr<_IntrusiveDemo> i2x(i2);
  CHAOS_CHECK_TRUE(i1 == i1x && i2 == i2x);
  CHAOS_CHECK_TRUE(i1->use_count() == 2 && i2->use_count() == 2);

  i1x.swap(i2x);
  CHAOS_CHECK_TRUE(i1 == i2x && i2 == i1x);
  i1x.reset();
  i2x.reset();
  CHAOS_CHECK_TRUE(i1->use_count() == 1 && i2->use_count() == 1);

  Chaos::IntrusivePtr<_IntrusiveDemo> i1y(std::move(i1));
  Chaos::IntrusivePtr<_IntrusiveDemo> i2y(std::move(i2));
  CHAOS_CHECK_TRUE(i1 == nullptr && i2 == nullptr);

  CHAOS_CHECK_TRUE(i1y->get_nid() == 111 && i2y->get_nid() == 222);
  i1y.detach();
  CHAOS_CHECK_TRUE(i1y == nullptr && p1->use_count() == 1);
  delete p1;
  i2y.reset();
}
