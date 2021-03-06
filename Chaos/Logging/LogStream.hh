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

#include <Chaos/Base/Types.hh>
#include <Chaos/Container/StringPiece.hh>

namespace Chaos {

constexpr int kSmallBuffer = 4096;
constexpr int kLargeBuffer = 4096000; // 4906 * 1000

template <int N>
class FixedBuffer : private UnCopyable {
  using CookieCallback = void (*)(void);

  char data_[N];
  char* current_;
  CookieCallback cookie_cb_;
private:
  const char* get_tail(void) const {
    return data_ + sizeof(data_);
  }

  static void cookie_start(void);
  static void cookie_end(void);
public:
  FixedBuffer(void)
    : current_(data_) {
    set_cookie(cookie_start);
  }

  ~FixedBuffer(void) {
    set_cookie(cookie_end);
  }

  const char* data(void) const {
    return data_;
  }

  std::size_t size(void) const {
    return static_cast<std::size_t>(current_ - data_);
  }

  char* get_current(void) {
    return current_;
  }

  const char* get_current(void) const {
    return current_;
  }

  std::size_t get_avail(void) const {
    return static_cast<std::size_t>(get_tail() - current_);
  }

  void remove_prefix(std::size_t n) {
    current_ += n;
  }

  void append(const char* buf, std::size_t len) {
    if (get_avail() > len) {
      std::memcpy(current_, buf, len);
      current_ += len;
    }
  }

  void reset(void) {
    current_ = data_;
  }

  void bzero(void) {
    std::memset(data_, 0, sizeof(data_));
  }

  void set_cookie(CookieCallback cb) {
    cookie_cb_ = cb;
  }

  std::string to_string(void) const {
    return std::string(data(), size());
  }

  StringPiece to_string_piece(void) const {
    return StringPiece(data(), size());
  }

  const char* debug_string(void);
};

class LogStream : private UnCopyable {
public:
  using buffer_type = FixedBuffer<kSmallBuffer>;
private:
  buffer_type buff_;
  static const int kMaxNumericSize = 32;

  void static_check(void);
  template <typename T>
  void format_integer(T value);
public:
  void append(const char* s, std::size_t n) {
    buff_.append(s, n);
  }

  void reset_buffer(void) {
    buff_.reset();
  }

  const buffer_type& get_buffer(void) const {
    return buff_;
  }

  LogStream& operator<<(bool v);
  LogStream& operator<<(char v);
  LogStream& operator<<(short v);
  LogStream& operator<<(unsigned short v);
  LogStream& operator<<(int v);
  LogStream& operator<<(unsigned int v);
  LogStream& operator<<(long v);
  LogStream& operator<<(unsigned long v);
  LogStream& operator<<(long long v);
  LogStream& operator<<(unsigned long long v);
  LogStream& operator<<(float v);
  LogStream& operator<<(double v);
  LogStream& operator<<(const void* p);
  LogStream& operator<<(const char* s);
  LogStream& operator<<(const byte_t* s);
  LogStream& operator<<(const std::string& s);
  LogStream& operator<<(const StringPiece& piece);
  LogStream& operator<<(const buffer_type& buf);
};

class Format {
  char data_[32];
  std::size_t size_;
public:
  template <typename T>
  Format(const char* fmt, T value);

  const char* data(void) const {
    return data_;
  }

  std::size_t size(void) const {
    return size_;
  }
};

inline LogStream& operator<<(LogStream& s, const Format& fmt) {
  s.append(fmt.data(), fmt.size());
  return s;
}

}
