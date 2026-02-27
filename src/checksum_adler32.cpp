// SPDX-License-Identifier: MIT
// Implements Adler-32 checksum.
#include "hashlab/checksum_adler32.hpp"

namespace hashlab {

    
// Computes the Adler-32 checksum for the provided message.
// Example:
//   auto result = hashlab::adler32(msg);
// High level: Computes an Adler-32 checksum over the input bytes.
std::uint32_t adler32(bytespan msg) noexcept {
  constexpr std::uint32_t MOD = 65521u;
  std::uint32_t a = 1, b = 0;
  for (std::byte x : msg) {
    a += static_cast<std::uint8_t>(x);
    if (a >= MOD) a -= MOD;
    b += a;
    b %= MOD;
  }
  return (b << 16) | a;
}

}
