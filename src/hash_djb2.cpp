// SPDX-License-Identifier: MIT
// Implements the DJB2 32-bit hash.
#include "hashlab/hash_djb2.hpp"

namespace hashlab {

// Computes the DJB2 32-bit hash for the provided message.
// Example:
//   auto result = hashlab::djb2_32(msg);
// High level: Computes a DJB2 32-bit hash over the input bytes.
std::uint32_t djb2_32(bytespan msg) noexcept {
  std::uint32_t h = 5381u;
  for (std::byte b : msg) {
    h = ((h << 5) + h) + static_cast<std::uint8_t>(b); // h*33 + c
  }
  return h;
}

}
