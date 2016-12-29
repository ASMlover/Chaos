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
#ifndef CHAOS_UTILITY_ANY_H
#define CHAOS_UTILITY_ANY_H

#include <typeinfo>
#include <utility>
#include <Chaos/Utility/Traits.h>

namespace Chaos {

class Any {
  class Placeholder {
  public:
    virtual ~Placeholder(void) = default;

    virtual const std::type_info& get_type(void) const = 0;
    virtual Placeholder* clone(void) const = 0;
  };

  template <typename ValueType>
  class Holder : public Placeholder {
    Holder& operator=(const Holder&) = delete;
  public:
    ValueType held_;
  public:
    Holder(const ValueType& value)
      : held_(value) {
    }

    Holder(const ValueType&& value)
      : held_(std::move(value)) {
    }

    virtual const std::type_info& get_type(const) const override {
      return typeid(ValueType);
    }

    virtual Placeholder* clone(void) const override {
      return new Holder(held_);
    }
  };

  Placeholder* content_{};

  template <typename ValueType> friend ValueType* any_cast(Any* operand);
  template <typename ValueType> friend ValueType* unsafe_any_cast(Any* operand);
public:
  Any(void) = default;

  ~Any(void) {
    if (nullptr != content_)
      delete content_;
  }

  template <typename ValueType>
  Any(const ValueType& value)
    : content_(new Holder<RemoveCV_t<typename std::decay<const ValueType>::trype>>(value)) {
  }

  template <typename ValueType>
  Any(ValueType&& value,
      DisableIf_t<std::is_same<Any&, ValueType>::value>* = nullptr,
      DisableIf_t<std::is_const<ValueType>::value>* = nullptr)
    : content_(new Holder<typename std::decay<ValueType>::type>(std::move(value))) {
  }

  Any(const Any& r)
    : content_(nullptr != r.content_ ? r.content_->clone() : nullptr) {
  }

  Any(Any&& r)
    : content_(r.content_) {
    r.content_ = nullptr;
  }

  Any& operator=(const Any& r) {
    Any(r).swap(*this);
    return *this;
  }

  Any& operator=(Any&& r) {
    r.swap(*this);
    Any().swap(r);
    return *this;
  }

  template <typename ValueType>
  Any& operator=(const ValueType& r) {
    Any(r).swap(*this);
    return *this;
  }

  template <typename ValueType>
  Any& operator=(ValueType&& r) {
    Any(std::move(r)).swap(*this);
    return *this;
  }

  void swap(Any& r) {
    std::swap(content_, r.content_);
  }

  bool empty(void) const {
    return nullptr == content_;
  }

  void clear(void) {
    Any().swap(*this);
  }

  const std::type_info& get_type(void) const {
    return (nullptr != content_) ? content_->get_type() : typeid(void);
  }
};

class BadAnyCast : public std::bad_cast {
public:
  virtual const char* what(void) const noexcept override {
    return "Chaos::BadAnyCast - failed conversion using Chaos::any_cast";
  }
};

}

#endif // CHAOS_UTILITY_ANY_H
