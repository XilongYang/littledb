// Created by Xilong Yang on 2022-05-01.
//

#ifndef LITTLEDB_MEMTABLE_H
#define LITTLEDB_MEMTABLE_H

#include "littledb.h"
#include "skip_list.h"
#include "mempool.h"

namespace littledb{

class MemTable {
 public:
  MemTable();

  MemTable(const MemTable&) = delete;
  MemTable& operator=(MemTable&) = delete;

  void Add(const InnerKey& key, const Code& value);
  Code Get(const InnerKey& key);
 private:
  MemPool mem_pool_;
  SkipList skip_list_;
};

}

#endif  // LITTLEDB_MEMTABLE_H
