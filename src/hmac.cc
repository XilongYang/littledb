// Created by Xilong Yang on 2022-03-20.
//

#include "little_crypt.h"

namespace {
  little_crypt::Code XOR(const little_crypt::Code& code, uint8_t pad) {
    auto str = code.value();
    for (auto &c : str) {
      c ^= pad;
    }
    return little_crypt::Code(str);
  }
}

namespace little_crypt {
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
      padded_key += Code(std::string(
          chunk_size - padded_key.value().size(), 0x00));
    }

    auto ipad_key = XOR(padded_key, ipad);
    auto opad_key = XOR(padded_key, opad);

    return hash_algorithm(opad_key
                          + hash_algorithm(ipad_key + message));
  }
}