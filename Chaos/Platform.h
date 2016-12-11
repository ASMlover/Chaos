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
#ifndef CHAOS_PLATFORM_H
#define CHAOS_PLATFORM_H

#if defined(_WIN32) || defined(_WIN64) || defined(_WINDOWS_) || defined(_MSC_VER)
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

#if defined(_MSC_VER)
# define CHAOS_CC_VER       _MSC_VER
# define CHAOS_CC_VER_LIMIT 1900
#elif defined(__GNUC__)
# define CHAOS_CC_VER       (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)
# if defined(CHAOS_DARWIN)
#   define CHAOS_CC_VER_LIMIT 30300
# else
#   define CHAOS_CC_VER_LIMIT 40801
# endif
#elif defined(__clang__)
# define CHAOS_CC_VER       (__clang_major__ * 10000 + __clang_major__ * 100 + __clang_patchlevel__)
# if defined(CHAOS_DARWIN)
#   define CHAOS_CC_VER_LIMIT 80000
# else
#   define CHAOS_CC_VER_LIMIT 30300
# endif
#else
# error "Unknown Compiler."
#endif

#if CHAOS_CC_VER < CHAOS_CC_VER_LIMIT
# error "Please use a higher version of compiler."
#endif

#define CHAOS_LITTLE_ENDIAN 0x41424344UL
#define CHAOS_BIG_ENDIAN 0x44434241UL
#define CHAOS_PDP_ENDIAN 0x42414443UL
#define CHAOS_BYTE_ORDER 'ABCD'

#define CHAOS_IMPL_WITH_STD (0)

#if defined(CHAOS_WINDOWS)
# define CHAOS_ARRAY(type, name, count) type* name = (type*)_alloca((count) * sizeof(type))
#else
# define CHAOS_ARRAY(type, name, count) type name[count]
#endif

#endif // CHAOS_PLATFORM_H
