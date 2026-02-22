// SPDX-License-Identifier: MIT
#include "hashlab/hash_fnv1a64.hpp"

namespace hashlab {

std::uint64_t fnv1a64(bytespan msg) noexcept {
  constexpr std::uint64_t kOffset = 14695981039346656037ULL;
  constexpr std::uint64_t kPrime  = 1099511628211ULL;
  std::uint64_t h = kOffset;
  for (std::byte b : msg) {
    h ^= static_cast<std::uint8_t>(b);
    h *= kPrime;
  }
  return h;
}

}
