// SPDX-License-Identifier: MIT
#include "hashlab/checksum_adler32.hpp"

namespace hashlab {

    
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
