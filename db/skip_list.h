// Created by Xilong Yang on 2022-04-28.
//

#ifndef LITTLEDB_SKIP_LIST_H
#define LITTLEDB_SKIP_LIST_H

#include "littledb.h"
#include "mempool.h"

namespace littledb{


class SkipList {
 public:
  SkipList();
  ~SkipList();



 private:
  static size_t kMaxHeight;

  struct Node {
    Node* next_;
    Code key_;
    Code value_;
  };

  Node* const head_;
  MemPool* const mem_pool_;
};

}

#endif  // LITTLEDB_SKIP_LIST_H
