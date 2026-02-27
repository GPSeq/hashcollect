// SPDX-License-Identifier: MIT
// Implements a 32-bit buzhash rolling hash.
#include "hashlab/hash_buzhash32.hpp"

namespace hashlab {

// Seed table for byte mixing (first 32 values set explicitly).
static constexpr std::uint32_t R[256] = {
  0x02a65c15u,0x7b1f2a3eu,0x9e3c6f1bu,0x3c4d6f2au,0x11b4d2d7u,0xa0c3e19du,0x5c2a9d47u,0x8f1c3a52u,
  0x6d3e2a19u,0x4a7c1d2eu,0x1f2e3d4cu,0x89ab12cdu,0xf1e2d3c4u,0x13579bdfu,0x2468ace0u,0x0f1e2d3cu,
  0x4b5a6978u,0x98a7b6c5u,0x11223344u,0x55667788u,0x99aabbccu,0xddeeff00u,0xc001d00du,0xdeadbeefu,
  0xabcdef01u,0x10fedcbau,0x76543210u,0xfedcba98u,0x89abcdefu,0x01234567u,0x3f2e1d0cu,0x0c1d2e3fu,
  // fill remainder deterministically with a simple LCG-like progression
};

// Rotates x left by r bits.
// High level: Rotates a 32-bit value left to mix bits.
static inline std::uint32_t rotl32(std::uint32_t x, int r) noexcept { return (x << r) | (x >> (32 - r)); }

// build full table at compile-time
// Returns a deterministic mixing value for a byte.
// High level: Generates a deterministic mix value for a byte.
static inline std::uint32_t rbyte(std::uint8_t b) noexcept {
  if (b < 32) return R[b];
  std::uint32_t x = 0x9e3779b9u * (b + 1u);
  x ^= x >> 16; x *= 0x7feb352du; x ^= x >> 15; x *= 0x846ca68bu; x ^= x >> 16;
  return x;
}

// Computes a 32-bit buzhash for the provided message.
// Example:
//   auto result = hashlab::buzhash32(msg);
// High level: Computes a 32-bit buzhash over the input bytes.
std::uint32_t buzhash32(bytespan msg) noexcept {
  std::uint32_t h = 0;
  for (std::byte bb : msg) {
    std::uint8_t b = static_cast<std::uint8_t>(bb);
    h = rotl32(h, 1) ^ rbyte(b);
  }
  return h;
}

}
