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

#if defined(_WIN32) || defined(_WIN64) || defined(_WINDOWS_) || \
  defined(_MSC_VER) || defined(__MINGW32__) || defined(__MINGW64__)
# define CHAOS_WINDOWS
#elif defined(linux) || defined(__linux) || defined(__linux__)
# define CHAOS_LINUX
#elif defined(macintosh) || defined(__APPLE__) || defined(__MACH__)
# define CHAOS_DARWIN
#else
# error "Unknown Platform."
#endif

#if !defined(CHAOS_WINDOWS)
# define CHAOS_POSIX
#endif

// https://docs.microsoft.com/en-us/cpp/visual-cpp-language-conformance
// https://gcc.gnu.org/projects/cxx-status.html
// https://clang.llvm.org/cxx_status.html
#if defined(_MSC_VER)
# define CHAOS_CC_VER         _MSC_VER
# define CHAOS_CC_VER_LIMIT   1910
#elif defined(__clang__)
# define CHAOS_CC_VER         \
  (__clang_major__ * 10000 + __clang_minor__ * 100 + __clang_patchlevel__)
# if defined(CHAOS_DARWIN)
#   define CHAOS_CC_VER_LIMIT 90000
# else
#   define CHAOS_CC_VER_LIMIT 50000
# endif
#elif defined(__GNUC__)
# define CHAOS_CC_VER         \
  (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)
# if defined(CHAOS_DARWIN)
#   define CHAOS_CC_VER_LIMIT 50000
# else
#   define CHAOS_CC_VER_LIMIT 70100
# endif
#else
# error "Unknown Compiler."
#endif

#if CHAOS_CC_VER < CHAOS_CC_VER_LIMIT
# error "Please use a higher version of compiler."
#endif

#if defined(i386) || defined(__i386__) || defined(__i486__) || \
  defined(__i586__) || defined(__i686__) || defined(__i386) || \
  defined(_M_IX86) || defined(_X86_) || defined(__THW_INTEL__) || \
  defined(__I86__) || defined(__INTEL__)
# define CHAOS_ARCH32
#elif defined(__x86_64) || defined(__x86_64__) || \
  defined(__amd64__) || defined(__amd64) || defined(_M_X64)
# define CHAOS_ARCH64
#else
# error "Unknown Architecture."
#endif

// fixed Wmultichar for gcc and clang
// [REF] Best practices for multi-character character constants
// [REF] http://zipcon.net/~swhite/docs/computers/languages/c_multi-char_const.html
#define CHAOS_LECHR(a, b, c, d) (((a) << 24) | ((b) << 16) | ((c) << 8) | (d))

// machine byte order checking macros
#define CHAOS_LITTLE_ENDIAN (0x41424344UL)
#define CHAOS_BIG_ENDIAN    (0x44434241UL)
#define CHAOS_PDP_ENDIAN    (0x42414443UL)
#define CHAOS_BYTE_ORDER    CHAOS_LECHR('A', 'B', 'C', 'D')

// functional implementations used C++ standard library
// defining the compiling flag in Makefile (use `Surtr` script)
// #define CHAOS_IMPL_WITH_STD (0)

// enabled using `chaos_fast_dycast` and `chaos_fast_dypcast`
// #define CHAOS_ENABLE_FAST_CAST (0)

#if defined(CHAOS_WINDOWS)
# define CHAOS_ARRAY(type, name, count) \
  type* name = (type*)_alloca((count) * sizeof(type))
#else
# define CHAOS_ARRAY(type, name, count) type name[count]
#endif

// thread local storage duration
#if defined(CHAOS_WINDOWS)
# define __chaos_tl __declspec(thread)
#else
# define __chaos_tl __thread
#endif
