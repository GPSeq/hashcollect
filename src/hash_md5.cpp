// SPDX-License-Identifier: MIT
#include "hashlab/hash_md5.hpp"

namespace hashlab {

static inline std::uint32_t rotl32(std::uint32_t x, int r) noexcept { return (x << r) | (x >> (32 - r)); }

std::array<std::uint8_t, 16> md5(bytespan msg) noexcept {
  // RFC 1321 constants
  static constexpr std::uint32_t s[64] = {
    7,12,17,22, 7,12,17,22, 7,12,17,22, 7,12,17,22,
    5, 9,14,20, 5, 9,14,20, 5, 9,14,20, 5, 9,14,20,
    4,11,16,23, 4,11,16,23, 4,11,16,23, 4,11,16,23,
    6,10,15,21, 6,10,15,21, 6,10,15,21, 6,10,15,21
  };
  static constexpr std::uint32_t K[64] = {
    0xd76aa478u,0xe8c7b756u,0x242070dbu,0xc1bdceeeu,0xf57c0fafu,0x4787c62au,0xa8304613u,0xfd469501u,
    0x698098d8u,0x8b44f7afu,0xffff5bb1u,0x895cd7beu,0x6b901122u,0xfd987193u,0xa679438eu,0x49b40821u,
    0xf61e2562u,0xc040b340u,0x265e5a51u,0xe9b6c7aau,0xd62f105du,0x02441453u,0xd8a1e681u,0xe7d3fbc8u,
    0x21e1cde6u,0xc33707d6u,0xf4d50d87u,0x455a14edu,0xa9e3e905u,0xfcefa3f8u,0x676f02d9u,0x8d2a4c8au,
    0xfffa3942u,0x8771f681u,0x6d9d6122u,0xfde5380cu,0xa4beea44u,0x4bdecfa9u,0xf6bb4b60u,0xbebfbc70u,
    0x289b7ec6u,0xeaa127fau,0xd4ef3085u,0x04881d05u,0xd9d4d039u,0xe6db99e5u,0x1fa27cf8u,0xc4ac5665u,
    0xf4292244u,0x432aff97u,0xab9423a7u,0xfc93a039u,0x655b59c3u,0x8f0ccc92u,0xffeff47du,0x85845dd1u,
    0x6fa87e4fu,0xfe2ce6e0u,0xa3014314u,0x4e0811a1u,0xf7537e82u,0xbd3af235u,0x2ad7d2bbu,0xeb86d391u
  };

  std::uint32_t a0 = 0x67452301u;
  std::uint32_t b0 = 0xefcdab89u;
  std::uint32_t c0 = 0x98badcfeu;
  std::uint32_t d0 = 0x10325476u;

  std::vector<std::uint8_t> data(msg.size());
  std::memcpy(data.data(), msg.data(), msg.size());

  // pad
  std::uint64_t bitlen = (std::uint64_t)data.size() * 8ULL;
  data.push_back(0x80u);
  while ((data.size() % 64) != 56) data.push_back(0x00u);
  for (int i = 0; i < 8; ++i) data.push_back((std::uint8_t)((bitlen >> (8 * i)) & 0xFFu));

  for (std::size_t offset = 0; offset < data.size(); offset += 64) {
    std::uint32_t M[16];
    for (int i = 0; i < 16; ++i) M[i] = load_le32(data.data() + offset + i * 4);

    std::uint32_t A = a0, B = b0, C = c0, D = d0;

    for (int i = 0; i < 64; ++i) {
      std::uint32_t F, g;
      if (i < 16) { F = (B & C) | (~B & D); g = (std::uint32_t)i; }
      else if (i < 32) { F = (D & B) | (~D & C); g = (std::uint32_t)((5*i + 1) % 16); }
      else if (i < 48) { F = B ^ C ^ D; g = (std::uint32_t)((3*i + 5) % 16); }
      else { F = C ^ (B | ~D); g = (std::uint32_t)((7*i) % 16); }

      std::uint32_t tmp = D;
      D = C;
      C = B;
      B = B + rotl32(A + F + K[i] + M[g], (int)s[i]);
      A = tmp;
    }

    a0 += A; b0 += B; c0 += C; d0 += D;
  }

  std::array<std::uint8_t, 16> out{};
  store_le32(out.data() + 0, a0);
  store_le32(out.data() + 4, b0);
  store_le32(out.data() + 8, c0);
  store_le32(out.data() + 12, d0);
  return out;
}

}
