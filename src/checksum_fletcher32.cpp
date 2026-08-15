// SPDX-License-Identifier: MIT
// Implements Fletcher-32.
#include "hashlab/checksum_fletcher32.hpp"

namespace hashlab {

std::uint32_t fletcher32(bytespan msg) noexcept {
  std::uint32_t sum1 = 0xffffu;
  std::uint32_t sum2 = 0xffffu;
  const auto* p = reinterpret_cast<const std::uint8_t*>(msg.data());
  std::size_t len = msg.size();

  while (len > 1) {
    std::size_t words = len / 2;
    if (words > 359) words = 359;
    len -= words * 2;
    for (std::size_t i = 0; i < words; ++i) {
      const std::uint16_t word = static_cast<std::uint16_t>((p[0] << 8) | p[1]);
      p += 2;
      sum1 += word;
      sum2 += sum1;
    }
    sum1 = (sum1 & 0xffffu) + (sum1 >> 16);
    sum2 = (sum2 & 0xffffu) + (sum2 >> 16);
  }

  if (len != 0) {
    sum1 += static_cast<std::uint16_t>(*p) << 8;
    sum2 += sum1;
    sum1 = (sum1 & 0xffffu) + (sum1 >> 16);
    sum2 = (sum2 & 0xffffu) + (sum2 >> 16);
  }

  sum1 = (sum1 & 0xffffu) + (sum1 >> 16);
  sum2 = (sum2 & 0xffffu) + (sum2 >> 16);
  return (sum2 << 16) | sum1;
}

}
