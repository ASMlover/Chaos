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
#include <memory>
#include <tuple>
#include <vector>
#include <Chaos/Unittest/TestHarness.hh>

namespace Chaos {

using HarnessContext =
  std::tuple<std::string_view, std::string_view, HarnessClosure>;
using HarnessContextVector = std::vector<HarnessContext>;
HarnessContextVector* g_tests{}; // must be raw pointer for darwin
                                 // (unique_ptr is invalid)

bool register_testharness(
    std::string_view base, std::string_view name, HarnessClosure&& closure) {
  if (nullptr == g_tests)
    g_tests = new HarnessContextVector;

  g_tests->push_back(std::make_tuple(base, name, std::move(closure)));
  return true;
}

int run_all_testharness(void) {
  int total_tests = 0;
  int passed_tests = 0;

  if (nullptr != g_tests && !g_tests->empty()) {
    total_tests = static_cast<int>(g_tests->size());

    for (auto& hc : *g_tests) {
      auto [_, hc_name, hc_closure] = hc;

      hc_closure();
      ++passed_tests;
      ColorIO::fprintf(stdout,
          ColorIO::ColorType::COLORTYPE_FG_GREEN,
          "********** [%s] test harness PASSED (%d/%d) **********\n",
          hc_name.data(),
          passed_tests,
          total_tests);
    }

    delete g_tests;
  }
  ColorIO::fprintf(stdout,
      ColorIO::ColorType::COLORTYPE_FG_LIGHTGREEN,
      "========== PASSED (%d/%d) test harness ==========\n",
      passed_tests,
      total_tests);
  return 0;
}

}
