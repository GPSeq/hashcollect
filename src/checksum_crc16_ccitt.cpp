// SPDX-License-Identifier: MIT
// Implements CRC-16/CCITT-FALSE.
#include "hashlab/checksum_crc16_ccitt.hpp"

namespace hashlab {

std::uint16_t crc16_ccitt(bytespan msg, std::uint16_t initial) noexcept {
  std::uint16_t crc = initial;
  for (std::byte b : msg) {
    crc ^= static_cast<std::uint16_t>(static_cast<std::uint8_t>(b)) << 8;
    for (int bit = 0; bit < 8; ++bit) {
      crc = (crc & 0x8000u) ? static_cast<std::uint16_t>((crc << 1) ^ 0x1021u)
                            : static_cast<std::uint16_t>(crc << 1);
    }
  }
  return crc;
}

}
