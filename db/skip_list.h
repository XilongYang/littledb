// Created by Xilong Yang on 2022-04-28.
//

#ifndef LITTLEDB_SKIP_LIST_H
#define LITTLEDB_SKIP_LIST_H

#include "littledb.h"
#include "mempool.h"
#include "inner_key.h"

namespace littledb{


class SkipList {
 public:
  SkipList(MemPool* mem_pool);

  static size_t GetMaxHeight();

  void Insert(const InnerKey& key, const Code& value = Code());
  Code Get(const InnerKey& key);

  class Iterator;

 private:
  class Node;
  static size_t kMaxHeight;
  static size_t RandomLevel();

  Node* const head_;
  MemPool* const mem_pool_;

  Node* FindGreaterOrEqual(const InnerKey& key, Node** prev = nullptr) const;
  Node* FindLessThan(const InnerKey& key) const;
  Node* FindLast() const;

  Node* NewNode(const InnerKey& key, size_t height, const Code value = Code());
};

class SkipList::Iterator{
 public:
  explicit Iterator(const SkipList* list);

  bool Valid() const;

  const InnerKey& Key() const;

  const Code& Value() const;

  void Next();

  void Prev();

  void Seek(const InnerKey& target);

  void SeekToFirst();

  void SeekToLast();

 private:
  const SkipList* list_;
  Node* node_;
};

struct SkipList::Node {
  Node(const InnerKey& key, size_t height = 0, Node** next = nullptr
       , Byte* value = nullptr, size_t value_length = 0);

  Code GetValue() const;

  bool operator==(const Node& other) const;
  bool operator!=(const Node& other) const;

  bool operator<(const Node& other) const;
  bool operator>(const Node& other) const;

  bool operator<=(const Node& other) const;
  bool operator>=(const Node& other) const;

  InnerKey key_;
  size_t height_;
  Byte* value_;
  size_t value_length_;
  Node** next_;

};

}

#endif  // LITTLEDB_SKIP_LIST_H
