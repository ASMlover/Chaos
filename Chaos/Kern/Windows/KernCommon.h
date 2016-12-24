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
#ifndef CHAOS_KERN_WINDOWS_KERNCOMMON_H
#define CHAOS_KERN_WINDOWS_KERNCOMMON_H

#include <Windows.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <io.h>
#include <errno.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include <string>

typedef int pid_t;
typedef uint16_t mode_t;
typedef SSIZE_T ssize_t;

struct timezone {
  int tz_minuteswest;
  int tz_dsttime;
};

namespace Chaos {

#if !defined(__builtin_expect)
# define __builtin_expect(exp, c) (exp)
#endif

inline errno_t kern_gmtime(const time_t* timep, struct tm* result) {
  return gmtime_s(result, timep);
}

inline errno_t kern_strerror(int errnum, char* buf, size_t buflen) {
  return strerror_s(buf, buflen, errnum);
}

inline time_t kern_timegm(struct tm* timep) {
  return _mkgmtime(timep);
}

inline pid_t kern_getpid(void) {
  return static_cast<pid_t>(GetCurrentProcessId());
}

inline pid_t kern_gettid(void) {
  return static_cast<pid_t>(GetCurrentThreadId());
}

// int kern_getppid(void); // not support on Windows
int kern_gettimeofday(struct timeval* tv, struct timezone* tz);
int kern_this_thread_setname(const char* name);
int kern_backtrace(std::string& bt);

// Windows thread methods wrapper
struct _Thread_t {
  HANDLE notify_start{};
  HANDLE handle{};

  _Thread_t(void) = default;

  _Thread_t(std::nullptr_t)
    : notify_start(nullptr)
    , handle(nullptr) {
  }

  _Thread_t& operator=(std::nullptr_t) {
    notify_start = nullptr;
    handle = nullptr;
    return *this;
  }
};

int kern_thread_create(_Thread_t* thread, void* (*start_routine)(void*), void* arg);

inline int kern_thread_join(_Thread_t thread) {
  if (nullptr != thread.handle) {
    WaitForSingleObject(thread.handle, INFINITE);
    CloseHandle(thread.handle);
  }
  return 0;
}

inline int kern_thread_detach(_Thread_t thread) {
  if (nullptr != thread.handle)
    CloseHandle(thread.handle);
  return 0;
}

inline int kern_thread_atfork(void (*prepare)(void), void (*parent)(void), void (*child)(void)) {
  return 0;
}

// Windows thread local methods wrapper
typedef DWORD _Tls_t;

inline int kern_tls_create(_Tls_t* tls, void (*destructor)(void*)) {
  return *tls = FlsAlloc((PFLS_CALLBACK_FUNCTION)destructor), 0;
}

inline int kern_tls_delete(_Tls_t tls) {
  return TRUE == FlsFree(tls) ? 0 : -1;
}

inline int kern_tls_setspecific(_Tls_t tls, const void* value) {
  return TRUE == FlsSetValue(tls, (PVOID)value) ? 0 : -1;
}

inline void* kern_tls_getspecific(_Tls_t tls) {
  return FlsGetValue(tls);
}

// Windows execute once methods wrapper
typedef INIT_ONCE _Once_t;
const _Once_t kInitOnceValue = INIT_ONCE_STATIC_INIT;

int kern_once(_Once_t* once_control, void (*init_routine)(void));

// Windows low-level IO methods wrapper
namespace io {
  inline bool _kern_stat_istype(mode_t mode, mode_t mask) {
    return (mode & _S_IFMT) == mask;
  }

  inline bool kern_stat_isdir(mode_t mode) {
    return _kern_stat_istype(mode, _S_IFDIR);
  }

  inline bool kern_stat_ischr(mode_t mode) {
    return _kern_stat_istype(mode, _S_IFCHR);
  }

  inline bool kern_stat_isreg(mode_t mode) {
    return _kern_stat_istype(mode, _S_IFREG);
  }

  inline int kern_open(const char* path, int oflag) {
    return _open(path, oflag);
  }

  inline int kern_close(int fileds) {
    return _close(fileds);
  }

  inline ssize_t kern_read(int fd, void* buf, size_t len) {
    return _read(fd, buf, len);
  }

  inline ssize_t kern_pread(int fd, void* buf, size_t len, off_t offset) {
    _lseek(fd, 0L, SEEK_SET);
    return _read(fd, buf, len);
  }

  typedef struct _stat _Stat_t;
  inline int kern_fstat(int fileds, _Stat_t* buf) {
    return _fstat(fileds, buf);
  }
}

}

#endif // CHAOS_KERN_WINDOWS_KERNCOMMON_H
