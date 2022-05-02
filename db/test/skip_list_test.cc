// Created by Xilong Yang on 2022-05-02.
//

#include <gtest/gtest.h>
#include "skip_list.h"

using namespace littledb;

TEST(SkipList, SkipList) {
  auto key1 = Code(ByteString(
      Sha256(Code(
                 ToByteString("Hello"))).value()
          ,0,16));

  auto key2 = Code(ByteString(
      Sha256(Code(
          ToByteString("World"))).value()
      ,0,16));

  auto key3 = Code(ByteString(
      Sha256(Code(
          ToByteString("none"))).value()
      ,0,16));

  InnerKey k1(key1);
  InnerKey k2(key2);
  InnerKey k3(key3);

  auto mem_pool = new MemPool();
  auto skip_list = new SkipList(mem_pool);

  EXPECT_THROW(skip_list->Get(k1), NotFoundError);

  skip_list->Insert(k1, Code(ToByteString("Hello!")));
  skip_list->Insert(k2, Code(ToByteString("World!")));

  EXPECT_EQ(skip_list->Get(k1), Code(ToByteString("Hello!")));
  EXPECT_EQ(skip_list->Get(k2), Code(ToByteString("World!")));

  SkipList::Iterator i(skip_list);
  EXPECT_EQ(i.Valid(), true);

  i.Prev();
  EXPECT_EQ(i.Valid(), false);

  i = SkipList::Iterator(skip_list);
  EXPECT_EQ(i.Key(), InnerKey());
  EXPECT_EQ(i.Value(), Code());

  i.Next();
  EXPECT_EQ(i.Key(), k2);
  EXPECT_EQ(i.Value(), Code(ToByteString("World!")));

  i.Next();
  EXPECT_EQ(i.Key(), k1);
  EXPECT_EQ(i.Value(), Code(ToByteString("Hello!")));

  i.SeekToFirst();
  EXPECT_EQ(i.Key(), k2);
  EXPECT_EQ(i.Value(), Code(ToByteString("World!")));

  i.SeekToLast();
  EXPECT_EQ(i.Key(), k1);
  EXPECT_EQ(i.Value(), Code(ToByteString("Hello!")));

  i.Seek(k1);
  EXPECT_EQ(i.Key(), k1);
  EXPECT_EQ(i.Value(), Code(ToByteString("Hello!")));

  i.Seek(k3);
  EXPECT_EQ(i.Valid(), false);
}