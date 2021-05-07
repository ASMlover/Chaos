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
#include <cerrno>
#include <cstdio>
#include <string>
#include <Chaos/Base/Platform.hh>
#include <Chaos/Base/Types.hh>
#include <Chaos/Logging/Logging.h>
#include <Chaos/Kern/KernCommon.h>
#include <Chaos/File/FileUtil.h>

namespace Chaos {

namespace FileUtil {
  ReadSmallFile::ReadSmallFile(StringArg fname)
#if defined(CHAOS_WINDOWS)
    : fd_(Chaos::io::kern_open(fname.c_str(), _O_RDONLY))
#else
    : fd_(Chaos::io::kern_open(fname.c_str(), O_RDONLY | O_CLOEXEC))
#endif
    , errno_(0) {
    if (fd_ < 0)
      errno_ = errno;
  }

  ReadSmallFile::~ReadSmallFile(void) {
    if (fd_ >= 0)
      Chaos::io::kern_close(fd_);
  }

  template <typename String>
  int ReadSmallFile::read_to_string(
      int maxsz, String* content, std::int64_t* filesz,
      std::int64_t* modify_time, std::int64_t* create_time) {
#if defined(CHAOS_WINDOWS)
    static_assert(sizeof(off_t) == 4, "offset bits is `32`");
#else
    static_assert(sizeof(off_t) == 8, "offset bits is `64`");
#endif
    CHAOS_CHECK(nullptr != content, "content must be valid");

    int err = errno_;
    if (fd_ >= 0) {
      content->clear();
      if (filesz != nullptr) {
        Chaos::io::_Stat_t statbuf;
        if (0 == Chaos::io::kern_fstat(fd_, &statbuf)) {
          if (Chaos::io::kern_stat_isreg(statbuf.st_mode)) {
            *filesz = statbuf.st_size;
            content->reserve(static_cast<int>(
                  chaos_min(implicit_cast<std::int64_t>(maxsz), *filesz)));
          }
          else if (Chaos::io::kern_stat_isdir(statbuf.st_mode)) {
            err = EISDIR;
          }

          if (nullptr != modify_time)
            *modify_time = statbuf.st_mtime;
          if (nullptr != create_time)
            *create_time = statbuf.st_ctime;
        }
        else {
          err = errno;
        }
      }

      while (content->size() < implicit_cast<std::size_t>(maxsz)) {
        std::size_t to_read = chaos_min(
            implicit_cast<std::size_t>(maxsz) - content->size(),
            sizeof(buffer_));
        ssize_t n = Chaos::io::kern_read(fd_, buffer_, to_read);
        if (n > 0) {
          content->append(buffer_, n);
        }
        else {
          if (n < 0)
            err = errno;
          break;
        }
      }
    }

    return err;
  }

  int ReadSmallFile::read_to_buffer(int* size) {
    int err = errno_;
    if (fd_ >= 0) {
      ssize_t n = Chaos::io::kern_pread(fd_, buffer_, sizeof(buffer_) - 1, 0);
      if (n >= 0) {
        if (nullptr != size)
          *size = static_cast<int>(n);
        buffer_[n] = '\0';
      }
      else {
        err = errno;
      }
    }

    return err;
  }

  template int ReadSmallFile::read_to_string(
      int, std::string*, std::int64_t*, std::int64_t*, std::int64_t*);
  template int read_file(StringArg,
      int, std::string*, std::int64_t*, std::int64_t*, std::int64_t*);

  AppendFile::AppendFile(StringArg fname)
    : stream_(std::fopen(fname.c_str(), "ae")) {
    CHAOS_CHECK(nullptr != stream_, "`stream_` with fopen should be valid");
    std::setvbuf(stream_, buffer_, _IOFBF, sizeof(buffer_));
  }

  AppendFile::~AppendFile(void) {
    std::fclose(stream_);
  }

  void AppendFile::append(const char* buf, std::size_t len) {
    std::size_t nwrote = 0;
    std::size_t nremain = len - nwrote;
    while (nremain > 0) {
      std::size_t n = write(buf + nwrote, nremain);
      if (0 == n) {
        int err = std::ferror(stream_);
        if (0 != err) {
          std::fprintf(stderr,
              "AppendFile::append - failed %s\n", Chaos::strerror_tl(err));
        }
        break;
      }
      nwrote += n;
      nremain = len - nwrote;
    }

    written_bytes_ += nwrote;
  }

  void AppendFile::flush(void) {
    std::fflush(stream_);
  }

  int AppendFile::write(const char* buf, std::size_t len) {
    return static_cast<int>(
        Chaos::io::kern_fwrite_unlocked(buf, 1, len, stream_));
  }
}

}
