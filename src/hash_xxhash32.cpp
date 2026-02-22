// SPDX-License-Identifier: MIT
#include "hashlab/hash_xxhash32.hpp"

namespace hashlab {

static inline std::uint32_t rotl32(std::uint32_t x, int r) noexcept { return (x << r) | (x >> (32 - r)); }

std::uint32_t xxhash32(bytespan msg, std::uint32_t seed) noexcept {
  constexpr std::uint32_t P1 = 0x9E3779B1u;
  constexpr std::uint32_t P2 = 0x85EBCA77u;
  constexpr std::uint32_t P3 = 0xC2B2AE3Du;
  constexpr std::uint32_t P4 = 0x27D4EB2Fu;
  constexpr std::uint32_t P5 = 0x165667B1u;

  const std::uint8_t* p = reinterpret_cast<const std::uint8_t*>(msg.data());
  const std::uint8_t* const bEnd = p + msg.size();

  std::uint32_t h32;
  if (msg.size() >= 16) {
    std::uint32_t v1 = seed + P1 + P2;
    std::uint32_t v2 = seed + P2;
    std::uint32_t v3 = seed + 0;
    std::uint32_t v4 = seed - P1;

    const std::uint8_t* const limit = bEnd - 16;
    do {
      v1 += load_le32(p) * P2; v1 = rotl32(v1, 13); v1 *= P1; p += 4;
      v2 += load_le32(p) * P2; v2 = rotl32(v2, 13); v2 *= P1; p += 4;
      v3 += load_le32(p) * P2; v3 = rotl32(v3, 13); v3 *= P1; p += 4;
      v4 += load_le32(p) * P2; v4 = rotl32(v4, 13); v4 *= P1; p += 4;
    } while (p <= limit);

    h32 = rotl32(v1, 1) + rotl32(v2, 7) + rotl32(v3, 12) + rotl32(v4, 18);
  } else {
    h32 = seed + P5;
  }

  h32 += static_cast<std::uint32_t>(msg.size());

  while (p + 4 <= bEnd) {
    h32 += load_le32(p) * P3;
    h32 = rotl32(h32, 17) * P4;
    p += 4;
  }
  while (p < bEnd) {
    h32 += (*p) * P5;
    h32 = rotl32(h32, 11) * P1;
    ++p;
  }

  h32 ^= h32 >> 15; h32 *= P2;
  h32 ^= h32 >> 13; h32 *= P3;
  h32 ^= h32 >> 16;
  return h32;
}

}
