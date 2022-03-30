// Created by Xilong Yang on 2022-03-11.
//

#include "little_crypt.h"

#include <array>
#include <cstdint>

namespace {
  // 8 initialized values of hash function.
  const std::array<uint32_t, 8> kHashInit =
      {0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a
      ,0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19};

  // 64 constant of hash function
  const std::array<uint32_t, 64> kK =
      {0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b
      ,0x59f111f1, 0x923f82a4, 0xab1c5ed5, 0xd807aa98, 0x12835b01
      ,0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7
      ,0xc19bf174, 0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc
      ,0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da, 0x983e5152
      ,0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147
      ,0x06ca6351, 0x14292967, 0x27b70a85, 0x2e1b2138, 0x4d2c6dfc
      ,0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85
      ,0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819
      ,0xd6990624, 0xf40e3585, 0x106aa070, 0x19a4c116, 0x1e376c08
      ,0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f
      ,0x682e6ff3, 0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208
      ,0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2};

  // Rotate Right
  template<typename T>
  inline T CROR(const T &input, size_t bits) {
    size_t size = sizeof(T) * 8;
    return ((input >> bits) | (input << (size - bits)));
  }

  // Some function defined by sha256
  inline uint32_t S0(uint32_t x) {
      return CROR(x, 7)
           ^ CROR(x, 18)
           ^ (x >> 3);
  }

  inline uint32_t S1(uint32_t x) {
      return CROR(x, 17)
           ^ CROR(x, 19)
           ^ (x >> 10);
  }

  inline uint32_t EP0(uint32_t x) {
      return CROR(x, 2)
           ^ CROR(x, 13)
           ^ CROR(x, 22);
  }

  inline uint32_t EP1(uint32_t x) {
      return CROR(x, 6)
           ^ CROR(x, 11)
           ^ CROR(x, 25);
  }

  inline uint32_t CH(uint32_t x, uint32_t y, uint32_t z) {
          return ((x & y) ^ ((~x) & z));
  }

  inline uint32_t MAJ(uint32_t x, uint32_t y, uint32_t z) {
      return ((x & y) ^ (x & z) ^ (y & z));
  }

  constexpr size_t chunk_size = 64;
  constexpr size_t target_size = 56;
  constexpr size_t word_total = 64;

  little_crypt::Code Preprocess(const little_crypt::Code& message) {
    auto msg = message;
    msg += little_crypt::Code(std::string(1, static_cast<char>(0x80)));
    size_t need_byte = target_size - msg.value().size() % chunk_size;
    if (need_byte < 0)
      need_byte += 64;

    msg += little_crypt::Code(std::string(need_byte, 0x00));

    uint64_t length = little_crypt::BigEndian(
        message.value().size() * 8);
    msg += little_crypt::Code::BaseToCode(length);
    return msg;
  }

  std::array<uint32_t, word_total>
  GenerateWords(const little_crypt::Code& msg, size_t pos) {
    std::array<uint32_t, word_total> words{};
    constexpr size_t word_size = 4;
    constexpr size_t word_per_chunk = chunk_size / word_size;
    for (size_t i = 0; i < word_per_chunk; ++i) {
      uint32_t value = *reinterpret_cast<const uint32_t *>(msg.value().c_str()
                                                           + pos
                                                           + word_size * i);
      words[i] = little_crypt::LocalEndian(value);
    }
    for (size_t i = word_per_chunk; i < word_total; ++i) {
      words[i] = words[i - 16] + S0(words[i - 15]) + words[i - 7]
                 + S1(words[i - 2]);
    }
    return words;
  }

  std::array<uint32_t, 8>
  HashEvolution(const std::array<uint32_t, word_total>& words
                , const std::array<uint32_t, 8>& hash) {
    auto hash_raw = hash;
    for (size_t i = 0; i < word_total; ++i) {
      auto t1 = hash_raw[7] + EP1(hash_raw[4])
                + CH(hash_raw[4], hash_raw[5], hash_raw[6])
                + kK[i] + words[i];
      auto t2 = EP0(hash_raw[0])
                + MAJ(hash_raw[0], hash_raw[1], hash_raw[2]);

      for (size_t j = 7; j > 0; --j) {
        hash_raw[j] = hash_raw[j - 1];
        if (j == 4) {
          hash_raw[j] += t1;
        }
      }
      hash_raw[0] = t1 + t2;
    } // for word_total
    return hash_raw;
  }

  little_crypt::Code HashCombine(const std::array<uint32_t , 8>& hash_array) {
    little_crypt::Code result;
    for (auto v : hash_array) {
      uint32_t value = little_crypt::BigEndian(v);
      result += little_crypt::Code::BaseToCode(value);
    }
    return result;
  }
}

namespace little_crypt {
  Code Sha256(const Code& message) {
    auto msg = message;
    msg = Preprocess(msg);
    // Divide to chunk and calculate hash value.
    auto chunk_count = msg.value().size() / chunk_size;
    auto hash = kHashInit;
    for (size_t i = 0; i < chunk_count; ++i) {
      size_t pos = i * chunk_size;
      auto words = GenerateWords(msg, pos);
      auto hash_raw = HashEvolution(words, hash);
      for (size_t j = 0; j < hash.size(); ++j)
        hash[j] += hash_raw[j];
    } // for chunk_count
    return HashCombine(hash);
  } // Sha256
}  // namespace little_crypt
