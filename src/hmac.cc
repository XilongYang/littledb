// Created by Xilong Yang on 2022-03-20.
//

#include "littledb.h"


namespace littledb {
namespace {
  Code XOR(const Code& code, uint8_t pad) {
    auto str = code.value();
    for (auto &c : str) {
      c ^= pad;
    }
    return Code(str);
  }
}

Code Hmac(const Code& message, const Code& key
            , std::pair<std::function<Code(const Code&)>, size_t> hash) {
    auto hash_algorithm = hash.first;
    auto chunk_size = hash.second;

    constexpr uint8_t ipad{0x36};
    constexpr uint8_t opad{0x5c};

    auto padded_key = key;
    if (padded_key.value().size() > chunk_size) {
      padded_key = hash_algorithm(padded_key);
    } else {
      padded_key += Code(ByteString(
          chunk_size - padded_key.value().size(), 0x00));
    }

    auto ipad_key = XOR(padded_key, ipad);
    auto opad_key = XOR(padded_key, opad);

    return hash_algorithm(opad_key
                          + hash_algorithm(ipad_key + message));
  }
}