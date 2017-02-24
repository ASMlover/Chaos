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
#include <ctime>
#include <Chaos/Datetime/Timezone.h>
#include <Chaos/Logging/Logging.h>
#include <Chaos/Unittest/TestHarness.h>

CHAOS_TEST(Timezone, Chaos::FakeTester) {
  {
    Chaos::Timezone t;
    CHAOS_CHECK_TRUE(!t.is_valid());
    std::tm utc(Chaos::Timezone::to_utc_time(std::time(nullptr), true));
    CHAOSLOG_INFO << "Chaos::Timezone unittest - @from_utc_time=" << Chaos::Timezone::from_utc_time(utc);
    CHAOSLOG_INFO << "Chaos::Timezone unittest - @from_utc_time(2017-2-24 17:30:30)="
      << Chaos::Timezone::from_utc_time(2017, 2, 24, 17, 30, 30);
  }

  {
    Chaos::Timezone t(8 * 3600, "CST");
    std::tm utc = t.to_localtime(std::time(nullptr));
    CHAOS_CHECK_TRUE(t.is_valid());
    CHAOSLOG_INFO << "Chaos::Timezone unittest - @localtime=" << t.from_localtime(utc);
  }
}
