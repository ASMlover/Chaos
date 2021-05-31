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
#include <string>
#include <Chaos/Codecs/Base64.hh>
#include <Chaos/Datetime/Timestamp.hh>
#include <Chaos/Memory/Memory.h>
#include <Chaos/Unittest/TestHarness.h>

static int kNidCounter = 1;

std::string _gen_uid(int nid) {
  char buf[128]{};
  std::snprintf(buf, sizeof(buf), "%s-%d",
      Chaos::Timestamp::now().to_string().c_str(), nid);
  return Chaos::Base64::encode(buf);
}

class _UselessDemo : private Chaos::UnCopyable {
  int nid_{};
  std::string uid_;
public:
  _UselessDemo(int nid = kNidCounter)
    : nid_(nid)
    , uid_(_gen_uid(nid)) {
    std::cout
      << "Chaos::ScopedPtr unittest - _UselessDemo::_UselessDemo "
      << "@this=" << this << ", @nid=" << nid_ << std::endl;
    kNidCounter++;
  }

  ~_UselessDemo(void) {
    std::cout
      << "Chaos::ScopedPtr unittest - _UselessDemo::~_UselessDemo "
      << "@this=" << this << ", @nid=" << nid_ << std::endl;
  }

  void show(void) {
    std::cout
      << "Chaos::ScopedPtr unittest - _UselessDemo::show "
      << "@this=" << this << ", @nid=" << nid_ << ", @uid=" << uid_
      << std::endl;
  }
};

CHAOS_TEST(ScopedPtr, Chaos::FakeTester) {
  auto u1 = Chaos::ScopedPtr<_UselessDemo>(new _UselessDemo(1));
  u1->show();
  auto u2 = Chaos::ScopedPtr<_UselessDemo>(new _UselessDemo(2));
  u2->show();
  CHAOS_CHECK_TRUE(nullptr != u1);
  CHAOS_CHECK_TRUE(nullptr != u2);
  u1.swap(u2);
  u1->show();
  u2->show();
  u1 = nullptr;
  u2.reset();
  CHAOS_CHECK_TRUE(u1 == nullptr);
  CHAOS_CHECK_TRUE(u2 == nullptr);
}

CHAOS_TEST(ScopedArray, Chaos::FakeTester) {
  auto u = Chaos::ScopedArray<_UselessDemo>(new _UselessDemo[5]);
  for (auto i = 0; i < 5; ++i)
    u[i].show();
  u.reset();
}

CHAOS_TEST(SharedPtr, Chaos::FakeTester) {
  auto p1 = new _UselessDemo(11);
  auto u1 = Chaos::SharedPtr<_UselessDemo>(p1);
  CHAOS_CHECK_EQ(p1, u1.get());
  CHAOS_CHECK_TRUE(nullptr != u1);
  CHAOS_CHECK_TRUE(u1 != nullptr);

  auto p2 = new _UselessDemo(22);
  auto u2 = Chaos::SharedPtr<_UselessDemo>(p2);
  CHAOS_CHECK_EQ(p2, u2.get());
  CHAOS_CHECK_TRUE(nullptr != u2);
  CHAOS_CHECK_TRUE(u2 != nullptr);

  CHAOS_CHECK_TRUE(u1 != u2);

  Chaos::SharedPtr<_UselessDemo> u1x(u1);
  CHAOS_CHECK_TRUE(u1 == u1x);
  Chaos::SharedPtr<_UselessDemo> u2x(u2);
  CHAOS_CHECK_TRUE(u2 == u2x);

  u1.swap(u2);
  CHAOS_CHECK_TRUE(u1 == u2x);
  CHAOS_CHECK_TRUE(u2 == u1x);

  u1x.reset();
  u2x.reset();
  CHAOS_CHECK_TRUE(u1x == u2x);
  CHAOS_CHECK_TRUE(u1x == nullptr);
  CHAOS_CHECK_TRUE(u2x == nullptr);

  Chaos::SharedPtr<_UselessDemo> u1y(std::move(u1));
  CHAOS_CHECK_TRUE(u1 == nullptr);
  Chaos::SharedPtr<_UselessDemo> u2y(std::move(u2));
  CHAOS_CHECK_TRUE(u2 == nullptr);

  u1y.swap(u2y);
  u1y->show();
  u2y->show();

  u1y = nullptr;
  u2y.reset();
}

CHAOS_TEST(SharedArray, Chaos::FakeTester) {
  auto u1 = Chaos::SharedArray<_UselessDemo>(new _UselessDemo[5]);
  for (auto i = 0; i < 5; ++i)
    u1[i].show();

  auto u2 = Chaos::SharedArray<_UselessDemo>(new _UselessDemo[5]);
  for (auto i = 0; i < 5; ++i)
    u2[i].show();

  CHAOS_CHECK_TRUE(nullptr != u1 && nullptr != u2);
  CHAOS_CHECK_TRUE(u1 != u2);

  Chaos::SharedArray<_UselessDemo> u1x(u1);
  Chaos::SharedArray<_UselessDemo> u2x(u2);
  CHAOS_CHECK_TRUE(u1 == u1x);
  CHAOS_CHECK_TRUE(u2 == u2x);

  u1x.swap(u2x);
  CHAOS_CHECK_TRUE(u1 == u2x && u2 == u1x);

  u1x = nullptr;
  u2x.reset();
  CHAOS_CHECK_TRUE(nullptr == u1x && u2x == nullptr);

  Chaos::SharedArray<_UselessDemo> u1y(std::move(u1));
  Chaos::SharedArray<_UselessDemo> u2y(std::move(u2));
  CHAOS_CHECK_TRUE(u1 == u2);
  CHAOS_CHECK_TRUE(u1 == nullptr && nullptr == u2 && !u1 && !u2);

  u1y.reset();
  u2y.reset();
  CHAOS_CHECK_TRUE(u1y == u2y && !u1y && !u2y);
}

CHAOS_TEST(WeakPtr, Chaos::FakeTester) {
  auto sp = Chaos::SharedPtr<_UselessDemo>(new _UselessDemo(555));
  auto wp = Chaos::WeakPtr<_UselessDemo>(sp);
  CHAOS_CHECK_TRUE(sp.get() == wp.lock().get());
  sp.reset();
  CHAOS_CHECK_TRUE(wp.expired());
}
