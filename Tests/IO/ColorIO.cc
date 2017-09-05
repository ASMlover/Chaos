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
#include <Chaos/IO/ColorIO.h>
#include <Chaos/Unittest/TestHarness.h>

CHAOS_TEST(ColorIO, Chaos::FakeTester) {
  namespace cc = ::Chaos::ColorIO;

  cc::printf(cc::ColorType::COLORTYPE_FG_RED,
      "Chaos::ColorIO unittest - red information with `printf`\n");
  cc::printf(cc::ColorType::COLORTYPE_FG_LIGHTRED,
      "Chaos::ColorIO unittest - light red information with `printf`\n");
  cc::printf(cc::ColorType::COLORTYPE_FG_GREEN,
      "Chaos::ColorIO unittest - green information with `printf`\n");
  cc::printf(cc::ColorType::COLORTYPE_FG_LIGHTGREEN,
      "Chaos::ColorIO unittest - light green information with `printf`\n");
  cc::printf(cc::ColorType::COLORTYPE_FG_YELLOW,
      "Chaos::ColorIO unittest - yellow information with `printf`\n");
  cc::printf(cc::ColorType::COLORTYPE_FG_LIGHTYELLOW,
      "Chaos::ColorIO unittest - light yellow information with `printf`\n");
  cc::printf(cc::ColorType::COLORTYPE_FG_BLUE,
      "Chaos::ColorIO unittest - blue information with `printf`\n");
  cc::printf(cc::ColorType::COLORTYPE_FG_LIGHTBLUE,
      "Chaos::ColorIO unittest - light blue information with `printf`\n");
  cc::printf(cc::ColorType::COLORTYPE_FG_MAGENTA,
      "Chaos::ColorIO unittest - magenta information with `printf`\n");
  cc::printf(cc::ColorType::COLORTYPE_FG_LIGHTMAGENTA,
      "Chaos::ColorIO unittest - light magenta information with `printf`\n");
  cc::printf(cc::ColorType::COLORTYPE_FG_CYAN,
      "Chaos::ColorIO unittest - cyan information with `printf`\n");
  cc::printf(cc::ColorType::COLORTYPE_FG_LIGHTCYAN,
      "Chaos::ColorIO unittest - light cyan information with `printf`\n");
  cc::printf(cc::ColorType::COLORTYPE_FG_WHITE,
      "Chaos::ColorIO unittest - white information with `printf`\n");
  cc::printf(cc::ColorType::COLORTYPE_FG_LIGHTWHITE,
      "Chaos::ColorIO unittest - light white information with `printf`\n");
  cc::printf(cc::ColorType::COLORTYPE_FG_GRAY,
      "Chaos::ColorIO unittest - gray information with `printf`\n");

  cc::fprintf(stdout, cc::ColorType::COLORTYPE_FG_RED,
      "Chaos::ColorIO unittest - red information with `fprintf`\n");
  cc::fprintf(stdout, cc::ColorType::COLORTYPE_FG_LIGHTRED,
      "Chaos::ColorIO unittest - light red information with `fprintf`\n");
  cc::fprintf(stdout, cc::ColorType::COLORTYPE_FG_GREEN,
      "Chaos::ColorIO unittest - green information with `fprintf`\n");
  cc::fprintf(stdout, cc::ColorType::COLORTYPE_FG_LIGHTGREEN,
      "Chaos::ColorIO unittest - light green information with `fprintf`\n");
  cc::fprintf(stdout, cc::ColorType::COLORTYPE_FG_YELLOW,
      "Chaos::ColorIO unittest - yellow information with `fprintf`\n");
  cc::fprintf(stdout, cc::ColorType::COLORTYPE_FG_LIGHTYELLOW,
      "Chaos::ColorIO unittest - light yellow information with `fprintf`\n");
  cc::fprintf(stdout, cc::ColorType::COLORTYPE_FG_BLUE,
      "Chaos::ColorIO unittest - blue information with `fprintf`\n");
  cc::fprintf(stdout, cc::ColorType::COLORTYPE_FG_LIGHTBLUE,
      "Chaos::ColorIO unittest - light blue information with `fprintf`\n");
  cc::fprintf(stdout, cc::ColorType::COLORTYPE_FG_MAGENTA,
      "Chaos::ColorIO unittest - magenta information with `fprintf`\n");
  cc::fprintf(stdout, cc::ColorType::COLORTYPE_FG_LIGHTMAGENTA,
      "Chaos::ColorIO unittest - light magenta information with `fprintf`\n");
  cc::fprintf(stdout, cc::ColorType::COLORTYPE_FG_CYAN,
      "Chaos::ColorIO unittest - cyan information with `fprintf`\n");
  cc::fprintf(stdout, cc::ColorType::COLORTYPE_FG_LIGHTCYAN,
      "Chaos::ColorIO unittest - light cyan information with `fprintf`\n");
  cc::fprintf(stdout, cc::ColorType::COLORTYPE_FG_WHITE,
      "Chaos::ColorIO unittest - white information with `fprintf`\n");
  cc::fprintf(stdout, cc::ColorType::COLORTYPE_FG_LIGHTWHITE,
      "Chaos::ColorIO unittest - light white information with `fprintf`\n");
  cc::fprintf(stdout, cc::ColorType::COLORTYPE_FG_GRAY,
      "Chaos::ColorIO unittest - gray information with `fprintf`\n");
}
