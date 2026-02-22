// SPDX-License-Identifier: MIT
#include "hashlab/hash_sdbm.hpp"

namespace hashlab {

std::uint32_t sdbm_32(bytespan msg) noexcept {
  std::uint32_t h = 0;
  for (std::byte b : msg) {
    h = static_cast<std::uint32_t>(static_cast<std::uint8_t>(b)) + (h << 6) + (h << 16) - h;
  }
  return h;
}

}
