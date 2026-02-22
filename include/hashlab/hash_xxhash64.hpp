// SPDX-License-Identifier: MIT
#pragma once
#include <cstdint>
#include "hashlab/common.hpp"

namespace hashlab {
// Source:
//   Yann Collet, xxHash (XXH64) reference implementation
//   https://github.com/Cyan4973/xxHash
std::uint64_t xxhash64(bytespan msg, std::uint64_t seed = 0) noexcept;
}
