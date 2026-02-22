// SPDX-License-Identifier: MIT
#include "hashlab/hash_djb2.hpp"

namespace hashlab {

std::uint32_t djb2_32(bytespan msg) noexcept {
  std::uint32_t h = 5381u;
  for (std::byte b : msg) {
    h = ((h << 5) + h) + static_cast<std::uint8_t>(b); // h*33 + c
  }
  return h;
}

}
