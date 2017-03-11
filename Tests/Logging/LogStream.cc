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
#include <Chaos/Logging/LogStream.h>
#include <Chaos/Unittest/TestHarness.h>

CHAOS_TEST(LogStream, Chaos::FakeTester) {
  Chaos::LogStream os;
  auto& buf = os.get_buffer();

  // unittest for boolean
  os.reset_buffer();
  CHAOS_CHECK_EQ(buf.to_string(), std::string(""));
  os.reset_buffer(); os << true;
  CHAOS_CHECK_EQ(buf.to_string(), std::string("1"));
  os.reset_buffer(); os << false;
  CHAOS_CHECK_EQ(buf.to_string(), std::string("0"));

  // unittest for char
  os.reset_buffer(); os << 'a';
  CHAOS_CHECK_EQ(buf.to_string(), std::string("a"));
  os.reset_buffer(); os << 'A';
  CHAOS_CHECK_EQ(buf.to_string(), std::string("A"));

  // unittest for integer
  os.reset_buffer(); os << 1;
  CHAOS_CHECK_EQ(buf.to_string(), std::string("1"));
  os.reset_buffer(); os << -5;
  CHAOS_CHECK_EQ(buf.to_string(), std::string("-5"));
  os.reset_buffer(); os << 1212121;
  CHAOS_CHECK_EQ(buf.to_string(), std::string("1212121"));

  // unittest for float
  os.reset_buffer(); os << 3.14;
  CHAOS_CHECK_EQ(buf.to_string(), std::string("3.14"));
  os.reset_buffer(); os << 0.12134;
  CHAOS_CHECK_EQ(buf.to_string(), std::string("0.12134"));

  // unittest for string
  os.reset_buffer(); os << "Chaos::LogStream";
  CHAOS_CHECK_EQ(buf.to_string(), std::string("Chaos::LogStream"));
  std::string s("Chaos::LogStream unittest");
  os.reset_buffer(); os << s;
  CHAOS_CHECK_EQ(buf.to_string(), s);
}
