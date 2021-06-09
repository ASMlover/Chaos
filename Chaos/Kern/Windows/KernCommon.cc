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
#include <DbgHelp.h>
#include <process.h>
#include <Chaos/Utility/Memory.hh>
#include <Chaos/Kern/Windows/KernCommon.hh>

namespace Chaos {

static constexpr std::uint64_t kEpoch = 116444736000000000ULL;
static constexpr DWORD kMSVCException = 0x406D1388;
static constexpr int kMaxBacktrace = 256;
static const HANDLE kMainProc = GetCurrentProcess();
static const BOOL kBTIgnoredInit = SymInitialize(kMainProc, NULL, TRUE);

#pragma pack(push, 8)
typedef struct _ThreadName_t {
  DWORD dwType; // must be 0x1000
  LPCSTR szName;
  DWORD dwThreadID; // thread id
  DWORD dwFlags;
} _ThreadName_t;
#pragma pack(pop)

typedef struct _ThreadBinder_t {
  _Thread_t* thread;
  void* (*start)(void*);
  void* arg;
} _ThreadBinder_t;

int kern_gettimeofday(struct timeval* tv, struct timezone* /*tz*/) {
  if (tv) {
    FILETIME ft;
    SYSTEMTIME st;
    ULARGE_INTEGER uli;

    GetSystemTime(&st);
    SystemTimeToFileTime(&st, &ft);
    uli.LowPart = ft.dwLowDateTime;
    uli.HighPart = ft.dwHighDateTime;

    tv->tv_sec = static_cast<long>((uli.QuadPart - kEpoch) / 10000000L);
    tv->tv_usec = static_cast<long>(st.wMilliseconds * 1000);
  }
  return 0;
}

int kern_100nanosleep(std::int64_t nanosec) {
  HANDLE timer;
  if ((timer = CreateWaitableTimer(NULL, TRUE, NULL)) == NULL)
    return -1;

  __try {
    LARGE_INTEGER li;
    li.QuadPart = -static_cast<LONGLONG>(nanosec);
    if (!SetWaitableTimer(timer, &li, 0, NULL, NULL, FALSE))
      return -1;

    WaitForSingleObject(timer, INFINITE);
  }
  __finally {
    CloseHandle(timer);
  }
  return 0;
}

int kern_this_thread_setname(const char* name) {
  _ThreadName_t tn;
  tn.dwType = 0x1000;
  tn.szName = name;
  tn.dwThreadID = GetCurrentThreadId();
  tn.dwFlags = 0;
  __try {
    RaiseException(
        kMSVCException, 0, sizeof(tn) / sizeof(ULONG_PTR), (ULONG_PTR*)&tn);
  }
  __except (EXCEPTION_EXECUTE_HANDLER) {
  }
  return 0;
}

int kern_backtrace(std::string& bt) {
  void* stack[kMaxBacktrace];
  int frames = CaptureStackBackTrace(0, kMaxBacktrace, stack, NULL);

  char symbol_buff[sizeof(SYMBOL_INFO) + kMaxBacktrace * sizeof(char)];
  PSYMBOL_INFO symbol = (PSYMBOL_INFO)symbol_buff;
  symbol->MaxNameLen = kMaxBacktrace;
  symbol->SizeOfStruct = sizeof(SYMBOL_INFO);

  char message[1024];
  for (int i = 0; i < frames; ++i) {
    SymFromAddr(kMainProc, *reinterpret_cast<DWORD*>(stack[i]), 0, symbol);
    std::snprintf(message,
        sizeof(message),
        "%i: %s - 0x%p\n",
        frames - i - 1,
        symbol->Name,
        (void*)symbol->Address);
    bt.append(message);
  }

  return 0;
}

static UINT CALLBACK kern_thread_start_routine(void* arg) {
  auto params = Chaos::as_unique<_ThreadBinder_t>(arg);
  if (!params)
    return 0;

  SetEvent(params->thread->notify_start);
  if (params->start)
    params->start(params->arg);

  return 0;
}

int kern_thread_create(
    _Thread_t* thread, void* (*start_routine)(void*), void* arg) {
  thread->notify_start = CreateEvent(NULL, TRUE, FALSE, NULL);
  if (!thread->notify_start)
    return -1;

  int result = -1;
  auto params = Chaos::make_unique<_ThreadBinder_t>();
  if (!params)
    goto _Exit;
  params->thread = thread;
  params->start = start_routine;
  params->arg = arg;

  thread->handle = reinterpret_cast<HANDLE>(_beginthreadex(
        nullptr, 0, kern_thread_start_routine, params.get(), 0, nullptr));
  if (!thread->is_valid())
    goto _Exit;

  WaitForSingleObject(thread->notify_start, INFINITE);
  params.release();
  result = 0;

_Exit:
  CloseHandle(thread->notify_start);
  thread->notify_start = NULL;
  return result;
}

static BOOL CALLBACK kern_once_callback(
    PINIT_ONCE /*once_control*/, PVOID arg, PVOID* /*context*/) {
  typedef void (*_InitCallback)(void);
  _InitCallback init_routine = (_InitCallback)arg;

  if (init_routine)
    init_routine();

  return TRUE;
}

int kern_once(_Once_t* once_control, void (*init_routine)(void)) {
  return InitOnceExecuteOnce(
      once_control, kern_once_callback, (PVOID)init_routine, nullptr) ? 0 : -1;
}

}
