// SPDX-License-Identifier: MIT
// Implements BLAKE2s-256.
#include "hashlab/hash_blake2s.hpp"

namespace hashlab {

static constexpr std::uint32_t B2S_IV[8] = {
  0x6A09E667u,0xBB67AE85u,0x3C6EF372u,0xA54FF53Au,
  0x510E527Fu,0x9B05688Cu,0x1F83D9ABu,0x5BE0CD19u
};

static constexpr std::uint8_t B2S_SIGMA[10][16] = {
  {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15},
  {14,10,4,8,9,15,13,6,1,12,0,2,11,7,5,3},
  {11,8,12,0,5,2,15,13,10,14,3,6,7,1,9,4},
  {7,9,3,1,13,12,11,14,2,6,5,10,4,0,15,8},
  {9,0,5,7,2,4,10,15,14,1,11,12,6,8,3,13},
  {2,12,6,10,0,11,8,3,4,13,7,5,15,14,1,9},
  {12,5,1,15,14,13,4,10,0,7,6,3,9,2,8,11},
  {13,11,7,14,12,1,3,9,5,0,15,4,8,6,2,10},
  {6,15,14,9,11,3,0,8,12,2,13,7,1,4,10,5},
  {10,2,8,4,7,6,1,5,15,11,9,14,3,12,13,0}
};

static inline std::uint32_t rotr32_b2s(std::uint32_t x, int r) noexcept {
  return (x >> r) | (x << (32 - r));
}

static inline void b2s_g(std::uint32_t v[16], int a, int b, int c, int d, std::uint32_t x, std::uint32_t y) noexcept {
  v[a] = v[a] + v[b] + x;
  v[d] = rotr32_b2s(v[d] ^ v[a], 16);
  v[c] += v[d];
  v[b] = rotr32_b2s(v[b] ^ v[c], 12);
  v[a] = v[a] + v[b] + y;
  v[d] = rotr32_b2s(v[d] ^ v[a], 8);
  v[c] += v[d];
  v[b] = rotr32_b2s(v[b] ^ v[c], 7);
}

static void b2s_compress(std::uint32_t h[8], const std::uint8_t block[64], std::uint64_t t, bool last) noexcept {
  std::uint32_t m[16];
  for (int i = 0; i < 16; ++i) m[i] = load_le32(block + i * 4);

  std::uint32_t v[16];
  for (int i = 0; i < 8; ++i) v[i] = h[i];
  for (int i = 0; i < 8; ++i) v[i + 8] = B2S_IV[i];
  v[12] ^= static_cast<std::uint32_t>(t);
  v[13] ^= static_cast<std::uint32_t>(t >> 32);
  if (last) v[14] = ~v[14];

  for (int r = 0; r < 10; ++r) {
    const auto* s = B2S_SIGMA[r];
    b2s_g(v, 0, 4, 8, 12, m[s[0]], m[s[1]]);
    b2s_g(v, 1, 5, 9, 13, m[s[2]], m[s[3]]);
    b2s_g(v, 2, 6, 10, 14, m[s[4]], m[s[5]]);
    b2s_g(v, 3, 7, 11, 15, m[s[6]], m[s[7]]);
    b2s_g(v, 0, 5, 10, 15, m[s[8]], m[s[9]]);
    b2s_g(v, 1, 6, 11, 12, m[s[10]], m[s[11]]);
    b2s_g(v, 2, 7, 8, 13, m[s[12]], m[s[13]]);
    b2s_g(v, 3, 4, 9, 14, m[s[14]], m[s[15]]);
  }

  for (int i = 0; i < 8; ++i) h[i] ^= v[i] ^ v[i + 8];
}

std::array<std::uint8_t, 32> blake2s(bytespan msg) noexcept {
  std::uint32_t h[8];
  for (int i = 0; i < 8; ++i) h[i] = B2S_IV[i];
  h[0] ^= 0x01010020u;

  const auto* p = reinterpret_cast<const std::uint8_t*>(msg.data());
  std::size_t remaining = msg.size();
  std::uint64_t total = 0;

  while (remaining > 64) {
    total += 64;
    b2s_compress(h, p, total, false);
    p += 64;
    remaining -= 64;
  }

  std::uint8_t block[64]{};
  if (remaining != 0) std::memcpy(block, p, remaining);
  total += static_cast<std::uint64_t>(remaining);
  b2s_compress(h, block, total, true);

  std::array<std::uint8_t, 32> out{};
  for (int i = 0; i < 8; ++i) store_le32(out.data() + i * 4, h[i]);
  return out;
}

}
