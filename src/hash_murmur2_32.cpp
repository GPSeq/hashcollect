// SPDX-License-Identifier: MIT
#include "hashlab/hash_murmur2_32.hpp"

namespace hashlab {

std::uint32_t murmur2_32(bytespan msg, std::uint32_t seed) noexcept {

  constexpr std::uint32_t m = 0x5bd1e995U;
  constexpr int r = 24;

  std::uint32_t h = seed ^ static_cast<std::uint32_t>(msg.size());
  const std::uint8_t* data = reinterpret_cast<const std::uint8_t*>(msg.data());
  std::size_t len = msg.size();

  while (len >= 4) {
    std::uint32_t k = load_le32(data);
    k *= m;
    k ^= k >> r;
    k *= m;

    h *= m;
    h ^= k;

    data += 4;
    len -= 4;
  }

  switch (len) {
    case 3: h ^= static_cast<std::uint32_t>(data[2]) << 16; [[fallthrough]];
    case 2: h ^= static_cast<std::uint32_t>(data[1]) << 8;  [[fallthrough]];
    case 1: h ^= static_cast<std::uint32_t>(data[0]); h *= m; break;
    default: break;
  }

  h ^= h >> 13;
  h *= m;
  h ^= h >> 15;
  return h;
}

}
