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
#include <ctime>
#include <Chaos/Datetime/Timestamp.hh>
#include <Chaos/Logging/Logging.hh>
#include <Chaos/Unittest/TestHarness.h>

CHAOS_TEST(Timestamp, Chaos::FakeTester) {
  {
    Chaos::Timestamp t;
    CHAOSLOG_INFO
      << "Chaos::Timestamp unittest - " << t.to_string()
      << "|" << t.to_formatted_string(true);
    CHAOS_CHECK_TRUE(!t.is_valid());
    CHAOS_CHECK_TRUE(t.microsec_since_epoch() == 0);
    CHAOS_CHECK_TRUE(t.millisec_since_epoch() == 0);
    CHAOS_CHECK_TRUE(t.sec_since_epoch() == 0);
  }

  {
    auto epoch = std::time(nullptr) * Chaos::Timestamp::kMicrosecondsPerSecond;
    Chaos::Timestamp t(epoch);
    CHAOSLOG_INFO
      << "Chaos::Timestamp unittest - " << t.to_string()
      << "|" << t.to_formatted_string(true);
    CHAOS_CHECK_TRUE(t.is_valid());
    CHAOS_CHECK_TRUE(t.microsec_since_epoch() == epoch);
    CHAOS_CHECK_TRUE(t.millisec_since_epoch() ==
        epoch / Chaos::Timestamp::kMillisecondsPerSecond);
    CHAOS_CHECK_TRUE(t.sec_since_epoch() ==
        epoch / Chaos::Timestamp::kMicrosecondsPerSecond);
  }

  {
    Chaos::Timestamp t1(Chaos::Timestamp::invalid());
    Chaos::Timestamp t2(Chaos::Timestamp::now());
    CHAOSLOG_INFO
      << "Chaos::Timestamp unittest - @t1=" << t1.to_string()
      << "|" << t1.to_formatted_string(true);
    CHAOSLOG_INFO
      << "Chaos::Timestamp unittest - @t2=" << t2.to_string()
      << "|" << t2.to_formatted_string(true);
    CHAOS_CHECK_TRUE(t1 < t2);
    t1.swap(t2);
    CHAOSLOG_INFO
      << "Chaos::Timestamp unittest - @t1=" << t1.to_string()
      << "|" << t1.to_formatted_string(true);
    CHAOSLOG_INFO
      << "Chaos::Timestamp unittest - @t2=" << t2.to_string()
      << "|" << t2.to_formatted_string(true);
    CHAOS_CHECK_TRUE(t2 < t1);
  }

  {
    Chaos::Timestamp t1(Chaos::Timestamp::from_unix_time(std::time(nullptr)));
    Chaos::Timestamp t2(
        Chaos::Timestamp::from_unix_time(std::time(nullptr), 100));
    CHAOSLOG_INFO
      << "Chaos::Timestamp unittest - @t1=" << t1.to_string()
      << "|" << t1.to_formatted_string(true);
    CHAOSLOG_INFO
      << "Chaos::Timestamp unittest - @t2=" << t2.to_string()
      << "|" << t2.to_formatted_string(true);
    CHAOS_CHECK_TRUE(t1 < t2);
    t1.swap(t2);
    CHAOSLOG_INFO
      << "Chaos::Timestamp unittest - @t1=" << t1.to_string()
      << "|" << t1.to_formatted_string(true);
    CHAOSLOG_INFO
      << "Chaos::Timestamp unittest - @t2=" << t2.to_string()
      << "|" << t2.to_formatted_string(true);
    CHAOS_CHECK_TRUE(t2 < t1);

    CHAOSLOG_INFO
      << "Chaos::Timestamp unittest - @diff(t1-t2)="
      << Chaos::time_difference(t1, t2);
    Chaos::Timestamp t3(Chaos::time_add(t2, 0.0001));
    CHAOSLOG_INFO
      << "Chaos::Timestamp unittest - @t3=" << t3.to_string()
      << "|" << t3.to_formatted_string(true);
    CHAOS_CHECK_TRUE(t1 == t3 && t2 < t3);
  }
}
