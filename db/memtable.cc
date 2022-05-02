// Created by Xilong Yang on 2022-05-01.
//

#include "memtable.h"

namespace littledb {

namespace {
struct Entry : public CodableInterface {
 public:
  Entry() = default;
  explicit Entry(const Code& code) {
      Entry::Decode(code);
  }

  Code Encode() override{
    Byte seq = seq_;
    Byte type = type_;
    ByteString head{seq, type};
    return Code(head) + value_;
  }

  void Decode(const Code& code) override {
    if (code.value().length() < 2) {
      throw DecodeError("Entry code error.");
    }

    seq_ = code.value()[0];
    if (code.value()[1] == MemTable::kTypeDeleted) {
      type_ = MemTable::kTypeDeleted;
    } else {
      type_ = MemTable::kTypeValue;
    }
    value_ = Code({code.value().begin() + 2
                   , code.value().end()});
  }
  size_t seq_;
  MemTable::ValueType type_;
  Code value_;
};
}

MemTable::MemTable() : mem_pool_(), skip_list_(&mem_pool_) {}

size_t MemTable::MemoryUsage() {
  return mem_pool_.UsageBytes();
}

void MemTable::Add(size_t seq, ValueType type,
    const Code& key, const Code &value) {
  auto inner_key = InnerKey(
      Code({Sha256(key).value(), 0, 16}));

  Entry entry;
  entry.seq_ = seq;
  entry.type_ = type;
  entry.value_ = value;

  skip_list_.Insert(inner_key, entry.Encode());
}

Code MemTable::Get(const Code& key) {
  auto inner_key = InnerKey(
      Code({Sha256(key).value(), 0, 16}));
  auto entry = Entry(skip_list_.Get(inner_key));
  if (entry.type_ == MemTable::kTypeDeleted) {
    throw NotFoundError();
  }
  return entry.value_;
}

}