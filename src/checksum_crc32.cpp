// SPDX-License-Identifier: MIT
// Implements CRC-32 (IEEE) checksum.
#include "hashlab/checksum_crc32.hpp"

#include <array>

namespace hashlab {

// Reflected CRC-32 polynomial for IEEE 802.3.
static constexpr std::uint32_t POLY = 0xEDB88320u;

// Builds the CRC-32 lookup table at compile time.
// High level: Builds a lookup table for fast checksum updates.
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

// Computes the CRC-32/IEEE checksum for the provided message.
// Example:
//   auto result = hashlab::crc32_ieee(msg);
// High level: Computes a CRC-32/IEEE checksum over the input bytes.
std::uint32_t crc32_ieee(bytespan msg) noexcept {
  std::uint32_t crc = 0xFFFFFFFFu;
  for (std::byte b : msg) {
    std::uint8_t x = static_cast<std::uint8_t>(b);
    crc = T[(crc ^ x) & 0xFFu] ^ (crc >> 8);
  }
  return crc ^ 0xFFFFFFFFu;
}

}
