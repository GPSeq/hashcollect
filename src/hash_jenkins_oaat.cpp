// SPDX-License-Identifier: MIT
#include "hashlab/hash_jenkins_oaat.hpp"

namespace hashlab {

std::uint32_t jenkins_oaat32(bytespan msg) noexcept {
  std::uint32_t h = 0;
  for (std::byte b : msg) {
    h += static_cast<std::uint8_t>(b);
    h += (h << 10);
    h ^= (h >> 6);
  }
  h += (h << 3);
  h ^= (h >> 11);
  h += (h << 15);
  return h;
}

}
