// SPDX-License-Identifier: MIT
// Implements MurmurHash3 x86 32-bit variant.
#include "hashlab/hash_murmur3_x86_32.hpp"

namespace hashlab {

// Rotates x left by r bits.
// High level: Rotates a 32-bit value left to mix bits.
static inline std::uint32_t rotl32(std::uint32_t x, int r) noexcept {
  return (x << r) | (x >> (32 - r));
}
// Finalization mix for 32-bit state.
// High level: Finalizes 32-bit state by avalanching bits.
static inline std::uint32_t fmix32(std::uint32_t h) noexcept {
  h ^= h >> 16;
  h *= 0x85ebca6bU;
  h ^= h >> 13;
  h *= 0xc2b2ae35U;
  h ^= h >> 16;
  return h;
}

// Computes the MurmurHash3 x86 32-bit hash for the provided message and seed.
// Example:
//   auto result = hashlab::murmur3_x86_32(msg, seed);
// High level: Computes a 32-bit MurmurHash3 (x86) for the input bytes.
std::uint32_t murmur3_x86_32(bytespan msg, std::uint32_t seed) noexcept {
  const std::uint8_t* data = reinterpret_cast<const std::uint8_t*>(msg.data());
  const int len = static_cast<int>(msg.size());
  const int nblocks = len / 4;

  std::uint32_t h1 = seed;
  constexpr std::uint32_t c1 = 0xcc9e2d51U;
  constexpr std::uint32_t c2 = 0x1b873593U;

  for (int i = 0; i < nblocks; ++i) {
    std::uint32_t k1 = load_le32(data + i * 4);
    k1 *= c1;
    k1 = rotl32(k1, 15);
    k1 *= c2;

    h1 ^= k1;
    h1 = rotl32(h1, 13);
    h1 = h1 * 5U + 0xe6546b64U;
  }

  const std::uint8_t* tail = data + nblocks * 4;
  std::uint32_t k1 = 0;
  switch (len & 3) {
    case 3: k1 ^= static_cast<std::uint32_t>(tail[2]) << 16; [[fallthrough]];
    case 2: k1 ^= static_cast<std::uint32_t>(tail[1]) << 8;  [[fallthrough]];
    case 1: k1 ^= static_cast<std::uint32_t>(tail[0]);
            k1 *= c1; k1 = rotl32(k1, 15); k1 *= c2; h1 ^= k1;
            break;
    default: break;
  }

  h1 ^= static_cast<std::uint32_t>(len);
  return fmix32(h1);
}

}
