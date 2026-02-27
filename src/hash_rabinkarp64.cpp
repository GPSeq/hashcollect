// SPDX-License-Identifier: MIT
// Implements a 64-bit Rabin-Karp rolling hash.
#include "hashlab/hash_rabinkarp64.hpp"

namespace hashlab {

// Computes the Rabin-Karp rolling hash for the provided message and base.
// Example:
//   auto result = hashlab::rabinkarp64(msg, base);
// High level: Computes a rolling Rabin-Karp hash over the input bytes.
std::uint64_t rabinkarp64(bytespan msg, std::uint64_t base) noexcept {
  std::uint64_t h = 0;
  for (std::byte bb : msg) {
    h = h * base + static_cast<std::uint8_t>(bb);
  }
  return h;
}

}
