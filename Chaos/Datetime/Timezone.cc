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
#include <cstdio>
#include <cstring>
#include <algorithm>
#include <vector>
#include <Chaos/Base/Platform.hh>
#include <Chaos/Base/Types.hh>
#include <Chaos/Except/SystemError.hh>
#include <Chaos/IO/ColorIO.hh>
#include <Chaos/Kern/KernCommon.hh>
#include <Chaos/Datetime/Date.hh>
#include <Chaos/Datetime/Timezone.hh>

namespace Chaos {

struct Transition {
  std::time_t gmttime;
  std::time_t localtime;
  int localtime_index;

  Transition(std::time_t gmt, std::time_t lt, int index)
    : gmttime(gmt)
    , localtime(lt)
    , localtime_index(index) {
  }
};

struct TransitionCompare {
  bool compare_with_gmt{};

  explicit TransitionCompare(bool cmp_with_gmt)
    : compare_with_gmt(cmp_with_gmt) {
  }

  bool operator()(const Transition& a, const Transition& b) const {
    if (compare_with_gmt)
      return a.gmttime < b.gmttime;
    else
      return a.localtime < b.localtime;
  }

  bool equal(const Transition& a, const Transition& b) const {
    if (compare_with_gmt)
      return a.gmttime == b.gmttime;
    else
      return a.localtime == b.localtime;
  }
};

struct Localtime {
  std::time_t gmtoff;
  bool isdst;
  int arrb_index;

  Localtime(std::time_t _gmtoff, bool _isdst, int _arrb)
    : gmtoff(_gmtoff)
    , isdst(_isdst)
    , arrb_index(_arrb) {
  }
};

static constexpr int kSecondsPerDay = 86400; // 24 * 60 * 60

struct TZData {
  std::vector<Transition> transitions;
  std::vector<Localtime> localtimes;
  std::vector<std::string> names;
  std::string abbreviation;
};

inline void fill_time(std::uint32_t sec, struct std::tm& utc) {
  std::uint32_t min = sec / 60;
  utc.tm_hour = min / 60;
  utc.tm_min = min % 60;
  utc.tm_sec = sec % 60;
}

class TZFile : private UnCopyable {
  std::FILE* stream_{};
public:
  TZFile(const char* fname)
    : stream_(std::fopen(fname, "rb")) {
  }

  ~TZFile(void) {
    if (nullptr != stream_)
      std::fclose(stream_);
  }

  bool is_valid(void) const {
    return nullptr != stream_;
  }

  std::string read_bytes(std::size_t bytes) {
    CHAOS_ARRAY(char, buf, bytes);
    std::size_t n = std::fread(buf, 1, bytes, stream_);
    if (n != bytes)
      __chaos_throw_exception(std::logic_error("no enough data"));
    return buf;
  }

  template <typename Integer>
  Integer read_integer(void) {
    static_assert(
        (sizeof(Integer) == sizeof(std::int8_t) ||
         sizeof(Integer) == sizeof(std::int16_t) ||
         sizeof(Integer) == sizeof(std::int32_t) ||
         sizeof(Integer) == sizeof(std::int64_t)),
        "Integer size should be `1`, `2`, `4`, `8` bytes");

    Integer x = 0;
    std::size_t n = std::fread(&x, 1, sizeof(Integer), stream_);
    if (n != sizeof(Integer))
      __chaos_throw_exception(std::logic_error("bad Integer data"));
    return x;
  }

  std::uint8_t read_uint8(void) {
    return read_integer<std::uint8_t>();
  }

