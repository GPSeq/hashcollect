// SPDX-License-Identifier: MIT
// Implements the SDBM 32-bit hash.
#include "hashlab/hash_sdbm.hpp"

namespace hashlab {

// Computes the SDBM 32-bit hash for the provided message.
// Example:
//   auto result = hashlab::sdbm_32(msg);
// High level: Computes an SDBM 32-bit hash over the input bytes.
std::uint32_t sdbm_32(bytespan msg) noexcept {
  std::uint32_t h = 0;
  for (std::byte b : msg) {
    h = static_cast<std::uint32_t>(static_cast<std::uint8_t>(b)) + (h << 6) + (h << 16) - h;
  }
  return h;
}

}
