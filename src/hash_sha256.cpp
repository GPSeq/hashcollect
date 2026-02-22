// SPDX-License-Identifier: MIT
#include "hashlab/hash_sha256.hpp"

namespace hashlab {

static inline std::uint32_t rotr32(std::uint32_t x, int r) noexcept { return (x >> r) | (x << (32 - r)); }
static inline std::uint32_t ch(std::uint32_t x, std::uint32_t y, std::uint32_t z) noexcept { return (x & y) ^ (~x & z); }
static inline std::uint32_t maj(std::uint32_t x, std::uint32_t y, std::uint32_t z) noexcept { return (x & y) ^ (x & z) ^ (y & z); }
static inline std::uint32_t bsig0(std::uint32_t x) noexcept { return rotr32(x, 2) ^ rotr32(x, 13) ^ rotr32(x, 22); }
static inline std::uint32_t bsig1(std::uint32_t x) noexcept { return rotr32(x, 6) ^ rotr32(x, 11) ^ rotr32(x, 25); }
static inline std::uint32_t ssig0(std::uint32_t x) noexcept { return rotr32(x, 7) ^ rotr32(x, 18) ^ (x >> 3); }
static inline std::uint32_t ssig1(std::uint32_t x) noexcept { return rotr32(x, 17) ^ rotr32(x, 19) ^ (x >> 10); }

std::array<std::uint8_t, 32> sha256(bytespan msg) noexcept {
  static constexpr std::uint32_t K[64] = {
    0x428a2f98u,0x71374491u,0xb5c0fbcfu,0xe9b5dba5u,0x3956c25bu,0x59f111f1u,0x923f82a4u,0xab1c5ed5u,
    0xd807aa98u,0x12835b01u,0x243185beu,0x550c7dc3u,0x72be5d74u,0x80deb1feu,0x9bdc06a7u,0xc19bf174u,
    0xe49b69c1u,0xefbe4786u,0x0fc19dc6u,0x240ca1ccu,0x2de92c6fu,0x4a7484aau,0x5cb0a9dcu,0x76f988dau,
    0x983e5152u,0xa831c66du,0xb00327c8u,0xbf597fc7u,0xc6e00bf3u,0xd5a79147u,0x06ca6351u,0x14292967u,
    0x27b70a85u,0x2e1b2138u,0x4d2c6dfcu,0x53380d13u,0x650a7354u,0x766a0abbu,0x81c2c92eu,0x92722c85u,
    0xa2bfe8a1u,0xa81a664bu,0xc24b8b70u,0xc76c51a3u,0xd192e819u,0xd6990624u,0xf40e3585u,0x106aa070u,
    0x19a4c116u,0x1e376c08u,0x2748774cu,0x34b0bcb5u,0x391c0cb3u,0x4ed8aa4au,0x5b9cca4fu,0x682e6ff3u,
    0x748f82eeu,0x78a5636fu,0x84c87814u,0x8cc70208u,0x90befffau,0xa4506cebu,0xbef9a3f7u,0xc67178f2u
  };

  std::uint32_t H[8] = {
    0x6a09e667u,0xbb67ae85u,0x3c6ef372u,0xa54ff53au,
    0x510e527fu,0x9b05688cu,0x1f83d9abu,0x5be0cd19u
  };

  std::vector<std::uint8_t> data(msg.size());
  std::memcpy(data.data(), msg.data(), msg.size());

  std::uint64_t bitlen = (std::uint64_t)data.size() * 8ULL;
  data.push_back(0x80u);
  while ((data.size() % 64) != 56) data.push_back(0x00u);

  // big-endian length
  for (int i = 7; i >= 0; --i) data.push_back((std::uint8_t)((bitlen >> (8 * i)) & 0xFFu));

  for (std::size_t off = 0; off < data.size(); off += 64) {
    std::uint32_t W[64];
    for (int i = 0; i < 16; ++i) {
      // SHA uses big-endian words
      const std::uint8_t* p = data.data() + off + i * 4;
      W[i] = (std::uint32_t)p[0] << 24 | (std::uint32_t)p[1] << 16 | (std::uint32_t)p[2] << 8 | (std::uint32_t)p[3];
    }
    for (int i = 16; i < 64; ++i) {
      W[i] = ssig1(W[i-2]) + W[i-7] + ssig0(W[i-15]) + W[i-16];
    }

    std::uint32_t a = H[0], b = H[1], c = H[2], d = H[3];
    std::uint32_t e = H[4], f = H[5], g = H[6], h = H[7];

    for (int i = 0; i < 64; ++i) {
      std::uint32_t T1 = h + bsig1(e) + ch(e,f,g) + K[i] + W[i];
      std::uint32_t T2 = bsig0(a) + maj(a,b,c);
      h = g;
      g = f;
      f = e;
      e = d + T1;
      d = c;
      c = b;
      b = a;
      a = T1 + T2;
    }

    H[0] += a; H[1] += b; H[2] += c; H[3] += d;
    H[4] += e; H[5] += f; H[6] += g; H[7] += h;
  }

  std::array<std::uint8_t, 32> out{};
  for (int i = 0; i < 8; ++i) {
    out[i*4 + 0] = (std::uint8_t)((H[i] >> 24) & 0xFFu);
    out[i*4 + 1] = (std::uint8_t)((H[i] >> 16) & 0xFFu);
    out[i*4 + 2] = (std::uint8_t)((H[i] >> 8) & 0xFFu);
    out[i*4 + 3] = (std::uint8_t)((H[i] >> 0) & 0xFFu);
  }
  return out;
}

}
