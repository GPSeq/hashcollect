// SPDX-License-Identifier: MIT
// Implements compact XXH3-family 64-bit and 128-bit hashes.
#include "hashlab/hash_xxh3.hpp"

namespace hashlab {

static constexpr std::uint64_t XXH_PRIME64_1 = 0x9E3779B185EBCA87ULL;
static constexpr std::uint64_t XXH_PRIME64_2 = 0xC2B2AE3D27D4EB4FULL;
static constexpr std::uint64_t XXH_PRIME64_3 = 0x165667B19E3779F9ULL;
static constexpr std::uint64_t XXH_PRIME64_4 = 0x85EBCA77C2B2AE63ULL;
static constexpr std::uint64_t XXH_PRIME64_5 = 0x27D4EB2F165667C5ULL;

static constexpr std::uint8_t XXH3_SECRET[136] = {
  0xb8,0xfe,0x6c,0x39,0x23,0xa4,0x4b,0xbe,0x7c,0x01,0x81,0x2c,0xf7,0x21,0xad,0x1c,
  0xde,0xd4,0x6d,0xe9,0x83,0x90,0x97,0xdb,0x72,0x40,0xa4,0xa4,0xb7,0xb3,0x67,0x1f,
  0xcb,0x79,0xe6,0x4e,0xcc,0xc0,0xe5,0x78,0x82,0x5a,0xd0,0x7d,0xcc,0xff,0x72,0x21,
  0xb8,0x08,0x46,0x74,0xf7,0x43,0x24,0x8e,0xe0,0x35,0x90,0xe6,0x81,0x3a,0x26,0x4c,
  0x3c,0x28,0x52,0xbb,0x91,0xc3,0x00,0xcb,0x88,0xd0,0x65,0x8b,0x1b,0x53,0x2e,0xa3,
  0x71,0x64,0x48,0x97,0xa2,0x0d,0xf9,0x4e,0x38,0x19,0xef,0x46,0xa9,0xde,0xac,0xd8,
  0xa8,0xfa,0x76,0x3f,0xe3,0x9c,0x34,0x3f,0xf9,0xdc,0xbb,0xc7,0xc7,0x0e,0x1c,0x2c,
  0x87,0x96,0x68,0x1b,0xe7,0x93,0xf3,0x9b,0xd6,0x4b,0x6b,0xe4,0xe6,0x00,0x41,0x5e,
  0x7b,0xb6,0x0d,0xa5,0xe2,0xe1,0x2a,0xf3
};

static inline std::uint64_t rotl64_xxh3(std::uint64_t x, int r) noexcept {
  return (x << r) | (x >> (64 - r));
}

static inline std::uint64_t avalanche64(std::uint64_t h) noexcept {
  h ^= h >> 37;
  h *= 0x165667919E3779F9ULL;
  h ^= h >> 32;
  return h;
}

static inline std::uint64_t rrmxmx(std::uint64_t h, std::uint64_t len) noexcept {
  h ^= rotl64_xxh3(h, 49) ^ rotl64_xxh3(h, 24);
  h *= 0x9FB21C651E98DF25ULL;
  h ^= (h >> 35) + len;
  h *= 0x9FB21C651E98DF25ULL;
  h ^= h >> 28;
  return h;
}

static inline std::uint64_t fold_mul64(std::uint64_t lhs, std::uint64_t rhs) noexcept {
  const std::uint64_t lhs0 = static_cast<std::uint32_t>(lhs);
  const std::uint64_t lhs1 = lhs >> 32;
  const std::uint64_t rhs0 = static_cast<std::uint32_t>(rhs);
  const std::uint64_t rhs1 = rhs >> 32;
  const std::uint64_t p00 = lhs0 * rhs0;
  const std::uint64_t p01 = lhs0 * rhs1;
  const std::uint64_t p10 = lhs1 * rhs0;
  const std::uint64_t p11 = lhs1 * rhs1;
  const std::uint64_t middle = (p00 >> 32) + static_cast<std::uint32_t>(p01) + static_cast<std::uint32_t>(p10);
  const std::uint64_t low = (p00 & 0xffffffffULL) | (middle << 32);
  const std::uint64_t high = p11 + (p01 >> 32) + (p10 >> 32) + (middle >> 32);
  return low ^ high;
}

static inline std::uint32_t bswap32(std::uint32_t v) noexcept {
  return (v >> 24) |
         ((v >> 8) & 0x0000ff00u) |
         ((v << 8) & 0x00ff0000u) |
         (v << 24);
}

static inline std::uint64_t bswap64(std::uint64_t v) noexcept {
  return (v >> 56) |
         ((v >> 40) & 0x000000000000ff00ULL) |
         ((v >> 24) & 0x0000000000ff0000ULL) |
         ((v >> 8) & 0x00000000ff000000ULL) |
         ((v << 8) & 0x000000ff00000000ULL) |
         ((v << 24) & 0x0000ff0000000000ULL) |
         ((v << 40) & 0x00ff000000000000ULL) |
         (v << 56);
}

static inline std::uint64_t mix16(const std::uint8_t* input, std::size_t secret_offset, std::uint64_t seed) noexcept {
  const auto* secret = XXH3_SECRET + secret_offset;
  const std::uint64_t lo = load_le64(input) ^ (load_le64(secret) + seed);
  const std::uint64_t hi = load_le64(input + 8) ^ (load_le64(secret + 8) - seed);
  return fold_mul64(lo, hi);
}

static std::uint64_t xxh3_len_1to3(const std::uint8_t* p, std::size_t len, std::uint64_t seed) noexcept {
  const std::uint32_t c1 = p[0];
  const std::uint32_t c2 = p[len >> 1];
  const std::uint32_t c3 = p[len - 1];
  const std::uint32_t combined = (c1 << 16) | (c2 << 24) | c3 | (static_cast<std::uint32_t>(len) << 8);
  const std::uint64_t bitflip = (static_cast<std::uint64_t>(load_le32(XXH3_SECRET)) ^
                                 static_cast<std::uint64_t>(load_le32(XXH3_SECRET + 4))) + seed;
  return avalanche64(static_cast<std::uint64_t>(combined) ^ bitflip);
}

static std::uint64_t xxh3_len_4to8(const std::uint8_t* p, std::size_t len, std::uint64_t seed) noexcept {
  seed ^= static_cast<std::uint64_t>(bswap32(static_cast<std::uint32_t>(seed))) << 32;
  const std::uint64_t input = static_cast<std::uint64_t>(load_le32(p)) |
                              (static_cast<std::uint64_t>(load_le32(p + len - 4)) << 32);
  const std::uint64_t bitflip = (load_le64(XXH3_SECRET + 8) ^ load_le64(XXH3_SECRET + 16)) - seed;
  return rrmxmx(input ^ bitflip, len);
}

static std::uint64_t xxh3_len_9to16(const std::uint8_t* p, std::size_t len, std::uint64_t seed) noexcept {
  const std::uint64_t bitflip1 = (load_le64(XXH3_SECRET + 24) ^ load_le64(XXH3_SECRET + 32)) + seed;
  const std::uint64_t bitflip2 = (load_le64(XXH3_SECRET + 40) ^ load_le64(XXH3_SECRET + 48)) - seed;
  const std::uint64_t lo = load_le64(p) ^ bitflip1;
  const std::uint64_t hi = load_le64(p + len - 8) ^ bitflip2;
  return avalanche64(static_cast<std::uint64_t>(len) + bswap64(lo) + hi + fold_mul64(lo, hi));
}

static std::uint64_t xxh3_len_17to128(const std::uint8_t* p, std::size_t len, std::uint64_t seed) noexcept {
  std::uint64_t acc = static_cast<std::uint64_t>(len) * XXH_PRIME64_1;
  if (len > 32) {
    if (len > 64) {
      if (len > 96) {
        acc += mix16(p + 48, 96, seed);
        acc += mix16(p + len - 64, 112, seed);
      }
      acc += mix16(p + 32, 64, seed);
      acc += mix16(p + len - 48, 80, seed);
    }
    acc += mix16(p + 16, 32, seed);
    acc += mix16(p + len - 32, 48, seed);
  }
  acc += mix16(p, 0, seed);
  acc += mix16(p + len - 16, 16, seed);
  return avalanche64(acc);
}

static std::uint64_t xxh3_len_129to240(const std::uint8_t* p, std::size_t len, std::uint64_t seed) noexcept {
  std::uint64_t acc = static_cast<std::uint64_t>(len) * XXH_PRIME64_1;
  const std::size_t rounds = len / 16;
  for (std::size_t i = 0; i < 8; ++i) {
    acc += mix16(p + i * 16, i * 16, seed);
  }
  acc = avalanche64(acc);
  for (std::size_t i = 8; i < rounds; ++i) {
    acc += mix16(p + i * 16, 3 + (i - 8) * 16, seed);
  }
  acc += mix16(p + len - 16, sizeof(XXH3_SECRET) - 17, seed);
  return avalanche64(acc);
}

std::uint64_t xxh3_64(bytespan msg, std::uint64_t seed) noexcept {
  const auto* p = reinterpret_cast<const std::uint8_t*>(msg.data());
  const std::size_t len = msg.size();

  if (len == 0) {
    const std::uint64_t bitflip = load_le64(XXH3_SECRET + 56) ^ load_le64(XXH3_SECRET + 64);
    return avalanche64(seed ^ bitflip);
  }
  if (len <= 3) return xxh3_len_1to3(p, len, seed);
  if (len <= 8) return xxh3_len_4to8(p, len, seed);
  if (len <= 16) return xxh3_len_9to16(p, len, seed);
  if (len <= 128) return xxh3_len_17to128(p, len, seed);
  if (len <= 240) return xxh3_len_129to240(p, len, seed);

  std::uint64_t acc = static_cast<std::uint64_t>(len) * XXH_PRIME64_1;
  const std::size_t stripes = len / 16;
  for (std::size_t i = 0; i < stripes; ++i) {
    acc += mix16(p + i * 16, (i * 16) % (sizeof(XXH3_SECRET) - 16), seed);
    acc = rotl64_xxh3(acc, 27) * XXH_PRIME64_1 + XXH_PRIME64_4;
  }
  acc += mix16(p + len - 16, sizeof(XXH3_SECRET) - 16, seed ^ XXH_PRIME64_2);
  return avalanche64(acc);
}

std::array<std::uint8_t, 16> xxh3_128(bytespan msg, std::uint64_t seed) noexcept {
  const std::uint64_t low = xxh3_64(msg, seed);
  const std::uint64_t high = xxh3_64(msg, seed ^ XXH_PRIME64_2 ^ XXH_PRIME64_3 ^ XXH_PRIME64_5);
  std::array<std::uint8_t, 16> out{};
  store_le64(out.data(), low);
  store_le64(out.data() + 8, high);
  return out;
}

}
