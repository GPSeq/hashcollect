// SPDX-License-Identifier: MIT
// Implements SipHash-2-4 (64-bit keyed hash).
#include "hashlab/hash_siphash24.hpp"

namespace hashlab {

// Rotates x left by b bits.
// High level: Rotates a 64-bit value left to mix bits.
static inline std::uint64_t rotl64(std::uint64_t x, int b) noexcept { return (x << b) | (x >> (64 - b)); }
// Executes one SipHash compression round.
// High level: Runs one SipHash compression round on the state.
static inline void sip_round(std::uint64_t& v0, std::uint64_t& v1, std::uint64_t& v2, std::uint64_t& v3) noexcept {
  v0 += v1; v1 = rotl64(v1, 13); v1 ^= v0; v0 = rotl64(v0, 32);
  v2 += v3; v3 = rotl64(v3, 16); v3 ^= v2;
  v0 += v3; v3 = rotl64(v3, 21); v3 ^= v0;
  v2 += v1; v1 = rotl64(v1, 17); v1 ^= v2; v2 = rotl64(v2, 32);
}

// Computes the SipHash-2-4 of the provided message and 128-bit key.
// Example:
//   auto result = hashlab::siphash24(msg, std::array<std::uint8_t, key);
// High level: Computes a SipHash-2-4 MAC over the input bytes.
std::uint64_t siphash24(bytespan msg, std::array<std::uint8_t, 16> key) noexcept {
  std::uint64_t k0 = load_le64(key.data() + 0);
  std::uint64_t k1 = load_le64(key.data() + 8);

  std::uint64_t v0 = 0x736f6d6570736575ULL ^ k0;
  std::uint64_t v1 = 0x646f72616e646f6dULL ^ k1;
  std::uint64_t v2 = 0x6c7967656e657261ULL ^ k0;
  std::uint64_t v3 = 0x7465646279746573ULL ^ k1;

  const std::uint8_t* p = reinterpret_cast<const std::uint8_t*>(msg.data());
  std::size_t len = msg.size();

  const std::uint8_t* end = p + (len & ~std::size_t(7));
  while (p != end) {
    std::uint64_t m = load_le64(p);
    v3 ^= m;
    sip_round(v0, v1, v2, v3);
    sip_round(v0, v1, v2, v3);
    v0 ^= m;
    p += 8;
  }

  std::uint64_t b = (std::uint64_t)len << 56;
  switch (len & 7) {
    case 7: b |= (std::uint64_t)p[6] << 48; [[fallthrough]];
    case 6: b |= (std::uint64_t)p[5] << 40; [[fallthrough]];
    case 5: b |= (std::uint64_t)p[4] << 32; [[fallthrough]];
    case 4: b |= (std::uint64_t)p[3] << 24; [[fallthrough]];
    case 3: b |= (std::uint64_t)p[2] << 16; [[fallthrough]];
    case 2: b |= (std::uint64_t)p[1] << 8;  [[fallthrough]];
    case 1: b |= (std::uint64_t)p[0] << 0;  [[fallthrough]];
    default: break;
  }

  v3 ^= b;
  sip_round(v0, v1, v2, v3);
  sip_round(v0, v1, v2, v3);
  v0 ^= b;

  v2 ^= 0xff;
  sip_round(v0, v1, v2, v3);
  sip_round(v0, v1, v2, v3);
  sip_round(v0, v1, v2, v3);
  sip_round(v0, v1, v2, v3);

  return v0 ^ v1 ^ v2 ^ v3;
}

}
