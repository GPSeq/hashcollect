// SPDX-License-Identifier: MIT
// Implements MurmurHash3 x86 128-bit.
#include "hashlab/hash_murmur3_x86_128.hpp"

namespace hashlab {

static inline std::uint32_t rotl32_local(std::uint32_t x, int r) noexcept {
  return (x << r) | (x >> (32 - r));
}

static inline std::uint32_t fmix32(std::uint32_t h) noexcept {
  h ^= h >> 16;
  h *= 0x85ebca6bu;
  h ^= h >> 13;
  h *= 0xc2b2ae35u;
  h ^= h >> 16;
  return h;
}

std::array<std::uint8_t, 16> murmur3_x86_128(bytespan msg, std::uint32_t seed) noexcept {
  constexpr std::uint32_t C1 = 0x239b961bu;
  constexpr std::uint32_t C2 = 0xab0e9789u;
  constexpr std::uint32_t C3 = 0x38b34ae5u;
  constexpr std::uint32_t C4 = 0xa1e38b93u;

  const auto* data = reinterpret_cast<const std::uint8_t*>(msg.data());
  const int nblocks = static_cast<int>(msg.size() / 16);
  std::uint32_t h1 = seed, h2 = seed, h3 = seed, h4 = seed;

  for (int i = 0; i < nblocks; ++i) {
    std::uint32_t k1 = load_le32(data + i * 16 + 0);
    std::uint32_t k2 = load_le32(data + i * 16 + 4);
    std::uint32_t k3 = load_le32(data + i * 16 + 8);
    std::uint32_t k4 = load_le32(data + i * 16 + 12);

    k1 *= C1; k1 = rotl32_local(k1, 15); k1 *= C2; h1 ^= k1;
    h1 = rotl32_local(h1, 19); h1 += h2; h1 = h1 * 5u + 0x561ccd1bu;

    k2 *= C2; k2 = rotl32_local(k2, 16); k2 *= C3; h2 ^= k2;
    h2 = rotl32_local(h2, 17); h2 += h3; h2 = h2 * 5u + 0x0bcaa747u;

    k3 *= C3; k3 = rotl32_local(k3, 17); k3 *= C4; h3 ^= k3;
    h3 = rotl32_local(h3, 15); h3 += h4; h3 = h3 * 5u + 0x96cd1c35u;

    k4 *= C4; k4 = rotl32_local(k4, 18); k4 *= C1; h4 ^= k4;
    h4 = rotl32_local(h4, 13); h4 += h1; h4 = h4 * 5u + 0x32ac3b17u;
  }

  const auto* tail = data + nblocks * 16;
  std::uint32_t k1 = 0, k2 = 0, k3 = 0, k4 = 0;

  switch (msg.size() & 15u) {
    case 15: k4 ^= static_cast<std::uint32_t>(tail[14]) << 16; [[fallthrough]];
    case 14: k4 ^= static_cast<std::uint32_t>(tail[13]) << 8; [[fallthrough]];
    case 13: k4 ^= static_cast<std::uint32_t>(tail[12]); k4 *= C4; k4 = rotl32_local(k4, 18); k4 *= C1; h4 ^= k4; [[fallthrough]];
    case 12: k3 ^= static_cast<std::uint32_t>(tail[11]) << 24; [[fallthrough]];
    case 11: k3 ^= static_cast<std::uint32_t>(tail[10]) << 16; [[fallthrough]];
    case 10: k3 ^= static_cast<std::uint32_t>(tail[9]) << 8; [[fallthrough]];
    case 9:  k3 ^= static_cast<std::uint32_t>(tail[8]); k3 *= C3; k3 = rotl32_local(k3, 17); k3 *= C4; h3 ^= k3; [[fallthrough]];
    case 8:  k2 ^= static_cast<std::uint32_t>(tail[7]) << 24; [[fallthrough]];
    case 7:  k2 ^= static_cast<std::uint32_t>(tail[6]) << 16; [[fallthrough]];
    case 6:  k2 ^= static_cast<std::uint32_t>(tail[5]) << 8; [[fallthrough]];
    case 5:  k2 ^= static_cast<std::uint32_t>(tail[4]); k2 *= C2; k2 = rotl32_local(k2, 16); k2 *= C3; h2 ^= k2; [[fallthrough]];
    case 4:  k1 ^= static_cast<std::uint32_t>(tail[3]) << 24; [[fallthrough]];
    case 3:  k1 ^= static_cast<std::uint32_t>(tail[2]) << 16; [[fallthrough]];
    case 2:  k1 ^= static_cast<std::uint32_t>(tail[1]) << 8; [[fallthrough]];
    case 1:  k1 ^= static_cast<std::uint32_t>(tail[0]); k1 *= C1; k1 = rotl32_local(k1, 15); k1 *= C2; h1 ^= k1; [[fallthrough]];
    default: break;
  }

  const auto len = static_cast<std::uint32_t>(msg.size());
  h1 ^= len; h2 ^= len; h3 ^= len; h4 ^= len;
  h1 += h2 + h3 + h4;
  h2 += h1; h3 += h1; h4 += h1;
  h1 = fmix32(h1); h2 = fmix32(h2); h3 = fmix32(h3); h4 = fmix32(h4);
  h1 += h2 + h3 + h4;
  h2 += h1; h3 += h1; h4 += h1;

  std::array<std::uint8_t, 16> out{};
  store_le32(out.data() + 0, h1);
  store_le32(out.data() + 4, h2);
  store_le32(out.data() + 8, h3);
  store_le32(out.data() + 12, h4);
  return out;
}

}
