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
#include <Windows.h>
#include <Chaos/Except/SystemError.hh>
#include <Chaos/Concurrent/Mutex.hh>
#include <Chaos/IO/ColorIO.h>

namespace Chaos::ColorIO {

static Mutex g_color_mutex;

static WORD windows_color(ColorType color) {
  switch (color) {
  case ColorType::COLORTYPE_INVALID:
    __chaos_throw_exception(std::logic_error("invalid color type"));
    break;
  case ColorType::COLORTYPE_FG_BLACK:
    return FOREGROUND_RED & FOREGROUND_GREEN & FOREGROUND_GREEN;
  case ColorType::COLORTYPE_FG_RED:
    return FOREGROUND_RED;
  case ColorType::COLORTYPE_FG_GREEN:
    return FOREGROUND_GREEN;
  case ColorType::COLORTYPE_FG_YELLOW:
    return FOREGROUND_RED | FOREGROUND_GREEN;
  case ColorType::COLORTYPE_FG_BLUE:
    return FOREGROUND_BLUE;
  case ColorType::COLORTYPE_FG_MAGENTA:
    return FOREGROUND_RED | FOREGROUND_BLUE;
  case ColorType::COLORTYPE_FG_CYAN:
    return FOREGROUND_GREEN | FOREGROUND_BLUE;
  case ColorType::COLORTYPE_FG_WHITE:
    return FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
  case ColorType::COLORTYPE_FG_GRAY:
    return FOREGROUND_INTENSITY;
  case ColorType::COLORTYPE_FG_LIGHTRED:
    return FOREGROUND_RED | FOREGROUND_INTENSITY;
  case ColorType::COLORTYPE_FG_LIGHTGREEN:
    return FOREGROUND_GREEN | FOREGROUND_INTENSITY;
  case ColorType::COLORTYPE_FG_LIGHTYELLOW:
    return FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
  case ColorType::COLORTYPE_FG_LIGHTBLUE:
    return FOREGROUND_BLUE | FOREGROUND_INTENSITY;
  case ColorType::COLORTYPE_FG_LIGHTMAGENTA:
    return FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
  case ColorType::COLORTYPE_FG_LIGHTCYAN:
    return FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
  case ColorType::COLORTYPE_FG_LIGHTWHITE:
    return FOREGROUND_RED |
      FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
  case ColorType::COLORTYPE_BG_BLACK:
    return BACKGROUND_RED & BACKGROUND_GREEN & BACKGROUND_BLUE;
  case ColorType::COLORTYPE_BG_RED:
    return BACKGROUND_RED;
  case ColorType::COLORTYPE_BG_GREEN:
    return BACKGROUND_GREEN;
  case ColorType::COLORTYPE_BG_YELLOW:
    return BACKGROUND_RED | BACKGROUND_GREEN;
  case ColorType::COLORTYPE_BG_BLUE:
    return BACKGROUND_BLUE;
  case ColorType::COLORTYPE_BG_MAGENTA:
    return BACKGROUND_RED | BACKGROUND_BLUE;
  case ColorType::COLORTYPE_BG_CYAN:
    return BACKGROUND_GREEN | BACKGROUND_BLUE;
  case ColorType::COLORTYPE_BG_WHITE:
    return BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE;
  case ColorType::COLORTYPE_BG_GRAY:
    return BACKGROUND_INTENSITY;
  case ColorType::COLORTYPE_BG_LIGHTRED:
    return BACKGROUND_RED | BACKGROUND_INTENSITY;
  case ColorType::COLORTYPE_BG_LIGHTGREEN:
    return BACKGROUND_GREEN | BACKGROUND_INTENSITY;
  case ColorType::COLORTYPE_BG_LIGHTYELLOW:
    return BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_INTENSITY;
  case ColorType::COLORTYPE_BG_LIGHTBLUE:
    return BACKGROUND_BLUE | BACKGROUND_INTENSITY;
  case ColorType::COLORTYPE_BG_LIGHTMAGENTA:
    return BACKGROUND_RED | BACKGROUND_BLUE | BACKGROUND_INTENSITY;
  case ColorType::COLORTYPE_BG_LIGHTCYAN:
    return BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_INTENSITY;
  case ColorType::COLORTYPE_BG_LIGHTWHITE:
    return BACKGROUND_RED |
      BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_INTENSITY;
  case ColorType::COLORTYPE_COUNTER:
    break;
  }
  return 0;
}

static int vfprintf_impl(
    std::FILE* stream, WORD color, const char* format, std::va_list ap) {
  HANDLE out_handle = GetStdHandle(STD_OUTPUT_HANDLE);
  CONSOLE_SCREEN_BUFFER_INFO info;
  GetConsoleScreenBufferInfo(out_handle, &info);
  WORD old_color = info.wAttributes;

  int n{};
  {
    ScopedLock<Mutex> guard(g_color_mutex);
    SetConsoleTextAttribute(out_handle, color);
    n = std::vfprintf(stream, format, ap);
    SetConsoleTextAttribute(out_handle, old_color);
  }

  return n;
}

int vfprintf(
    std::FILE* stream, ColorType color, const char* format, std::va_list ap) {
  return vfprintf_impl(stream, windows_color(color), format, ap);
}

int vfprintf(std::FILE* stream,
    ColorType fcolor, ColorType bcolor, const char* format, std::va_list ap) {
  auto color = windows_color(fcolor) | windows_color(bcolor);
  return vfprintf_impl(stream, color, format, ap);
}

static inline std::ostream& set_colorful(
    std::ostream& stream, ColorType fcolor, ColorType bcolor) noexcept {
  static WORD wDefaultAttributes = 0;

  HANDLE hTerminal = INVALID_HANDLE_VALUE;
  if (&stream == &std::cout)
    hTerminal = ::GetStdHandle(STD_OUTPUT_HANDLE);
  else if (&stream == &std::cerr)
    hTerminal = ::GetStdHandle(STD_ERROR_HANDLE);

  if (!wDefaultAttributes) {
    CONSOLE_SCREEN_BUFFER_INFO info;
    if (!::GetConsoleScreenBufferInfo(hTerminal, &info))
      return stream;
    wDefaultAttributes = info.wAttributes;
  }

  WORD wAttributes;
  if (fcolor == ColorType::COLORTYPE_RESET && bcolor == ColorType::COLORTYPE_RESET) {
    wAttributes = wDefaultAttributes;
  }
  else {
    CONSOLE_SCREEN_BUFFER_INFO info;
    if (!::GetConsoleScreenBufferInfo(hTerminal, &info))
      return stream;

    wAttributes = info.wAttributes;
    if (fcolor != ColorType::COLORTYPE_RESET) {
      wAttributes &= ~(info.wAttributes & 0x0F);
      wAttributes |= windows_color(fcolor);
    }
    if (bcolor != ColorType::COLORTYPE_RESET) {
      wAttributes &= ~(info.wAttributes & 0xF0);
      wAttributes |= windows_color(bcolor);
    }
  }

  {
    ScopedLock<Mutex> guard(g_color_mutex);
    SetConsoleTextAttribute(hTerminal, wAttributes);
  }
  return stream;
}

std::ostream& set_colorful(std::ostream& stream, ColorType color) noexcept {
  return set_colorful(stream, color, ColorType::COLORTYPE_RESET);
}

std::ostream& set_foreground_colorful(std::ostream& stream, ColorType color) noexcept {
  return set_colorful(stream, color, ColorType::COLORTYPE_RESET);
}

std::ostream& set_background_colorful(std::ostream& stream, ColorType color) noexcept {
  return set_colorful(stream, ColorType::COLORTYPE_RESET, color);
}

}
