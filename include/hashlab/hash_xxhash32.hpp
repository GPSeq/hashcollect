// SPDX-License-Identifier: MIT
#pragma once
#include <cstdint>
#include "hashlab/common.hpp"

namespace hashlab {
// Source:
//   Yann Collet, xxHash (XXH32) reference implementation
//   https://github.com/Cyan4973/xxHash
std::uint32_t xxhash32(bytespan msg, std::uint32_t seed = 0) noexcept;
}
