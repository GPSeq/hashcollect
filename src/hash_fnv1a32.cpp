// SPDX-License-Identifier: MIT
#include "hashlab/hash_fnv1a32.hpp"

namespace hashlab {

std::uint32_t fnv1a32(bytespan msg) noexcept {

  constexpr std::uint32_t kOffset = 2166136261u;
  constexpr std::uint32_t kPrime  = 16777619u;
  std::uint32_t h = kOffset;
  for (std::byte b : msg) {
    h ^= static_cast<std::uint8_t>(b);
    h *= kPrime;
  }
  return h;
}

}
