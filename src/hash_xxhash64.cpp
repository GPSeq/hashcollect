// SPDX-License-Identifier: MIT
#include "hashlab/hash_xxhash64.hpp"

namespace hashlab {

static inline std::uint64_t rotl64(std::uint64_t x, int r) noexcept { return (x << r) | (x >> (64 - r)); }
static inline std::uint64_t xxh64_round(std::uint64_t acc, std::uint64_t input) noexcept {
  constexpr std::uint64_t P1 = 11400714785074694791ULL;
  constexpr std::uint64_t P2 = 14029467366897019727ULL;
  acc += input * P2;
  acc = rotl64(acc, 31);
  acc *= P1;
  return acc;
}
static inline std::uint64_t xxh64_merge(std::uint64_t acc, std::uint64_t val) noexcept {
  constexpr std::uint64_t P1 = 11400714785074694791ULL;
  constexpr std::uint64_t P4 = 9650029242287828579ULL;
  acc ^= xxh64_round(0, val);
  acc = acc * P1 + P4;
  return acc;
}

std::uint64_t xxhash64(bytespan msg, std::uint64_t seed) noexcept {
  constexpr std::uint64_t P1 = 11400714785074694791ULL;
  constexpr std::uint64_t P2 = 14029467366897019727ULL;
  constexpr std::uint64_t P3 = 1609587929392839161ULL;
  constexpr std::uint64_t P4 = 9650029242287828579ULL;
  constexpr std::uint64_t P5 = 2870177450012600261ULL;

  const std::uint8_t* p = reinterpret_cast<const std::uint8_t*>(msg.data());
  const std::uint8_t* const bEnd = p + msg.size();

  std::uint64_t h64;
  if (msg.size() >= 32) {
    std::uint64_t v1 = seed + P1 + P2;
    std::uint64_t v2 = seed + P2;
    std::uint64_t v3 = seed + 0;
    std::uint64_t v4 = seed - P1;

    const std::uint8_t* const limit = bEnd - 32;
    do {
      v1 = xxh64_round(v1, load_le64(p)); p += 8;
      v2 = xxh64_round(v2, load_le64(p)); p += 8;
      v3 = xxh64_round(v3, load_le64(p)); p += 8;
      v4 = xxh64_round(v4, load_le64(p)); p += 8;
    } while (p <= limit);

    h64 = rotl64(v1, 1) + rotl64(v2, 7) + rotl64(v3, 12) + rotl64(v4, 18);
    h64 = xxh64_merge(h64, v1);
    h64 = xxh64_merge(h64, v2);
    h64 = xxh64_merge(h64, v3);
    h64 = xxh64_merge(h64, v4);
  } else {
    h64 = seed + P5;
  }

  h64 += static_cast<std::uint64_t>(msg.size());

  while (p + 8 <= bEnd) {
    std::uint64_t k1 = xxh64_round(0, load_le64(p));
    h64 ^= k1;
    h64 = rotl64(h64, 27) * P1 + P4;
    p += 8;
  }

  if (p + 4 <= bEnd) {
    h64 ^= (std::uint64_t)load_le32(p) * P1;
    h64 = rotl64(h64, 23) * P2 + P3;
    p += 4;
  }

  while (p < bEnd) {
    h64 ^= (std::uint64_t)(*p) * P5;
    h64 = rotl64(h64, 11) * P1;
    ++p;
  }

  h64 ^= h64 >> 33; h64 *= P2;
  h64 ^= h64 >> 29; h64 *= P3;
  h64 ^= h64 >> 32;
  return h64;
}

} // namespace hashlab
