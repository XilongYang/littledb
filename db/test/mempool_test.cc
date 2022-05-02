// Created by Xilong Yang on 2022-05-02.
//

#include <gtest/gtest.h>

#include "mempool.h"

using namespace littledb;

TEST(MemPool, MemPool) {
  auto mem_pool = new MemPool();
  EXPECT_EQ(mem_pool->UsageBytes(), 0);
  EXPECT_EQ(mem_pool->AllocatedBytes(), 0);

  mem_pool->Allocate(20);
  mem_pool->Allocate(30);
  mem_pool->Allocate(2);
  EXPECT_EQ(mem_pool->UsageBytes(), 52);
  EXPECT_EQ(mem_pool->AllocatedBytes(), mem_pool->kBlockSize);

  mem_pool->Allocate(2000);
  EXPECT_EQ(mem_pool->UsageBytes(), 6096);
  EXPECT_EQ(mem_pool->AllocatedBytes(), mem_pool->kBlockSize + 2000);

  mem_pool->Allocate(1000);
  mem_pool->Allocate(1000);
  mem_pool->Allocate(1000);
  mem_pool->Allocate(1000);
  EXPECT_EQ(mem_pool->UsageBytes(), 10096);
  EXPECT_EQ(mem_pool->AllocatedBytes(), mem_pool->kBlockSize * 2 + 2000);

  EXPECT_THROW(mem_pool->Allocate(0), std::runtime_error);
}
