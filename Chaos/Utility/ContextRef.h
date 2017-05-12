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
#ifndef CHAOS_UTILITY_CONTEXTREF_H
#define CHAOS_UTILITY_CONTEXTREF_H

#include <memory>
#include <Chaos/Copyable.h>

namespace Chaos {

template <typename T, typename Allocator = std::allocator<char>>
class ContextRef : public Copyable {
  struct Node {
    std::shared_ptr<void> parent;
    T value;

    template <typename... Args>
    Node(std::shared_ptr<void> p, Args&&... args)
      : parent(std::move(p))
      , value(std::forward<Args>(args)...) {
    }
  };

  std::shared_ptr<Node> node_;
  Allocator alloc_;

  template <typename Y, typename YAllocator>
  friend class ContextRef;
public:
  using value_type = T;
  using allocator_type = Allocator;

  explicit ContextRef(const allocator_type& alloc) noexcept
    : alloc_(alloc) {
  }

  template <typename... Args>
  ContextRef(
      std::shared_ptr<void> parent, const allocator_type& alloc, Args&&... args)
    : node_(std::allocate_shared<Node>(
          alloc, std::move(parent), std::forward<Args>(args)...))
    , alloc_(alloc) {
  }

  ContextRef(const ContextRef& r) noexcept
    : node_(r.node_)
    , alloc_(r.alloc_) {
  }

  ContextRef(ContextRef&& r) noexcept
    : node_(std::move(r.node_))
    , alloc_(std::move(r.alloc_)) {
  }

  ContextRef& operator=(const ContextRef& r) noexcept {
    node_ = r.node_;
    alloc_ = r.alloc_;
    return *this;
  }

  ContextRef& operator=(ContextRef&& r) noexcept {
    node_ = std::move(r.node_);
    alloc_ = std::move(r.alloc_);
    return *this;
  }

  void swap(ContextRef& r) noexcept {
    std::swap(node_, r.node_);
    std::swap(alloc_, r.alloc_);
  }

  template <typename U, typename... Args>
  ContextRef<U, Allocator> spawn(Args&&... args) const {
    return ContextRef<U, Allocator>(node_, alloc_, std::forward<Args>(args)...);
  }

  template <typename U, typename UAllocator, typename... Args>
  ContextRef<U, UAllocator> spawn_with_allocator(
      const UAllocator& alloc, Args&&... args) const {
    return ContextRef<U, UAllocator>(node_, alloc, std::forward<Args>(args)...);
  }

  std::shared_ptr<void> get_pointer(void) const {
    return node_;
  }

  allocator_type get_allocator(void) const {
    return alloc_;
  }

  T* get(void) {
    return node_ == nullptr ? nullptr : &node_->value;
  }

  const T* get(void) const {
    return node_ == nullptr ? nullptr : &node_->value;
  }

  T& operator*(void) {
    return node_->value;
  }

  const T& operator*(void) const {
    return node_->value;
  }

  T* operator->(void) {
    return &node_->value;
  }

  const T* operator->(void) const {
    return &node_->value;
  }

  explicit operator bool(void) const {
    return node_ != nullptr;
  }

  ContextRef<void, Allocator> operator()(void) const {
    return ContextRef<void, Allocator>(*this);
  }
};

template <typename Allocator>
class ContextRef<void, Allocator> : public Copyable {
  std::shared_ptr<void> node_;
  Allocator alloc_;
public:
  using value_type = void;
  using allocator_type = Allocator;

  explicit ContextRef(const allocator_type& alloc) noexcept
    : alloc_(alloc) {
  }

  ContextRef(std::shared_ptr<void> parent, const allocator_type& alloc) noexcept
    : node_(std::move(parent))
    , alloc_(alloc) {
  }

  ContextRef(std::shared_ptr<void> parent) noexcept
    : ContextRef(parent, Allocator()) {
  }

  ContextRef(const ContextRef& r) noexcept
    : node_(r.node_)
    , alloc_(r.alloc_) {
  }

  template <typename Y>
  ContextRef(const ContextRef<Y, Allocator>& r) noexcept
    : node_(r.node_)
    , alloc_(r.alloc_) {
  }

  ContextRef(ContextRef&& r) noexcept
    : node_(std::move(r.node_))
    , alloc_(std::move(r.alloc_)) {
  }

  template <typename Y>
  ContextRef(ContextRef<Y, Allocator>&& r) noexcept
    : node_(std::move(r.node_))
    , alloc_(std::move(r.alloc_)) {
  }

  ContextRef& operator=(const ContextRef& r) {
    node_ = r.node_;
    alloc_ = r.alloc_;
    return *this;
  }

  template <typename Y>
  ContextRef& operator=(const ContextRef<Y>& r) {
    node_ = r.node_;
    alloc_ = r.alloc_;
    return *this;
  }

  ContextRef& operator=(ContextRef&& r) noexcept {
    node_ = std::move(r.node_);
    alloc_ = std::move(r.alloc_);
    return *this;
  }

  template <typename Y>
  ContextRef& operator=(ContextRef<Y>&& r) noexcept {
    node_ = std::move(r.node_);
    alloc_ = std::move(r.alloc_);
    return *this;
  }

  ContextRef& operator=(std::shared_ptr<void> parent) noexcept {
    node_ = std::move(parent);
    return *this;
  }

  void swap(ContextRef& r) noexcept {
    std::swap(node_, r.node_);
    std::swap(alloc_, r.alloc_);
  }

  template <typename U, typename... Args>
  ContextRef<U, Allocator> spawn(Args&&... args) const {
    return ContextRef<U, Allocator>(node_, alloc_, std::forward<Args>(args)...);
  }

  template <typename U, typename UAllocator, typename... Args>
  ContextRef<U, UAllocator> spawn_with_allocator(
      const UAllocator& alloc, Args&&... args) const {
    return ContextRef<U, UAllocator>(node_, alloc, std::forward<Args>(args)...);
  }

  std::shared_ptr<void> get_pointer(void) const {
    return node_;
  }

  allocator_type get_allocator(void) const {
    return alloc_;
  }

  explicit operator bool(void) const {
    return node_ != nullptr;
  }

  ContextRef<void, Allocator> operator()(void) const {
    return *this;
  }
};

template <typename T, typename Allocator>
inline bool operator==(const ContextRef<T, Allocator>& p, std::nullptr_t) {
  return !p;
}

template <typename T, typename Allocator>
inline bool operator==(std::nullptr_t, const ContextRef<T, Allocator>& p) {
  return p == nullptr;
}

template <typename T, typename Allocator>
inline bool operator!=(const ContextRef<T, Allocator>& p, std::nullptr_t) {
  return !(p == nullptr);
}

template <typename T, typename Allocator>
inline bool operator!=(std::nullptr_t, const ContextRef<T, Allocator>& p) {
  return !(p == nullptr);
}

template <typename T, typename Allocator, typename... Args>
inline ContextRef<T, Allocator> make_context_with_allocator(
    const Allocator& alloc, Args&&... args) {
  return ContextRef<T, Allocator>(
      std::shared_ptr<void>(), alloc, std::forward<Args>(args)...);
}

template <typename T,
          typename Allocator = std::allocator<char>, typename... Args>
inline ContextRef<T, Allocator> make_context(Args&&... args) {
  return make_context_with_allocator(Allocator(), std::forward<Args>(args)...);
}

}

#endif // CHAOS_UTILITY_CONTEXTREF_H
