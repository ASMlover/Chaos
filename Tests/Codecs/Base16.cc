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
#include <Chaos/Codecs/Base16.h>
#include <Chaos/Unittest/TestHarness.h>

CHAOS_TEST(Base16, Chaos::FakeTester) {
  std::string s("Chaos::Base16");

  std::string ens = Chaos::Base16::encode(s.data(), s.size());
  std::cout << "Chaos::Base16 unittest encode`" << s << "` is: " << ens << std::endl;
  std::string des = Chaos::Base16::decode(ens.data(), ens.size());
  std::cout << "Chaos::Base16 unittest decode`" << ens << "` is: " << des << std::endl;
  CHAOS_CHECK_EQ(s, des);

  ens = Chaos::Base16::encode(s.data());
  std::cout << "Chaos::Base16 unittest encode`" << s << "` is: " << ens << std::endl;
  des = Chaos::Base16::decode(ens.data());
  std::cout << "Chaos::Base16 unittest decode`" << ens << "` is: " << des << std::endl;
  CHAOS_CHECK_EQ(s, des);

  ens = Chaos::Base16::encode(s);
  std::cout << "Chaos::Base16 unittest encode`" << s << "` is: " << ens << std::endl;
  des = Chaos::Base16::decode(ens);
  std::cout << "Chaos::Base16 unittest encode`" << ens << "` is: " << des << std::endl;
  CHAOS_CHECK_EQ(s, des);
}
