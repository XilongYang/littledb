// Created by Xilong Yang on 2022-04-28.
//

#include "mempool.h"

#include <exception>

namespace littledb {
MemPool::MemPool()
    : allocate_ptr_(nullptr), allocated_bytes_(0)
      , remain_bytes_(0), pool_() {
  pool_.push_back(new char[kBlockSize]);
  allocate_ptr_ = pool_.back();
}

MemPool::~MemPool() {
    for (auto p : pool_) {
      delete[] p;
    }
}

char* MemPool::Allocate(size_t bytes) {
  if (bytes <= 0) {
    throw std::runtime_error("Allocate bytes < 0.");
  }

  if (bytes > 1024) {
    allocated_bytes_ += bytes;
    remain_bytes_ = 0;

    pool_.push_back(new char[bytes]);
    return pool_.back();
  }

  if (remain_bytes_ < bytes) {
    allocated_bytes_ += kBlockSize;
    remain_bytes_ = kBlockSize - bytes;

    pool_.push_back(new char[kBlockSize]);
    allocate_ptr_ = pool_.back() + bytes;
    return pool_.back();
  }

  auto result = allocate_ptr_;
  allocate_ptr_ += bytes;
  remain_bytes_ -= bytes;
  return result;
}

size_t MemPool::UsageBytes() const {
  return allocated_bytes_ - remain_bytes_;
}

size_t MemPool::AllocatedBytes() const {
  return allocated_bytes_;
}

}
