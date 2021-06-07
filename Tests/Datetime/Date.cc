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
#include <Chaos/Datetime/Date.hh>
#include <Chaos/Logging/Logging.hh>
#include <Chaos/Unittest/TestHarness.h>

CHAOS_TEST(Date, Chaos::FakeTester) {
  {
    Chaos::Date d;
    CHAOSLOG_INFO
      << "Chaos::Data unittest - " << d.to_iso_string()
      << ", @epoch_day=" << d.epoch_day();
    CHAOS_CHECK_TRUE(!d.is_valid());
    CHAOS_CHECK_TRUE(d.epoch_day() == 0);
  }

  {
    Chaos::Date d(2017, 2, 24);
    CHAOSLOG_INFO
      << "Chaos::Date unittest - " << d.to_iso_string()
      << ", @epoch_day=" << d.epoch_day();
    CHAOS_CHECK_TRUE(d.is_valid());
    CHAOS_CHECK_TRUE(d.epoch_day() == 2457809);
    CHAOS_CHECK_TRUE(d.year() == 2017);
    CHAOS_CHECK_TRUE(d.month() == 2);
    CHAOS_CHECK_TRUE(d.day() == 24);
    CHAOS_CHECK_TRUE(d.weekday() == 5);
  }

  {
    Chaos::Date d0(1990, 1, 1);
    CHAOSLOG_INFO
      << "Chaos::Date unittest - @d0=" << d0.to_iso_string()
      << ", @epoch_day=" << d0.epoch_day();
    std::time_t now(std::time(nullptr));
    std::tm* tm_now = std::localtime(&now);
    Chaos::Date d1(tm_now->tm_year + 1900, tm_now->tm_mon + 1, tm_now->tm_mday);
    CHAOSLOG_INFO
      << "Chaos::Date unittest - @d1=" << d1.to_iso_string()
      << ", @epoch_day=" << d1.epoch_day();
    Chaos::Date d2(*tm_now);
    CHAOSLOG_INFO
      << "Chaos::Date unittest - @d2=" << d1.to_iso_string()
      << ", @epoch_day=" << d1.epoch_day();

    CHAOS_CHECK_TRUE(d1 == d2);
    CHAOS_CHECK_TRUE(d0 < d1);
    CHAOS_CHECK_TRUE(d0 < d2);

    d0.swap(d2);
    CHAOS_CHECK_TRUE(d0.epoch_day() == d1.epoch_day()
        && d2.epoch_day() == 2447893);
    CHAOS_CHECK_TRUE(d0.year() == tm_now->tm_year + 1900 && d2.year() == 1990);
    CHAOS_CHECK_TRUE(d0.month() == tm_now->tm_mon + 1 && d2.month() == 1);
    CHAOS_CHECK_TRUE(d0.day() == tm_now->tm_mday && d2.day() == 1);
    CHAOS_CHECK_TRUE(d0.weekday() == tm_now->tm_wday && d2.weekday() == 1);
    CHAOS_CHECK_TRUE(d0 == d1);
    CHAOS_CHECK_TRUE(d2 < d0);
    CHAOS_CHECK_TRUE(d2 < d1);
  }
}
