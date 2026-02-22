// SPDX-License-Identifier: MIT
#pragma once
#include <cstdint>
#include "hashlab/common.hpp"

namespace hashlab {
// Source:
//   Austin Appleby, MurmurHash2 (reference in SMHasher)
//   https://github.com/aappleby/smhasher
std::uint32_t murmur2_32(bytespan msg, std::uint32_t seed = 0x9747b28cU) noexcept;
}
