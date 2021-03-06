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
#pragma once

#include <cstring>
#include <string>
#include <Chaos/Base/Types.hh>

namespace Chaos {

class StringArg {
  const char* str_;
public:
  StringArg(const char* s)
    : str_(s) {
  }

  StringArg(const std::string& s)
    : str_(s.c_str()) {
  }

  const char* c_str(void) const {
    return str_;
  }
};

class StringPiece {
  const char* str_{};
  std::size_t size_{};
public:
  StringPiece(void) = default;
  ~StringPiece(void) = default;

  StringPiece(const char* s)
    : str_(s)
    , size_(std::strlen(str_)) {
  }

  StringPiece(const byte_t* s)
    : str_(reinterpret_cast<const char*>(s))
    , size_(std::strlen(str_)) {
  }

  StringPiece(const std::string& s)
    : str_(s.data())
    , size_(s.size()) {
  }

  StringPiece(const char* s, std::size_t n)
    : str_(s)
    , size_(n) {
  }

  StringPiece(const void* s, std::size_t n)
    : str_(reinterpret_cast<const char*>(s))
    , size_(n) {
  }

  explicit operator bool(void) const {
    return nullptr != str_;
  }

  const char* data(void) const {
    return str_;
  }

  char operator[](std::size_t i) const {
    return str_[i];
  }

  std::size_t size(void) const {
    return size_;
  }

  bool empty(void) const {
    return 0 == size_;
  }

  const char* begin(void) const {
    return str_;
  }

  const char* end(void) const {
    return str_ + size_;
  }

  void clear(void) {
    str_ = nullptr;
    size_ = 0;
  }

  void set(const char* s) {
    str_ = s;
    size_ = std::strlen(str_);
  }

  void set(const char* s, std::size_t n) {
    str_ = s;
    size_ = n;
  }

  void set(const void* buf, std::size_t len) {
    str_ = reinterpret_cast<const char*>(buf);
    size_ = len;
  }

  void remove_prefix(std::size_t n) {
    str_ += n;
    size_ -= n;
  }

  void remove_suffix(std::size_t n) {
    size_ -= n;
  }

  std::string to_string(void) const {
    return std::string(str_, size_);
  }

  void copy_to_string(std::string* x) const {
    x->assign(str_, size_);
  }

  bool starts_with(const StringPiece& r) const {
    return ((size_ >= r.size_) && (0 == std::memcmp(str_, r.str_, r.size_)));
  }

  int compare(const StringPiece& r) const {
    int c = std::memcmp(str_, r.str_, chaos_min(size_, r.size_));
    if (0 == c) {
      if (size_ < r.size_)
        c = -1;
      else if (size_ > r.size_)
        c = 1;
    }
    return c;
  }

  bool operator==(const StringPiece& r) const {
    return ((size_ == r.size_) && (0 == std::memcmp(str_, r.str_, size_)));
  }

  bool operator<(const StringPiece& r) const {
    int c = std::memcmp(str_, r.str_, chaos_min(size_, r.size_));
    return ((c < 0) || ((0 == c) && (size_ < r.size_)));
  }

  bool operator<=(const StringPiece& r) const {
    int c = std::memcmp(str_, r.str_, chaos_min(size_, r.size_));
    return ((c < 0) || ((0 == c) && (size_ <= r.size_)));
  }

  bool operator>(const StringPiece& r) const {
    int c = std::memcmp(str_, r.str_, chaos_min(size_, r.size_));
    return ((c > 0) || ((0 == c) && (size_ > r.size_)));
  }

  bool operator>=(const StringPiece& r) const {
    int c = std::memcmp(str_, r.str_, chaos_min(size_, r.size_));
    return ((c > 0) || ((0 == c) && (size_ >= r.size_)));
  }
};

std::ostream& operator<<(std::ostream& out, const StringPiece& piece);

}
