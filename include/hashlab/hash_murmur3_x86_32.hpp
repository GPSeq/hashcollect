// SPDX-License-Identifier: MIT
#pragma once
#include <cstdint>
#include "hashlab/common.hpp"

namespace hashlab {
// Source (public domain reference; SMHasher):
//   Austin Appleby, MurmurHash3 (x86_32)
//   https://github.com/aappleby/smhasher
std::uint32_t murmur3_x86_32(bytespan msg, std::uint32_t seed = 0) noexcept;
}
