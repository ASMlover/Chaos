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
#include <Chaos/Base/Types.h>
#include <Chaos/Codecs/Base32.h>

namespace Chaos::Base32 {

static const char* base32_initilize(void);

static char kDecode32[128] = {};
static const char* kEncode32 = base32_initilize();

static const char* base32_initilize(void) {
  const char* s = "ABCDEFGHIJKLMNOPQRSTUVWXYZ234567";
  for (int i = 0; i < 32; ++i)
    kDecode32[static_cast<int>(s[i])] = i;
  return s;
}

std::string encode(const char* s, std::size_t n) {
  std::string r;

  const byte_t* b = reinterpret_cast<const byte_t*>(s);
  for (std::size_t i = 0; i < n; i += 5) {
    r.push_back(kEncode32[b[i] >> 3]);

    if (i + 1 < n) {
      r.push_back(kEncode32[((b[i] & 0x07) << 2) | (b[i + 1] >> 6)]);
    }
    else {
      r.push_back(kEncode32[(b[i] & 0x07) << 2]);
      r.append("======");
      break;
    }

    if (i + 2 < n) {
      r.push_back(kEncode32[(b[i + 1] & 0x3f) >> 1]);
      r.push_back(kEncode32[((b[i + 1] & 0x01) << 4) | (b[i + 2] >> 4)]);
    }
    else {
      r.push_back(kEncode32[(b[i + 1] & 0x3f) >> 1]);
      r.push_back(kEncode32[(b[i + 1] & 0x01) << 4]);
      r.append("====");
      break;
    }

    if (i + 3 < n) {
      r.push_back(kEncode32[((b[i + 2] & 0x0f) << 1) | (b[i + 3] >> 7)]);
      r.push_back(kEncode32[(b[i + 3] & 0x7f) >> 2]);
    }
    else {
      r.push_back(kEncode32[(b[i + 2] & 0x0f) << 1]);
      r.append("===");
      break;
    }

    if (i + 4 < n) {
      r.push_back(kEncode32[((b[i + 3] & 0x03) << 3) | (b[i + 4] >> 5)]);
      r.push_back(kEncode32[b[i + 4] & 0x1f]);
    }
    else {
      r.push_back(kEncode32[(b[i + 3] & 0x03) << 3]);
      r.push_back('=');
      break;
    }
  }

  return r;
}

std::string decode(const char* s, std::size_t n) {
  std::string r;

  const byte_t* b = reinterpret_cast<const byte_t*>(s);
  for (std::size_t i = 0; i < n; i += 8) {
    r.push_back((kDecode32[b[i]] << 3) | ((kDecode32[b[i + 1]] >> 2) & 0x07));
    if (b[i + 2] != '=')
      r.push_back((kDecode32[b[i + 1]] << 6)
          | (kDecode32[b[i + 2]] << 1) | ((kDecode32[b[i + 3]] >> 4) & 0x01));
    if (b[i + 4] != '=')
      r.push_back((kDecode32[b[i + 3]] << 4)
          | ((kDecode32[b[i + 4]] >> 1) & 0x0f));
    if (b[i + 5] != '=')
      r.push_back((kDecode32[b[i + 4]] << 7)
          | (kDecode32[b[i + 5]] << 2) | ((kDecode32[b[i + 6]] >> 3) & 0x03));
    if (b[i + 7] != '=')
      r.push_back((kDecode32[b[i + 6]] << 5) | (kDecode32[b[i + 7]] & 0x1f));
  }

  return r;
}

}
