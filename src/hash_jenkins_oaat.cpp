// SPDX-License-Identifier: MIT
// Implements the Jenkins one-at-a-time 32-bit hash.
#include "hashlab/hash_jenkins_oaat.hpp"

namespace hashlab {

// Computes the Jenkins one-at-a-time 32-bit hash for the provided message.
// Example:
//   auto result = hashlab::jenkins_oaat32(msg);
// High level: Computes a Jenkins one-at-a-time hash over the input bytes.
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
