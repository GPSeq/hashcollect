// SPDX-License-Identifier: MIT
// Implements a 64-bit Rabin-Karp-style rolling hash interface.
#include "hashlab/rolling_hash.hpp"

namespace hashlab {

// Computes a polynomial rolling hash for the provided message and base.
// Example:
//   auto result = hashlab::rolling_hash(msg, base);
// High level: Computes a rolling Rabin-Karp-style hash over the input bytes.
std::uint64_t rolling_hash(bytespan msg, std::uint64_t base) noexcept {
  std::uint64_t h = 0;
  for (std::byte bb : msg) {
    h = h * base + static_cast<std::uint8_t>(bb);
  }
  return h;
}

}
