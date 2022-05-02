// Created by Xilong Yang on 2022-04-28.
//

#include "skip_list.h"

#include <random>
#include <ctime>

namespace littledb {

size_t SkipList::kMaxHeight = 16;

size_t SkipList::GetMaxHeight() {
  return kMaxHeight;
}

size_t SkipList::RandomLevel() {
  static std::default_random_engine e(time(0));
  static std::bernoulli_distribution b(0.5);
  size_t level = 1;
  for (int i = 0; i < kMaxHeight; ++i) {
    if (b(e) || level == kMaxHeight) {
      return level;
    }
    ++level;
  }
  return level;
}

SkipList::SkipList(MemPool* mem_pool)
    : mem_pool_(mem_pool)
      , head_(NewNode(InnerKey(), kMaxHeight)) {}

void SkipList::Insert(const InnerKey &key, const Code &value) {
  auto prev = FindLessThan(key);
  auto Node = NewNode(key, RandomLevel(), value);
  for (int i = 0; i < Node->height_; ++i) {
    Node->next_[i] = prev->next_[i];
    prev->next_[i] = Node;
  }
}

Code SkipList::Get(const InnerKey &key) {
  auto x = FindGreaterOrEqual(key);
  if (x != nullptr && x != head_ && x->key_ == key) {
    return x->GetValue();
  }
  throw NotFoundError();
}

SkipList::Node*
SkipList::FindGreaterOrEqual(const InnerKey &key, Node **prev) const {
  Node* x = head_;
  size_t level = GetMaxHeight() - 1;
  while (true) {
    Node* next = x->next_[level];
    if (next != nullptr && key > next->key_) {
      x = next;
    } else {
      if (prev != nullptr) prev[level] = x;
      if (level == 0) {
        return next;
      } else {
        level--;
      }
    }
  }
}

SkipList::Node* SkipList::FindLessThan(const InnerKey &key) const {
  Node *x = head_;
  size_t level = GetMaxHeight() - 1;
  while (true) {
    if (x != head_ && x->key_ >= key) {
      throw std::runtime_error("FindLessThan Key Error.");
    }
    Node* next = x->next_[level];
    if (next == nullptr || next->key_ >= key) {
      if (level == 0) {
        return x;
      } else {
        level--;
      }
    } else {
      x = next;
    }
  }
}

SkipList::Node* SkipList::FindLast() const {
  Node* x = head_;
  while (x->next_[0] != nullptr) {
    x = x->next_[0];
  }
  return x;
}

SkipList::Node*
SkipList::NewNode(const InnerKey &key, size_t height, const Code value) {
  auto const node_mem =
      mem_pool_->Allocate(sizeof(SkipList::Node));
  auto const next_mem = reinterpret_cast<Node**>(
      mem_pool_->Allocate(sizeof(SkipList::Node*) * height));
  auto const value_length = value.value().size();
  Byte* value_mem = nullptr;
  if (value_length != 0) {
    value_mem = reinterpret_cast<Byte*>(
        mem_pool_->Allocate(value_length));
  }
  for (auto i = 0; i < value_length; ++i) {
    value_mem[i] = value.value()[i];
  }
  return new (node_mem) Node(key, height, next_mem
                             , value_mem, value_length);
}

SkipList::Iterator::Iterator(const SkipList *list)
    : list_(list), node_(list->head_) {}

bool SkipList::Iterator::Valid() const {
  return node_ != nullptr;
}

const InnerKey& SkipList::Iterator::Key() const {
  if (!Valid()) {
    throw std::runtime_error("Iterator not valid");
  }
  return node_->key_;
}

Code SkipList::Iterator::Value() const {
  if (!Valid()) {
    throw std::runtime_error("Iterator not valid");
  }
  return node_->GetValue();
}

void SkipList::Iterator::Next() {
  if (!Valid()) {
    throw std::runtime_error("Iterator not valid");
  }
  node_ = node_->next_[0];
}

void SkipList::Iterator::Prev() {
  if (!Valid()) {
    throw std::runtime_error("Iterator not valid");
  }
  node_ = list_->FindLessThan(node_->key_);
  if (node_ == list_->head_) {
    node_ = nullptr;
  }
}

void SkipList::Iterator::Seek(const InnerKey &target) {
  node_ = list_->FindGreaterOrEqual(target);
}

void SkipList::Iterator::SeekToFirst() {
  node_ = list_->head_->next_[0];
}

void SkipList::Iterator::SeekToLast() {
  node_ = list_->FindLast();
  if (node_ == list_->head_) {
    node_ = nullptr;
  }
}

SkipList::Node::Node(const InnerKey &key, size_t height, Node** next
                     , Byte *value, size_t value_length)
: key_(key), height_(height), value_(value), value_length_(value_length)
, next_(next){}

Code SkipList::Node::GetValue() const {
  auto byte_str = ByteString(value_length_, 0);
  for (int i = 0; i < byte_str.length(); ++i) {
    byte_str[i] = value_[i];
  }
  return Code(byte_str);
}

bool SkipList::Node::operator==(const Node &other) const {
  return key_ == other.key_ && value_ == other.value_;
}

bool SkipList::Node::operator!=(const Node &other) const {
  return !operator==(other);
}

bool SkipList::Node::operator<(const Node &other) const {
  return key_ < other.key_;
}

bool SkipList::Node::operator>(const Node &other) const {
  return key_ > other.key_;
}

bool SkipList::Node::operator<=(const Node &other) const {
  return key_ <= other.key_;
}

bool SkipList::Node::operator>=(const Node &other) const {
  return key_ >= other.key_;
}

}
