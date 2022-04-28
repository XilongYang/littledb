// Created by Xilong Yang on 2022-04-28.
//

#include "mempool.h"

#include <exception>

namespace littledb {
size_t MemPool::kBlockSize = 4096;

MemPool::MemPool() : allocate_ptr_(nullptr), used_bytes_(0), pool_() {
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
    used_bytes_ += bytes;
    allocated_bytes_ = 0;

    pool_.push_back(new char[bytes]);
    pool_.push_back(new char[kBlockSize]);
    return *(pool_.end() - 2);
  }

  auto remain_bytes = allocate_ptr_ - pool_.back();
  if (remain_bytes < bytes) {
    used_bytes_ += kBlockSize;
    allocated_bytes_ = 0;

    pool_.push_back(new char[kBlockSize]);
    allocate_ptr_ = pool_.back() + bytes;
    return pool_.back();
  }

  auto result = allocate_ptr_;
  allocate_ptr_ += bytes;
  allocated_bytes_ += bytes;
  return result;
}

size_t MemPool::UsageBytes() const {
  return used_bytes_ + allocated_bytes_;
}

}
