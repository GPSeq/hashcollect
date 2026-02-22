// SPDX-License-Identifier: MIT
#include "hashlab/hash_rabinkarp64.hpp"

namespace hashlab {

std::uint64_t rabinkarp64(bytespan msg, std::uint64_t base) noexcept {
  std::uint64_t h = 0;
  for (std::byte bb : msg) {
    h = h * base + static_cast<std::uint8_t>(bb);
  }
  return h;
}

}
