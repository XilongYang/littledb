// Created by Xilong Yang on 2022-05-01.
//

#include "memtable.h"

namespace littledb {

MemTable::MemTable() : mem_pool_(), skip_list_(&mem_pool_) {}

void MemTable::Add(const InnerKey &key, const Code &value) {
  skip_list_.Insert(key, value);
}

Code MemTable::Get(const InnerKey &key) {
  return skip_list_.Get(key);
}

}