  std::int32_t read_int32(void) {
    return read_integer<std::int32_t>();
  }
};

bool read_timezone_file(const char* zonefile, struct TZData* tzdata) {
  if (TZFile f(zonefile); f.is_valid()) {
    try {
      std::string head = f.read_bytes(4);
      if (head != "TZif")
        throw std::logic_error("read_timezone_file - bad head");
      std::string version = f.read_bytes(1);
      f.read_bytes(15);

      std::int32_t isgmtcnt = f.read_int32();
      std::int32_t isstdcnt = f.read_int32();
      std::int32_t leapcnt = f.read_int32();
      std::int32_t timecnt = f.read_int32();
      std::int32_t typecnt = f.read_int32();
      std::int32_t charcnt = f.read_int32();

      std::vector<std::int32_t> trans;
      std::vector<int> localtimes;
      trans.reserve(timecnt);
      for (int i = 0; i < timecnt; ++i)
        trans.push_back(f.read_int32());
      for (int i = 0; i < timecnt; ++i)
        localtimes.push_back(f.read_uint8());
      for (int i = 0; i < typecnt; ++i) {
        std::int32_t gmtoff = f.read_int32();
        bool isdst = 0 != f.read_uint8();
        std::uint8_t arrb_index = f.read_uint8();

        tzdata->localtimes.push_back(Localtime(gmtoff, isdst, arrb_index));
      }
      for (int i = 0; i < timecnt; ++i) {
        int local_index = localtimes[i];
        std::time_t lt = trans[i] + tzdata->localtimes[local_index].gmtoff;
        tzdata->transitions.push_back(Transition(trans[i], lt, local_index));
      }
      tzdata->abbreviation = f.read_bytes(charcnt);

      for (int i = 0; i < leapcnt; ++i) {
      }
      CHAOS_UNUSED(isstdcnt);
      CHAOS_UNUSED(isgmtcnt);
    }
    catch (std::logic_error& e) {
      ColorIO::fprintf(stderr,
          ColorIO::ColorType::COLORTYPE_FG_RED, "%s\n", e.what());
    }
  }
  return true;
}

const Localtime* find_localtime(
    const TZData& tzdata, Transition sentry, TransitionCompare cmp) {
  const Localtime* local = nullptr;

  if (tzdata.transitions.empty() || cmp(sentry, tzdata.transitions.front())) {
    local = &tzdata.localtimes.front();
  }
  else {
    auto trans_iter = std::lower_bound(
        tzdata.transitions.begin(), tzdata.transitions.end(), sentry, cmp);
    if (trans_iter != tzdata.transitions.end()) {
      if (!cmp.equal(sentry, *trans_iter)) {
        CHAOS_CHECK(trans_iter != tzdata.transitions.begin(),
            "TZData's transitions error");
        --trans_iter;
      }
      local = &tzdata.localtimes[trans_iter->localtime_index];
    }
    else {
      local = &tzdata.localtimes[tzdata.transitions.back().localtime_index];
    }
  }
  return local;
}

Timezone::Timezone(const char* zonefile)
  : data_(new TZData()) {
  if (!read_timezone_file(zonefile, data_.get()))
    data_.reset();
}

Timezone::Timezone(int east_of_utc, const char* tzname)
  : data_(new TZData()) {
  data_->localtimes.push_back(Localtime(east_of_utc, false, 0));
  data_->abbreviation = tzname;
}

bool Timezone::is_valid(void) const {
  return static_cast<bool>(data_);
}

struct std::tm Timezone::to_localtime(std::time_t sec_since_epoch) const {
  struct std::tm ltime{};
  CHAOS_CHECK(nullptr != data_,
      "Timezone::to_localtime - `data_` should not be null");
  const TZData& data(*data_);

  Transition sentry(sec_since_epoch, 0, 0);
  const Localtime* local =
    find_localtime(data, sentry, TransitionCompare(true));

  if (nullptr != local) {
    std::time_t local_seconds = sec_since_epoch + local->gmtoff;
    Chaos::kern_gmtime(&local_seconds, &ltime);
    ltime.tm_isdst = local->isdst;
#if !defined(CHAOS_WINDOWS)
    ltime.tm_gmtoff = local->gmtoff;
    ltime.tm_zone = (char*)&data.abbreviation[local->arrb_index];
#endif
  }
  return ltime;
}

std::time_t Timezone::from_localtime(const struct std::tm& t) const {
  CHAOS_CHECK(nullptr != data_,
      "Timezone::from_localtime - `data_` should not be null");
  const TZData data(*data_);

  struct std::tm tmp = t;
  std::time_t seconds = Chaos::kern_timegm(&tmp);
  Transition sentry(0, seconds, 0);
  const Localtime* local =
    find_localtime(data, sentry, TransitionCompare(false));
  if (t.tm_isdst) {
    struct std::tm try_tm = to_localtime(seconds - local->gmtoff);
    if (try_tm.tm_isdst && try_tm.tm_hour ==
        t.tm_hour && try_tm.tm_min == t.tm_min)
      seconds -= 3600;
  }
  return seconds - local->gmtoff;
}

struct std::tm Timezone::to_utc_time(std::time_t sec_since_epoch, bool yday) {
  struct std::tm utc{};
#if !defined(CHAOS_WINDOWS)
  utc.tm_zone = const_cast<char*>("GMT");
#endif
  int seconds = static_cast<int>(sec_since_epoch % kSecondsPerDay);
  int days = static_cast<int>(sec_since_epoch / kSecondsPerDay);
  if (seconds < 0) {
    seconds += kSecondsPerDay;
    --days;
  }
  fill_time(seconds, utc);
  Date date(days + Date::kEpochDay19700101);
  Date::DateValue dv = date.get_date();
  utc.tm_year = dv.year - 1900;
  utc.tm_mon = dv.month - 1;
  utc.tm_mday = dv.day;
  utc.tm_wday = date.weekday();

  if (yday) {
    Date start(dv.year, 1, 1);
    utc.tm_yday = date.epoch_day() - start.epoch_day();
  }
  return utc;
}

std::time_t Timezone::from_utc_time(const struct std::tm& utc) {
  return from_utc_time(utc.tm_year + 1900,
      utc.tm_mon + 1, utc.tm_mday, utc.tm_hour, utc.tm_min, utc.tm_sec);
}

std::time_t Timezone::from_utc_time(
    int year, int month, int day, int hour, int min, int sec) {
  Date date(year, month, day);
  int sec_in_day = hour * 3600 + min * 60 + sec;
  std::time_t days = date.epoch_day() - Date::kEpochDay19700101;
  return days * kSecondsPerDay + sec_in_day;
}

}
