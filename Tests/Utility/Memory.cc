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
#include <Chaos/Base/UnCopyable.h>
#include <Chaos/Utility/Memory.h>
#include <Chaos/Unittest/TestHarness.h>

class _UtilityCase : Chaos::UnCopyable {
  int nid_{};
public:
  _UtilityCase(int nid = 0)
    : nid_(nid) {
    std::cout
      << "Chaos::Utility.Memory unittest - _UtilityCase::_UtilityCase "
      << "@this=" << this << ", @nid=" << nid_ << std::endl;
  }

  ~_UtilityCase(void) {
    std::cout
      << "Chaos::Utility.Memory unittest - _UtilityCase::~_UtilityCase "
      << "@this=" << this << ", @nid=" << nid_ << std::endl;
  }

  void show(void) {
    std::cout
      << "Chaos::Utility.Memory unittest - _UtilityCase::show "
      << "@this=" << this << ", @nid=" << nid_ << std::endl;
  }

  int get_nid(void) const {
    return nid_;
  }
};

CHAOS_TEST(UtilityMakeUnique, Chaos::FakeTester) {
  auto u1 = Chaos::make_unique<_UtilityCase>();
  CHAOS_CHECK_EQ(0, u1->get_nid());
  u1->show();

  auto u2 = Chaos::make_unique<_UtilityCase>(23);
  CHAOS_CHECK_EQ(23, u2->get_nid());
  u2->show();
}

CHAOS_TEST(UtilityAsUnique, Chaos::FakeTester) {
  auto p1 = new _UtilityCase();
  auto u1 = Chaos::as_unique<_UtilityCase>(p1);
  CHAOS_CHECK_EQ(0, u1->get_nid());
  CHAOS_CHECK_EQ(p1, u1.get());

  auto p2 = new _UtilityCase(23);
  auto u2 = Chaos::as_unique<_UtilityCase>(p2);
  CHAOS_CHECK_EQ(23, u2->get_nid());
  CHAOS_CHECK_EQ(p2, u2.get());
}

CHAOS_TEST(UtilityMakeScoped, Chaos::FakeTester) {
  auto u1 = Chaos::make_scoped<_UtilityCase>();
  CHAOS_CHECK_EQ(0, u1->get_nid());
  u1->show();

  auto u2 = Chaos::make_scoped<_UtilityCase>(23);
  CHAOS_CHECK_EQ(23, u2->get_nid());
  u2->show();
}

CHAOS_TEST(UtilityAsScoped, Chaos::FakeTester) {
  auto p1 = new _UtilityCase();
  auto u1 = Chaos::as_scoped<_UtilityCase>(p1);
  CHAOS_CHECK_EQ(0, u1->get_nid());
  CHAOS_CHECK_EQ(p1, u1.get());

  auto p2 = new _UtilityCase(23);
  auto u2 = Chaos::as_scoped<_UtilityCase>(p2);
  CHAOS_CHECK_EQ(23, u2->get_nid());
  CHAOS_CHECK_EQ(p2, u2.get());
}

CHAOS_TEST(UtilityRawPtr, Chaos::FakeTester) {
  int n = 55;
  CHAOS_CHECK_EQ(&n, Chaos::raw_ptr(&n));

  auto p1 = new _UtilityCase(n);
  auto u1 = Chaos::as_unique<_UtilityCase>(p1);
  CHAOS_CHECK_EQ(p1, Chaos::raw_ptr(u1));

  int* p2 = nullptr;
  CHAOS_CHECK_TRUE(nullptr == Chaos::raw_ptr(p2));

  std::unique_ptr<_UtilityCase> p3;
  CHAOS_CHECK_TRUE(nullptr == Chaos::raw_ptr(p3));
}

CHAOS_TEST(UtilityWeakenPtr, Chaos::FakeTester) {
  auto sp1 = std::make_shared<_UtilityCase>(11);
  auto wp1 = Chaos::weaken_ptr(sp1);
  CHAOS_CHECK_EQ(sp1.get(), wp1.lock().get());
  CHAOS_CHECK_EQ(sp1->get_nid(), wp1.lock()->get_nid());

  auto sp2 = Chaos::make_shared<_UtilityCase>(22);
  auto wp2 = Chaos::weaken_ptr(sp2);
  CHAOS_CHECK_EQ(sp2.get(), wp2.lock().get());
  CHAOS_CHECK_EQ(sp2->get_nid(), wp2.lock()->get_nid());
}
