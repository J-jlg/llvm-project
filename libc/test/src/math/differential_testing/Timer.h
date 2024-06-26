//===-- Timer.h -------------------------------------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIBC_TEST_SRC_MATH_DIFFERENTIAL_TESTING_TIMER_H
#define LLVM_LIBC_TEST_SRC_MATH_DIFFERENTIAL_TESTING_TIMER_H

#include <stdint.h>

namespace LIBC_NAMESPACE {
namespace testing {

class Timer {
  void *Impl;

public:
  Timer();
  ~Timer();

  void start();
  void stop();

  uint64_t nanoseconds() const;
};

} // namespace testing
} // namespace LIBC_NAMESPACE

#endif // LLVM_LIBC_TEST_SRC_MATH_DIFFERENTIAL_TESTING_TIMER_H
