// SPDX-License-Identifier: MIT
#include "hashlab/checksum_crc64_ecma.hpp"

#include <array>

namespace hashlab {

// Reflected polynomial for CRC64-ECMA:
static constexpr std::uint64_t POLY = 0xC96C5795D7870F42ULL;

static constexpr std::array<std::uint64_t, 256> make_table() {
  std::array<std::uint64_t, 256> t{};
  for (std::uint64_t i = 0; i < 256; ++i) {
    std::uint64_t c = i;
    for (int k = 0; k < 8; ++k) c = (c & 1ULL) ? (POLY ^ (c >> 1)) : (c >> 1);
    t[(std::size_t)i] = c;
  }
  return t;
}
static constexpr auto T = make_table();

std::uint64_t crc64_ecma(bytespan msg) noexcept {
  std::uint64_t crc = 0xFFFFFFFFFFFFFFFFULL;
  for (std::byte b : msg) {
    std::uint8_t x = static_cast<std::uint8_t>(b);
    crc = T[(crc ^ x) & 0xFFULL] ^ (crc >> 8);
  }
  return crc ^ 0xFFFFFFFFFFFFFFFFULL;
}

}
