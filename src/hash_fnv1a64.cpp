// SPDX-License-Identifier: MIT
// Implements the 64-bit FNV-1a hash.
#include "hashlab/hash_fnv1a64.hpp"

namespace hashlab {

// Computes the 64-bit FNV-1a hash for the provided message.
// Example:
//   auto result = hashlab::fnv1a64(msg);
// High level: Computes a 64-bit FNV-1a hash over the input bytes.
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
