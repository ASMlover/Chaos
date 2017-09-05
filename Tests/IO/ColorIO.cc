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

  static const char* color_names[] = {
    "ColorType::COLORTYPE_FG_BLACK",
    "ColorType::COLORTYPE_FG_RED",
    "ColorType::COLORTYPE_FG_GREEN",
    "ColorType::COLORTYPE_FG_YELLOW",
    "ColorType::COLORTYPE_FG_BLUE",
    "ColorType::COLORTYPE_FG_MAGENTA",
    "ColorType::COLORTYPE_FG_CYAN",
    "ColorType::COLORTYPE_FG_WHITE",
    "ColorType::COLORTYPE_FG_GRAY",
    "ColorType::COLORTYPE_FG_LIGHTRED",
    "ColorType::COLORTYPE_FG_LIGHTGREEN",
    "ColorType::COLORTYPE_FG_LIGHTYELLOW",
    "ColorType::COLORTYPE_FG_LIGHTBLUE",
    "ColorType::COLORTYPE_FG_LIGHTMAGENTA",
    "ColorType::COLORTYPE_FG_LIGHTCYAN",
    "ColorType::COLORTYPE_FG_LIGHTWHITE",
    "ColorType::COLORTYPE_BG_BLACK",
    "ColorType::COLORTYPE_BG_RED",
    "ColorType::COLORTYPE_BG_GREEN",
    "ColorType::COLORTYPE_BG_YELLOW",
    "ColorType::COLORTYPE_BG_BLUE",
    "ColorType::COLORTYPE_BG_MAGENTA",
    "ColorType::COLORTYPE_BG_CYAN",
    "ColorType::COLORTYPE_BG_WHITE",
    "ColorType::COLORTYPE_BG_GRAY",
    "ColorType::COLORTYPE_BG_LIGHTRED",
    "ColorType::COLORTYPE_BG_LIGHTGREEN",
    "ColorType::COLORTYPE_BG_LIGHTYELLOW",
    "ColorType::COLORTYPE_BG_LIGHTBLUE",
    "ColorType::COLORTYPE_BG_LIGHTMAGENTA",
    "ColorType::COLORTYPE_BG_LIGHTCYAN",
    "ColorType::COLORTYPE_BG_LIGHTWHITE",
  };

  // unittest for single color
  auto color_count = static_cast<int>(cc::ColorType::COLORTYPE_COUNTER);
  for (int i = 0; i < color_count; ++i) {
    auto c = static_cast<cc::ColorType>(i);
    if (c == cc::ColorType::COLORTYPE_FG_BLACK
        || c == cc::ColorType::COLORTYPE_BG_BLACK) {
      auto bc = cc::ColorType::COLORTYPE_BG_GRAY;
      if (c == cc::ColorType::COLORTYPE_BG_BLACK) {
        bc = c;
        c = cc::ColorType::COLORTYPE_FG_GRAY;
      }

      cc::printf(
          c, bc, "Chaos::ColorIO unittest - `printf` %s", color_names[i]);
      std::printf("\n");
      cc::fprintf(stdout,
          c, bc, "Chaos::ColorIO unittest - `fprintf` %s", color_names[i]);
      std::printf("\n");
    }
    else {
      cc::printf(c, "Chaos::ColorIO unittest - `printf` %s", color_names[i]);
      std::printf("\n");
      cc::fprintf(stdout,
          c, "Chaos::ColorIO unittest - `fprintf` %s", color_names[i]);
      std::printf("\n");
    }
  }

  // unittest for both foreground color and background color
  color_count = static_cast<int>(cc::ColorType::COLORTYPE_FG_LIGHTWHITE) + 1;
  auto bc_beg = static_cast<int>(cc::ColorType::COLORTYPE_BG_BLACK);
  for (auto i = 0; i < color_count; ++i) {
    for (auto j = 0; j < color_count; ++j) {
      if (j == i)
        continue;

      auto fc = static_cast<cc::ColorType>(i);
      auto bc = static_cast<cc::ColorType>(bc_beg + j);
      cc::printf(fc, bc, "Chaos::ColorIO unittest - `printf` {%s, %s}",
          color_names[i], color_names[bc_beg + j]);
      std::printf("\n");
      cc::printf(fc, bc, "Chaos::ColorIO unittest - `fprintf` {%s, %s}",
          color_names[i], color_names[bc_beg + j]);
      std::printf("\n");
    }
  }
}
