// Copyright (c) 2016 ASMlover. All rights reserved.
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
#pragma once

#include <cstdarg>
#include <cstdio>

namespace Chaos::ColorIO {

enum class ColorType {
  COLORTYPE_INVALID = -1,

  COLORTYPE_FG_BLACK,
  COLORTYPE_FG_RED,
  COLORTYPE_FG_GREEN,
  COLORTYPE_FG_YELLOW,
  COLORTYPE_FG_BLUE,
  COLORTYPE_FG_MAGENTA,
  COLORTYPE_FG_CYAN,
  COLORTYPE_FG_WHITE,
  COLORTYPE_FG_GRAY,
  COLORTYPE_FG_LIGHTRED,
  COLORTYPE_FG_LIGHTGREEN,
  COLORTYPE_FG_LIGHTYELLOW,
  COLORTYPE_FG_LIGHTBLUE,
  COLORTYPE_FG_LIGHTMAGENTA,
  COLORTYPE_FG_LIGHTCYAN,
  COLORTYPE_FG_LIGHTWHITE,

  COLORTYPE_BG_BLACK,
  COLORTYPE_BG_RED,
  COLORTYPE_BG_GREEN,
  COLORTYPE_BG_YELLOW,
  COLORTYPE_BG_BLUE,
  COLORTYPE_BG_MAGENTA,
  COLORTYPE_BG_CYAN,
  COLORTYPE_BG_WHITE,
  COLORTYPE_BG_GRAY,
  COLORTYPE_BG_LIGHTRED,
  COLORTYPE_BG_LIGHTGREEN,
  COLORTYPE_BG_LIGHTYELLOW,
  COLORTYPE_BG_LIGHTBLUE,
  COLORTYPE_BG_LIGHTMAGENTA,
  COLORTYPE_BG_LIGHTCYAN,
  COLORTYPE_BG_LIGHTWHITE,

  COLORTYPE_COUNTER,
};

const char* color_as_string(ColorType color);

int vfprintf(
    std::FILE* stream, ColorType color, const char* format, std::va_list ap);
int vfprintf(std::FILE* stream,
    ColorType fcolor, ColorType bcolor, const char* format, std::va_list ap);

int printf(ColorType color, const char* format, ...);
int printf(ColorType fcolor, ColorType bcolor, const char* format, ...);
int fprintf(std::FILE* stream, ColorType color, const char* format, ...);
int fprintf(std::FILE* stream,
    ColorType fcolor, ColorType bcolor, const char* format, ...);

}
