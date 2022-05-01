// Created by Xilong Yang on 2022-04-28.
//

#ifndef LITTLEDB_INNER_KEY_H
#define LITTLEDB_INNER_KEY_H

#include "littledb.h"

namespace littledb{

class InnerKey : public CodableInterface{
 public:
  InnerKey() = default;
  explicit InnerKey(const Code& code);

  Code Encode() override;
  void Decode(const Code& code) override;

  bool operator==(const InnerKey& other) const;
  bool operator!=(const InnerKey& other) const;

  bool operator<(const InnerKey& other) const;
  bool operator>(const InnerKey& other) const;

  bool operator<=(const InnerKey& other) const;
  bool operator>=(const InnerKey& other) const;
 private:
  uint64_t key_part1_;
  uint64_t key_part2_;
};

}


#endif  // LITTLEDB_INNER_KEY_H
