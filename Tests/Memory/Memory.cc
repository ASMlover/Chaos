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
#include <string>
#include <Chaos/Codecs/Base64.h>
#include <Chaos/Datetime/Timestamp.h>
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
  u1.swap(u2);
  u1->show();
  u2->show();
  u1 = nullptr;
  u2.reset();
}

CHAOS_TEST(ScopedArray, Chaos::FakeTester) {
  auto u = Chaos::ScopedArray<_UselessDemo>(new _UselessDemo[5]);
  for (auto i = 0; i < 5; ++i)
    u[i].show();
  u.reset();
}
