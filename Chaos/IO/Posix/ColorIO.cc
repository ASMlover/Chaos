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
#include <Chaos/Except/SystemError.h>
#include <Chaos/Concurrent/Mutex.h>
#include <Chaos/IO/ColorIO.h>

namespace Chaos::ColorIO {

static Mutex g_color_mutex;

static const char* posix_color(ColorType color) {
  switch (color) {
  case ColorType::COLORTYPE_INVALID:
    __chaos_throw_exception(std::logic_error("invalid color type"));
    break;
  case ColorType::COLORTYPE_FG_BLACK:
    return "\033[30m";
  case ColorType::COLORTYPE_FG_RED:
    return "\033[31m";
  case ColorType::COLORTYPE_FG_GREEN:
    return "\033[32m";
  case ColorType::COLORTYPE_FG_YELLOW:
    return "\033[33m";
  case ColorType::COLORTYPE_FG_BLUE:
    return "\033[34m";
  case ColorType::COLORTYPE_FG_MAGENTA:
    return "\033[35m";
  case ColorType::COLORTYPE_FG_CYAN:
    return "\033[36m";
  case ColorType::COLORTYPE_FG_WHITE:
    return "\033[37m";
  case ColorType::COLORTYPE_FG_GRAY:
    return "\033[90m";
  case ColorType::COLORTYPE_FG_LIGHTRED:
    return "\033[91m";
  case ColorType::COLORTYPE_FG_LIGHTGREEN:
    return "\033[92m";
  case ColorType::COLORTYPE_FG_LIGHTYELLOW:
    return "\033[93m";
  case ColorType::COLORTYPE_FG_LIGHTBLUE:
    return "\033[94m";
  case ColorType::COLORTYPE_FG_LIGHTMAGENTA:
    return "\033[95m";
  case ColorType::COLORTYPE_FG_LIGHTCYAN:
    return "\033[96m";
  case ColorType::COLORTYPE_FG_LIGHTWHITE:
    return "\033[97m";
  case ColorType::COLORTYPE_BG_BLACK:
    return "\033[40m";
  case ColorType::COLORTYPE_BG_RED:
    return "\033[41m";
  case ColorType::COLORTYPE_BG_GREEN:
    return "\033[42m";
  case ColorType::COLORTYPE_BG_YELLOW:
    return "\033[43m";
  case ColorType::COLORTYPE_BG_BLUE:
    return "\033[44m";
  case ColorType::COLORTYPE_BG_MAGENTA:
    return "\033[45m";
  case ColorType::COLORTYPE_BG_CYAN:
    return "\033[46m";
  case ColorType::COLORTYPE_BG_WHITE:
    return "\033[47m";
  case ColorType::COLORTYPE_BG_GRAY:
    return "\033[100m";
  case ColorType::COLORTYPE_BG_LIGHTRED:
    return "\033[101m";
  case ColorType::COLORTYPE_BG_LIGHTGREEN:
    return "\033[102m";
  case ColorType::COLORTYPE_BG_LIGHTYELLOW:
    return "\033[103m";
  case ColorType::COLORTYPE_BG_LIGHTBLUE:
    return "\033[104m";
  case ColorType::COLORTYPE_BG_LIGHTMAGENTA:
    return "\033[105m";
  case ColorType::COLORTYPE_BG_LIGHTCYAN:
    return "\033[106m";
  case ColorType::COLORTYPE_BG_LIGHTWHITE:
    return "\033[107m";
  case ColorType::COLORTYPE_COUNTER:
    break;
  }
  return "\033[0m";
}

static int vfprintf_impl(std::FILE* stream,
    const char* color, const char* format, std::va_list ap) {
  ScopedLock<Mutex> guard(g_color_mutex);
  std::fprintf(stream, "%s", color);
  auto n = std::vfprintf(stream, format, ap);
  std::fprintf(stream, "\033[0m");
  return n;
}

int vfprintf(
    std::FILE* stream, ColorType color, const char* format, std::va_list ap) {
  return vfprintf_impl(stream, posix_color(color), format, ap);
}

int vfprintf(std::FILE* stream,
    ColorType fcolor, ColorType bcolor, const char* format, std::va_list ap) {
  char color[64];
  std::snprintf(color,
      sizeof(color), "%s%s", posix_color(fcolor), posix_color(bcolor));
  return vfprintf_impl(stream, color, format, ap);
}

}
