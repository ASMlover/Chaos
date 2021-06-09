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
#include <Chaos/IO/ColorIO.hh>
#include <Chaos/Unittest/TestHarness.hh>

CHAOS_TEST(ColorIO, Chaos::FakeTester) {
  namespace cc = ::Chaos::ColorIO;

#define CLRP(c)\
  cc::printf((c),\
      "Chaos::ColorIO unittest - `printf` %s", cc::color_as_string((c)));\
  std::printf("\n")
#define CLRP2(c, bc)\
  cc::printf((c), (bc),\
      "Chaos::ColorIO unittest - `printf` %s", cc::color_as_string((c)));\
  std::printf("\n")
#define CLRP3(fc, bc)\
  cc::printf((fc), (bc),\
      "Chaos::ColorIO unittest - `printf` {%s : %s}",\
      cc::color_as_string((fc)), cc::color_as_string((bc)));\
  std::printf("\n")

#define CLRFP(fp, c)\
  cc::fprintf((fp), (c),\
      "Chaos::ColorIO unittest - `fprintf(" #fp ")` %s",\
      cc::color_as_string((c)));\
  std::printf("\n")
#define CLRFP2(fp, c, bc)\
  cc::fprintf((fp), (c), (bc),\
      "Chaos::ColorIO unittest - `fprintf(" #fp ")` %s",\
      cc::color_as_string((c)));\
  std::printf("\n")
#define CLRFP3(fp, fc, bc)\
  cc::fprintf((fp), (fc), (bc),\
      "Chaos::ColorIO unittest - `fprintf(" #fp ")` {%s : %s}",\
      cc::color_as_string((fc)), cc::color_as_string((bc)));\
  std::printf("\n")

#define CLROF(c)\
  std::cout << cc::Foreground::c << "Chaos::ColorIO::Foreground unittest - `" #c "`"\
    << cc::reset << std::endl
#define CLROB(c)\
  std::cout << cc::Background::c << "Chaos::ColorIO::Background unittest - `" #c "`"\
    << cc::reset << std::endl
#define CLRFB(c)\
  std::cout << cc::Foreground::c << cc::Background::black\
    << "Chaos::ColorIO::Foreground unittest - with Background::black `" #c "`"\
    << cc::reset << std::endl;\
  std::cout << cc::Foreground::c << cc::Background::red\
    << "Chaos::ColorIO::Foreground unittest - with Background::red `" #c "`"\
    << cc::reset << std::endl;\
  std::cout << cc::Foreground::c << cc::Background::green\
    << "Chaos::ColorIO::Foreground unittest - with Background::green `" #c "`"\
    << cc::reset << std::endl;\
  std::cout << cc::Foreground::c << cc::Background::yellow\
    << "Chaos::ColorIO::Foreground unittest - with Background::yellow `" #c "`"\
    << cc::reset << std::endl;\
  std::cout << cc::Foreground::c << cc::Background::blue\
    << "Chaos::ColorIO::Foreground unittest - with Background::blue `" #c "`"\
    << cc::reset << std::endl;\
  std::cout << cc::Foreground::c << cc::Background::blue\
    << "Chaos::ColorIO::Foreground unittest - with Background::blue `" #c "`"\
    << cc::reset << std::endl;\
  std::cout << cc::Foreground::c << cc::Background::magenta\
    << "Chaos::ColorIO::Foreground unittest - with Background::magenta `" #c "`"\
    << cc::reset << std::endl;\
  std::cout << cc::Foreground::c << cc::Background::cyan\
    << "Chaos::ColorIO::Foreground unittest - with Background::cyan `" #c "`"\
    << cc::reset << std::endl;\
  std::cout << cc::Foreground::c << cc::Background::white\
    << "Chaos::ColorIO::Foreground unittest - with Background::white `" #c "`"\
    << cc::reset << std::endl;\
  std::cout << cc::Foreground::c << cc::Background::gray\
    << "Chaos::ColorIO::Foreground unittest - with Background::gray `" #c "`"\
    << cc::reset << std::endl;\
  std::cout << cc::Foreground::c << cc::Background::lightred\
    << "Chaos::ColorIO::Foreground unittest - with Background::lightred `" #c "`"\
    << cc::reset << std::endl;\
  std::cout << cc::Foreground::c << cc::Background::lightgreen\
    << "Chaos::ColorIO::Foreground unittest - with Background::lightgreen `" #c "`"\
    << cc::reset << std::endl;\
  std::cout << cc::Foreground::c << cc::Background::lightyellow\
    << "Chaos::ColorIO::Foreground unittest - with Background::lightyellow `" #c "`"\
    << cc::reset << std::endl;\
  std::cout << cc::Foreground::c << cc::Background::lightblue\
    << "Chaos::ColorIO::Foreground unittest - with Background::lightblue `" #c "`"\
    << cc::reset << std::endl;\
  std::cout << cc::Foreground::c << cc::Background::lightmagenta\
    << "Chaos::ColorIO::Foreground unittest - with Background::lightmagenta `" #c "`"\
    << cc::reset << std::endl;\
  std::cout << cc::Foreground::c << cc::Background::lightcyan\
    << "Chaos::ColorIO::Foreground unittest - with Background::lightcyan `" #c "`"\
    << cc::reset << std::endl;\
  std::cout << cc::Foreground::c << cc::Background::lightwhite\
    << "Chaos::ColorIO::Foreground unittest - with Background::lightwhite `" #c "`"\
    << cc::reset << std::endl

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

      CLRP2(c, bc);
      CLRFP2(stdout, c, bc); CLRFP2(stderr, c, bc);
    }
    else {
      CLRP(c);
      CLRFP(stdout, c); CLRFP(stderr, c);
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
      CLRP3(fc, bc);
      CLRFP3(stdout, fc, bc); CLRFP3(stderr, fc, bc);
    }
  }

  // unittest for foreground color stream
  CLROF(black);
  CLROF(red);
  CLROF(green);
  CLROF(yellow);
  CLROF(blue);
  CLROF(magenta);
  CLROF(cyan);
  CLROF(white);
  CLROF(gray);
  CLROF(lightred);
  CLROF(lightgreen);
  CLROF(lightyellow);
  CLROF(lightblue);
  CLROF(lightmagenta);
  CLROF(lightcyan);
  CLROF(lightwhite);

  // unittest for background color stream
  CLROB(black);
  CLROB(red);
  CLROB(green);
  CLROB(yellow);
  CLROB(blue);
  CLROB(magenta);
  CLROB(cyan);
  CLROB(white);
  CLROB(gray);
  CLROB(lightred);
  CLROB(lightgreen);
  CLROB(lightyellow);
  CLROB(lightblue);
  CLROB(lightmagenta);
  CLROB(lightcyan);
  CLROB(lightwhite);

  // unittest for color with foreground-background
  CLRFB(black);
  CLRFB(red);
  CLRFB(green);
  CLRFB(yellow);
  CLRFB(blue);
  CLRFB(magenta);
  CLRFB(cyan);
  CLRFB(white);
  CLRFB(gray);
  CLRFB(lightred);
  CLRFB(lightgreen);
  CLRFB(lightyellow);
  CLRFB(lightblue);
  CLRFB(lightmagenta);
  CLRFB(lightcyan);
  CLRFB(lightwhite);

#undef CLROB
#undef CLROF
#undef CLRP
#undef CLRP2
#undef CLRP3
#undef CLRFP
#undef CLRFP2
#undef CLRFP3
}
