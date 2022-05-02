// Created by Xilong Yang on 2022-05-02.
//

#include "littledb.h"

namespace littledb {
  ResponseProtocol::ResponseProtocol(size_t status, const Code& hmac_key
                                   , const Code &data)
    : status_(status), data_length_(data.value().length()), data_(data) {
    auto hmac_msg = Code::BaseToCode(status_)
                    + Code::BaseToCode(data_length_) + data_;
    hmac_ = Code({Hmac(hmac_msg, hmac_key).value()
                 , 0, 16});
  }

  Code ResponseProtocol::Encode() {
    return Code::BaseToCode(status_) + Code::BaseToCode(data_length_)
    + data_ + hmac_;
  }

  void ResponseProtocol::Decode(const Code &code) {
    if (code.value().length() < 32) {
      throw DecodeError("ResponseProtocol Decode Error.");
    }

    auto status_ptr_ = reinterpret_cast<Byte*>(&status_);
    auto data_length_ptr_ = reinterpret_cast<Byte*>(&data_length_);
    for (int i = 0; i < 8; ++i) {
      status_ptr_[i] = code.value()[i];
      data_length_ptr_[i] = code.value()[i + 8];
    }
    size_t cur_pos = 16;
    if (data_length_ != 0) {
      data_ = Code({code.value(), cur_pos, data_length_});
      cur_pos += data_length_;
    }
    hmac_ = Code({code.value(), cur_pos, 16});
  }

  size_t ResponseProtocol::Status() const {
    return status_;
  }

  size_t ResponseProtocol::DataLength() const {
    return data_length_;
  }

  Code ResponseProtocol::Data() const {
    return data_;
  }

  Code ResponseProtocol::HmacCode() const {
    return hmac_;
  }
}
