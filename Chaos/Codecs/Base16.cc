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
#include <Chaos/Types.h>
#include <Chaos/Codecs/Base16.h>

namespace Chaos {

namespace Base16 {
  static const char* base16_initilize(void);

  static char kDecode16[128] = {};
  static const char* kEncode16 = base16_initilize();

  static const char* base16_initilize(void) {
    const char* s = "0123456789ABCDEF";
    for (int i = 0; i < 16; ++i)
      kDecode16[static_cast<int>(s[i])] = i;
    return s;
  }

  std::string encode(const char* s, std::size_t n) {
    std::string r;

    const byte_t* b = reinterpret_cast<const byte_t*>(s);
    for (std::size_t i = 0; i < n; ++i) {
      r.push_back(kEncode16[b[i] >> 4]);
      r.push_back(kEncode16[b[i] & 0x0F]);
    }

    return r;
  }

  std::string decode(const char* s, std::size_t n) {
    std::string r;

    const byte_t* b = reinterpret_cast<const byte_t*>(s);
    for (std::size_t i = 0; i < n; i += 2)
      r.push_back((kDecode16[b[i]] << 4) | (kDecode16[b[i + 1]] & 0x0F));

    return r;
  }
}

}
