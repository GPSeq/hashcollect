// SPDX-License-Identifier: MIT
#include "hashlab/hash_murmur3_x64_128.hpp"

namespace hashlab {

static inline std::uint64_t rotl64(std::uint64_t x, int r) noexcept {
  return (x << r) | (x >> (64 - r));
}
static inline std::uint64_t fmix64(std::uint64_t k) noexcept {
  k ^= k >> 33;
  k *= 0xff51afd7ed558ccdULL;
  k ^= k >> 33;
  k *= 0xc4ceb9fe1a85ec53ULL;
  k ^= k >> 33;
  return k;
}

std::array<std::uint8_t, 16> murmur3_x64_128(bytespan msg, std::uint32_t seed) noexcept {
  const std::uint8_t* data = reinterpret_cast<const std::uint8_t*>(msg.data());
  const int len = static_cast<int>(msg.size());
  const int nblocks = len / 16;

  std::uint64_t h1 = seed;
  std::uint64_t h2 = seed;

  constexpr std::uint64_t c1 = 0x87c37b91114253d5ULL;
  constexpr std::uint64_t c2 = 0x4cf5ad432745937fULL;

  for (int i = 0; i < nblocks; ++i) {
    const std::uint8_t* block = data + i * 16;
    std::uint64_t k1 = load_le64(block + 0);
    std::uint64_t k2 = load_le64(block + 8);

    k1 *= c1; k1 = rotl64(k1, 31); k1 *= c2; h1 ^= k1;
    h1 = rotl64(h1, 27); h1 += h2; h1 = h1 * 5 + 0x52dce729;

    k2 *= c2; k2 = rotl64(k2, 33); k2 *= c1; h2 ^= k2;
    h2 = rotl64(h2, 31); h2 += h1; h2 = h2 * 5 + 0x38495ab5;
  }

  const std::uint8_t* tail = data + nblocks * 16;
  std::uint64_t k1 = 0, k2 = 0;

  switch (len & 15) {
    case 15: k2 ^= (std::uint64_t)tail[14] << 48; [[fallthrough]];
    case 14: k2 ^= (std::uint64_t)tail[13] << 40; [[fallthrough]];
    case 13: k2 ^= (std::uint64_t)tail[12] << 32; [[fallthrough]];
    case 12: k2 ^= (std::uint64_t)tail[11] << 24; [[fallthrough]];
    case 11: k2 ^= (std::uint64_t)tail[10] << 16; [[fallthrough]];
    case 10: k2 ^= (std::uint64_t)tail[9]  << 8;  [[fallthrough]];
    case  9: k2 ^= (std::uint64_t)tail[8]  << 0;
             k2 *= c2; k2 = rotl64(k2, 33); k2 *= c1; h2 ^= k2;
             [[fallthrough]];
    case  8: k1 ^= (std::uint64_t)tail[7]  << 56; [[fallthrough]];
    case  7: k1 ^= (std::uint64_t)tail[6]  << 48; [[fallthrough]];
    case  6: k1 ^= (std::uint64_t)tail[5]  << 40; [[fallthrough]];
    case  5: k1 ^= (std::uint64_t)tail[4]  << 32; [[fallthrough]];
    case  4: k1 ^= (std::uint64_t)tail[3]  << 24; [[fallthrough]];
    case  3: k1 ^= (std::uint64_t)tail[2]  << 16; [[fallthrough]];
    case  2: k1 ^= (std::uint64_t)tail[1]  << 8;  [[fallthrough]];
    case  1: k1 ^= (std::uint64_t)tail[0]  << 0;
             k1 *= c1; k1 = rotl64(k1, 31); k1 *= c2; h1 ^= k1;
             break;
    default: break;
  }

  h1 ^= (std::uint64_t)len;
  h2 ^= (std::uint64_t)len;

  h1 += h2;
  h2 += h1;

  h1 = fmix64(h1);
  h2 = fmix64(h2);

  h1 += h2;
  h2 += h1;

  std::array<std::uint8_t, 16> out{};
  store_le64(out.data() + 0, h1);
  store_le64(out.data() + 8, h2);
  return out;
}

}
