// SPDX-License-Identifier: MIT
#pragma once
#include <array>
#include <cstdint>
#include "hashlab/common.hpp"

namespace hashlab {
// Source (public domain reference; SMHasher):
//Austin Appleby, MurmurHash3 (x64_128)
//https://github.com/aappleby/smhasher
std::array<std::uint8_t, 16> murmur3_x64_128(bytespan msg, std::uint32_t seed = 0) noexcept;
}
