// Copyright (c) 2016 ASMlover. All rights reserved.
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
#include <Chaos/IO/ColorIO.hh>

namespace Chaos::ColorIO {

const char* color_as_string(ColorType color) {
  switch (color) {
  case ColorType::COLORTYPE_RESET: return "COLORTYPE_RESET";
  case ColorType::COLORTYPE_FG_BLACK: return "FOREGROUND_BLACK";
  case ColorType::COLORTYPE_FG_RED: return "FOREGROUND_RED";
  case ColorType::COLORTYPE_FG_GREEN: return "FOREGROUND_GREEN";
  case ColorType::COLORTYPE_FG_YELLOW: return "FOREGROUND_YELLOW";
  case ColorType::COLORTYPE_FG_BLUE: return "FOREGROUND_BLUE";
  case ColorType::COLORTYPE_FG_MAGENTA: return "FOREGROUND_MAGENTA";
  case ColorType::COLORTYPE_FG_CYAN: return "FOREGROUND_CYAN";
  case ColorType::COLORTYPE_FG_WHITE: return "FOREGROUND_WHITE";
  case ColorType::COLORTYPE_FG_GRAY: return "FOREGROUND_GRAY";
  case ColorType::COLORTYPE_FG_LIGHTRED: return "FOREGROUND_LIGHTRED";
  case ColorType::COLORTYPE_FG_LIGHTGREEN: return "FOREGROUND_LIGHTGREEN";
  case ColorType::COLORTYPE_FG_LIGHTYELLOW: return "FOREGROUND_LIGHTYELLOW";
  case ColorType::COLORTYPE_FG_LIGHTBLUE: return "FOREGROUND_LIGHTBLUE";
  case ColorType::COLORTYPE_FG_LIGHTMAGENTA: return "FOREGROUND_LIGHTMAGENTA";
  case ColorType::COLORTYPE_FG_LIGHTCYAN: return "FOREGROUND_LIGHTCYAN";
  case ColorType::COLORTYPE_FG_LIGHTWHITE: return "FOREGROUND_LIGHTWHITE";
  case ColorType::COLORTYPE_BG_BLACK: return "BACKGROUND_BLACK";
  case ColorType::COLORTYPE_BG_RED: return "BACKGROUND_RED";
  case ColorType::COLORTYPE_BG_GREEN: return "BACKGROUND_GREEN";
  case ColorType::COLORTYPE_BG_YELLOW: return "BACKGROUND_YELLOW";
  case ColorType::COLORTYPE_BG_BLUE: return "BACKGROUND_BLUE";
  case ColorType::COLORTYPE_BG_MAGENTA: return "BACKGROUND_MAGENTA";
  case ColorType::COLORTYPE_BG_CYAN: return "BACKGROUND_CYAN";
  case ColorType::COLORTYPE_BG_WHITE: return "BACKGROUND_WHITE";
  case ColorType::COLORTYPE_BG_GRAY: return "BACKGROUND_GRAY";
  case ColorType::COLORTYPE_BG_LIGHTRED: return "BACKGROUND_LIGHTRED";
  case ColorType::COLORTYPE_BG_LIGHTGREEN: return "BACKGROUND_LIGHTGREEN";
  case ColorType::COLORTYPE_BG_LIGHTYELLOW: return "BACKGROUND_LIGHTYELLOW";
  case ColorType::COLORTYPE_BG_LIGHTBLUE: return "BACKGROUND_LIGHTBLUE";
  case ColorType::COLORTYPE_BG_LIGHTMAGENTA: return "BACKGROUND_LIGHTMAGENTA";
  case ColorType::COLORTYPE_BG_LIGHTCYAN: return "BACKGROUND_LIGHTCYAN";
  case ColorType::COLORTYPE_BG_LIGHTWHITE: return "BACKGROUND_LIGHTWHITE";
  case ColorType::COLORTYPE_INVALID: return "COLORTYPE_INVALID";
  case ColorType::COLORTYPE_COUNTER: break;
  }
  return "COLORTYPE_UNKNOWN";
}

int printf(ColorType color, const char* format, ...) {
  std::va_list ap;

  va_start(ap, format);
  int n = ColorIO::vfprintf(stdout, color, format, ap);
  va_end(ap);

  return n;
}

int printf(ColorType fcolor, ColorType bcolor, const char* format, ...) {
  std::va_list ap;

  va_start(ap, format);
  int n = ColorIO::vfprintf(stdout, fcolor, bcolor, format, ap);
  va_end(ap);

  return n;
}

int fprintf(std::FILE* stream, ColorType color, const char* format, ...) {
  std::va_list ap;

  va_start(ap, format);
  int n = ColorIO::vfprintf(stream, color, format, ap);
  va_end(ap);

  return n;
}

int fprintf(std::FILE* stream,
    ColorType fcolor, ColorType bcolor, const char* format, ...) {
  std::va_list ap;

  va_start(ap, format);
  int n = ColorIO::vfprintf(stream, fcolor, bcolor, format, ap);
  va_end(ap);

  return n;
}

}
