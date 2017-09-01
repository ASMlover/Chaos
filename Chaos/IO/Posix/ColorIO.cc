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

namespace Chaos {

namespace ColorIO {
  static Mutex g_color_mutex;

  int vfprintf(
      std::FILE* stream, ColorType color, const char* format, std::va_list ap) {
    const char* new_color = "\033[0m";
    switch (color) {
    case ColorType::COLORTYPE_INVALID:
      __chaos_throw_exception(std::logic_error("invalid color type"));
      break;
    case ColorType::COLORTYPE_RED:
      new_color = "\033[31m";
      break;
    case ColorType::COLORTYPE_GREEN:
      new_color = "\033[32m";
      break;
    case ColorType::COLORTYPE_YELLOW:
      new_color = "\033[33m";
      break;
    case ColorType::COLORTYPE_BLUE:
      new_color = "\033[34m";
      break;
    case ColorType::COLORTYPE_MAGENTA:
      new_color = "\033[35m";
      break;
    case ColorType::COLORTYPE_CYAN:
      new_color = "\033[36m";
      break;
    case ColorType::COLORTYPE_WHITE:
      new_color = "\033[37m";
      break;
    case ColorType::COLORTYPE_LIGHTRED:
      new_color = "\033[91m";
      break;
    case ColorType::COLORTYPE_LIGHTGREEN:
      new_color = "\033[92m";
      break;
    case ColorType::COLORTYPE_LIGHTYELLOW:
      new_color = "\033[93m";
      break;
    case ColorType::COLORTYPE_LIGHTBLUE:
      new_color = "\033[94m";
      break;
    case ColorType::COLORTYPE_LIGHTMAGENTA:
      new_color = "\033[95m";
      break;
    case ColorType::COLORTYPE_LIGHTCYAN:
      new_color = "\033[96m";
      break;
    case ColorType::COLORTYPE_LIGHTWHITE:
      new_color = "\033[97m";
      break;
    }

    int n;
    {
      ScopedLock<Mutex> guard(g_color_mutex);
      fprintf(stream, "%s", new_color);
      n = std::vfprintf(stream, format, ap);
      std::fprintf(stream, "\033[0m");
    }

    return n;
  }
}

}
