// Created by Xilong Yang on 2022-05-02.
//

#include <gtest/gtest.h>
#include "memtable.h"

using namespace littledb;

TEST(MemTable, MemTable) {
  MemTable table;

  auto k1 = Code(ToByteString("Hello"));
  auto k2 = Code(ToByteString("World"));

  table.Add(0, littledb::MemTable::kTypeValue
            ,k1, Code(ToByteString("Hello!")));
  table.Add(1, littledb::MemTable::kTypeValue
            ,k2, Code(ToByteString("World!")));

  EXPECT_EQ(table.Get(k1), Code(ToByteString("Hello!")));
  EXPECT_EQ(table.Get(k2), Code(ToByteString("World!")));

  EXPECT_THROW(table.Get(Code()), NotFoundError);
}