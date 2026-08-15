// SPDX-License-Identifier: MIT
// Implements BLAKE2b-512.
#include "hashlab/hash_blake2b.hpp"

namespace hashlab {

static constexpr std::uint64_t B2B_IV[8] = {
  0x6a09e667f3bcc908ULL,0xbb67ae8584caa73bULL,0x3c6ef372fe94f82bULL,0xa54ff53a5f1d36f1ULL,
  0x510e527fade682d1ULL,0x9b05688c2b3e6c1fULL,0x1f83d9abfb41bd6bULL,0x5be0cd19137e2179ULL
};

static constexpr std::uint8_t B2B_SIGMA[12][16] = {
  {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15},
  {14,10,4,8,9,15,13,6,1,12,0,2,11,7,5,3},
  {11,8,12,0,5,2,15,13,10,14,3,6,7,1,9,4},
  {7,9,3,1,13,12,11,14,2,6,5,10,4,0,15,8},
  {9,0,5,7,2,4,10,15,14,1,11,12,6,8,3,13},
  {2,12,6,10,0,11,8,3,4,13,7,5,15,14,1,9},
  {12,5,1,15,14,13,4,10,0,7,6,3,9,2,8,11},
  {13,11,7,14,12,1,3,9,5,0,15,4,8,6,2,10},
  {6,15,14,9,11,3,0,8,12,2,13,7,1,4,10,5},
  {10,2,8,4,7,6,1,5,15,11,9,14,3,12,13,0},
  {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15},
  {14,10,4,8,9,15,13,6,1,12,0,2,11,7,5,3}
};

static inline std::uint64_t rotr64_b2b(std::uint64_t x, int r) noexcept {
  return (x >> r) | (x << (64 - r));
}

static inline void b2b_g(std::uint64_t v[16], int a, int b, int c, int d, std::uint64_t x, std::uint64_t y) noexcept {
  v[a] = v[a] + v[b] + x;
  v[d] = rotr64_b2b(v[d] ^ v[a], 32);
  v[c] += v[d];
  v[b] = rotr64_b2b(v[b] ^ v[c], 24);
  v[a] = v[a] + v[b] + y;
  v[d] = rotr64_b2b(v[d] ^ v[a], 16);
  v[c] += v[d];
  v[b] = rotr64_b2b(v[b] ^ v[c], 63);
}

static void b2b_compress(std::uint64_t h[8], const std::uint8_t block[128], std::uint64_t t, bool last) noexcept {
  std::uint64_t m[16];
  for (int i = 0; i < 16; ++i) m[i] = load_le64(block + i * 8);

  std::uint64_t v[16];
  for (int i = 0; i < 8; ++i) v[i] = h[i];
  for (int i = 0; i < 8; ++i) v[i + 8] = B2B_IV[i];
  v[12] ^= t;
  if (last) v[14] = ~v[14];

  for (int r = 0; r < 12; ++r) {
    const auto* s = B2B_SIGMA[r];
    b2b_g(v, 0, 4, 8, 12, m[s[0]], m[s[1]]);
    b2b_g(v, 1, 5, 9, 13, m[s[2]], m[s[3]]);
    b2b_g(v, 2, 6, 10, 14, m[s[4]], m[s[5]]);
    b2b_g(v, 3, 7, 11, 15, m[s[6]], m[s[7]]);
    b2b_g(v, 0, 5, 10, 15, m[s[8]], m[s[9]]);
    b2b_g(v, 1, 6, 11, 12, m[s[10]], m[s[11]]);
    b2b_g(v, 2, 7, 8, 13, m[s[12]], m[s[13]]);
    b2b_g(v, 3, 4, 9, 14, m[s[14]], m[s[15]]);
  }

  for (int i = 0; i < 8; ++i) h[i] ^= v[i] ^ v[i + 8];
}

std::array<std::uint8_t, 64> blake2b(bytespan msg) noexcept {
  std::uint64_t h[8];
  for (int i = 0; i < 8; ++i) h[i] = B2B_IV[i];
  h[0] ^= 0x01010040u;

  const auto* p = reinterpret_cast<const std::uint8_t*>(msg.data());
  std::size_t remaining = msg.size();
  std::uint64_t total = 0;

  while (remaining > 128) {
    total += 128;
    b2b_compress(h, p, total, false);
    p += 128;
    remaining -= 128;
  }

  std::uint8_t block[128]{};
  if (remaining != 0) std::memcpy(block, p, remaining);
  total += static_cast<std::uint64_t>(remaining);
  b2b_compress(h, block, total, true);

  std::array<std::uint8_t, 64> out{};
  for (int i = 0; i < 8; ++i) store_le64(out.data() + i * 8, h[i]);
  return out;
}

}
