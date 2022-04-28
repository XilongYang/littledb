// Created by Xilong Yang on 2022-04-28.
//

#ifndef LITTLEDB_MEMPOOL_H
#define LITTLEDB_MEMPOOL_H
#include "littledb.h"
#include <vector>

namespace littledb{
class MemPool {
 public:
  MemPool();
  ~MemPool();

  MemPool(const MemPool&) = delete;
  MemPool(MemPool&&) = delete;

  char* Allocate(size_t bytes);

  size_t UsageBytes() const;
 private:
  char* allocate_ptr_;
  size_t used_bytes_;
  size_t allocated_bytes_;
  std::vector<char*> pool_;

  static size_t kBlockSize;
};
}

#endif  // LITTLEDB_MEMPOOL_H
