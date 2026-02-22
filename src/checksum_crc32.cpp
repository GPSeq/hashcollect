// SPDX-License-Identifier: MIT
#include "hashlab/checksum_crc32.hpp"

#include <array>

namespace hashlab {

static constexpr std::uint32_t POLY = 0xEDB88320u;

static constexpr std::array<std::uint32_t, 256> make_table() {
  std::array<std::uint32_t, 256> t{};
  for (std::uint32_t i = 0; i < 256; ++i) {
    std::uint32_t c = i;
    for (int k = 0; k < 8; ++k) c = (c & 1u) ? (POLY ^ (c >> 1)) : (c >> 1);
    t[i] = c;
  }
  return t;
}
static constexpr auto T = make_table();

std::uint32_t crc32_ieee(bytespan msg) noexcept {
  std::uint32_t crc = 0xFFFFFFFFu;
  for (std::byte b : msg) {
    std::uint8_t x = static_cast<std::uint8_t>(b);
    crc = T[(crc ^ x) & 0xFFu] ^ (crc >> 8);
  }
  return crc ^ 0xFFFFFFFFu;
}

}
