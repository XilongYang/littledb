// Created by Xilong Yang on 2022-03-20.
//

#include "little_crypt.h"

#include <array>
#include <stdexcept>

namespace {
  class State : public little_crypt::CodableInterface {
   public:
    explicit State(const little_crypt::Code& code = little_crypt::Code()) {
      if (code != little_crypt::Code()) {
        State::Decode(code);
      }
    }

    little_crypt::Code Encode() override {
      std::string str(16, 0);
      for (int i = 0; i < 16; ++i) {
        str[i] = value_[i];
      }
      return little_crypt::Code(str);
    }

    void Decode(const little_crypt::Code& code) override {
      if (code.value().size() != 16) {
        throw little_crypt::DecodeError("S-Box can only be constructed from"
            "code of length 16");
      }
      for (int i = 0; i < 16; ++i) {
        value_[i] = code.value()[i];
      }
    }

    bool operator==(const State& state) const {
      return this->value_ == state.value_;
    }

    bool operator!=(const State& state) const {
      return this->value_ != state.value_;
    }

    char& operator[](int index) {
      return value_[index];
    }

    char operator[](int index) const {
      return value_[index];
    }

    State operator^(const State& state) const {
      State result;
      for (int i = 0; i < 16; ++i) {
        result[i] = (*this)[i]^state[i];
      }
      return result;
    }

    void operator^=(const State& state) {
      for (int i = 0; i < 16; ++i) {
        (*this)[i] ^= state[i];
      }
    }

   private:
    std::array<char, 16> value_;
  };
}

namespace little_crypt {
  Code Aes128Encrypt(const Code& plaintext, const Code& key) {
    Code hash_key(Sha256(key).value().substr(0, 16));
    State key_state(hash_key);

    Code padded_text(plaintext.value()
                     + std::string((16 - (plaintext.value().size() % 16))
                                       % 16, 0));

    for (int i = 0; i < padded_text.value().size(); i += 16) {
      Code current_text(padded_text.value().substr(i, 16));
      State text_state(current_text);
      text_state = (text_state^key_state);

    }
    return little_crypt::Code();
  }

  Code Aes128Decrypt(const Code& ciphertext, const Code& key) {
    return little_crypt::Code();
  }
}