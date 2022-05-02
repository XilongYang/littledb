// Created by Xilong Yang on 2022-05-02.
//

#include "littledb.h"

#include <random>


namespace littledb {
namespace {
Byte RandomByte() {
  static std::default_random_engine e(time(0));
  return e() % 256;
}
}
RequestProtocol::RequestProtocol(const Code &user_token, Byte opt
                               , const Code &data_key
                               , const Code &data)
  : user_token_(user_token), opt_(opt)
    , data_key_(data_key), data_(data)
    , data_length_(data.value().length()){
  for (int i = 0; i < 7; ++i) {
    padding_[i] = RandomByte();
  }
  auto hmac_key = Code(ByteString{opt_} + ByteString(padding_, 7));
  auto hmac_msg = user_token_ + hmac_key
                  + Code::BaseToCode(data_length_) + data_key_ + data_;
  hmac_ = Code({Hmac(hmac_msg, hmac_key).value()
                    , 0, 16});
}

Code RequestProtocol::Encode() {
  auto hmac_key = Code(ByteString{opt_} + ByteString(padding_, 7));
  auto header = Rsa1024(user_token_ + hmac_key
                        + Code::BaseToCode(data_length_)
                            , Code(), Code());
  return header + data_key_ + data_ + hmac_;
}

void RequestProtocol::Decode(const Code &code) {
  // Minimal length: header:1024bit + hmac:128bit = 144Byte
  if (code.value().length() < 144) {
    throw DecodeError("RequestProtocol Decode Error.");
  }
  size_t cur_pos = 0;
  auto header = Code({code.value(), cur_pos, 128});
  cur_pos += 128;
  header = Rsa1024(header, Code(), Code());
  // RSA is still fake now, padding size = 96
  user_token_ = Code({header.value(), 96, 16});
  opt_ = header.value()[112];
  for (int i = 0; i < 7; ++i) {
    padding_[i] = header.value()[113 + i];
  }
  auto p = reinterpret_cast<Byte*>(&data_length_);
  for (int i = 0; i < 8; ++i) {
    p[i] = header.value()[120 + i];
  }
  if (data_length_ != 0) {
    data_key_ = Code({code.value(), cur_pos, 16});
    cur_pos += 16;
    data_ = Code({code.value(), cur_pos, data_length_});
    cur_pos += data_length_;
  }
  hmac_ = Code({code.value(), cur_pos, 16});
}

Code RequestProtocol::UserToken() const {
  return user_token_;
}

Byte RequestProtocol::Option() const {
  return opt_;
}

size_t RequestProtocol::DataLength() const {
  return data_length_;
}

Code RequestProtocol::DataKey() const {
  return data_key_;
}

Code RequestProtocol::Data() const {
  return data_;
}

Code RequestProtocol::HmacKey() const {
  return Code(ByteString{opt_} + ByteString(padding_, 7));
}

Code RequestProtocol::HmacCode() const {
  return hmac_;
}

}
