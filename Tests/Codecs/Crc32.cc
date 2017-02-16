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
#include <stdint.h>
#include <string.h>
#include <Chaos/Codecs/Crc32c.h>
#include <Chaos/Unittest/TestHarness.h>

CHAOS_TEST(Crc32c, Chaos::FakeTester) {
  namespace cc = Chaos::Crc32c;

  char buf[32]{};
  CHAOS_CHECK_EQ(0x8a9136aau, cc::value(buf, sizeof(buf)));

  memset(buf, 0xff, sizeof(buf));
  CHAOS_CHECK_EQ(0x62a8ab43u, cc::value(buf, sizeof(buf)));

  for (auto i = 0; i < 32; ++i)
    buf[i] = i;
  CHAOS_CHECK_EQ(0x46dd794eu, cc::value(buf, sizeof(buf)));

  for (auto i = 0; i < 32; ++i)
    buf[i] = 31 - i;
  CHAOS_CHECK_EQ(0x113fdb5cu, cc::value(buf, sizeof(buf)));

  CHAOS_CHECK_NE(cc::value("a", 1), cc::value("foo", 3));
  CHAOS_CHECK_EQ(cc::value("ChaosCrc32c", 11), cc::extend(cc::value("Chaos", 5), "Crc32c", 6));

  uint32_t init_crc = cc::value("foo", 3);
  CHAOS_CHECK_NE(init_crc, cc::mask(init_crc));
  CHAOS_CHECK_NE(init_crc, cc::mask(cc::mask(init_crc)));
  CHAOS_CHECK_EQ(init_crc, cc::unmask(cc::mask(init_crc)));
  CHAOS_CHECK_EQ(init_crc, cc::unmask(cc::unmask(cc::mask(cc::mask(init_crc)))));
}
