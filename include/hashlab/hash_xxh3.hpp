// SPDX-License-Identifier: MIT
#pragma once
#include <array>
#include <cstdint>
#include "hashlab/common.hpp"

namespace hashlab {
// Source:
//   Yann Collet, xxHash reference implementation (XXH3 family).
//   https://github.com/Cyan4973/xxHash
std::uint64_t xxh3_64(bytespan msg, std::uint64_t seed = 0) noexcept;
std::array<std::uint8_t, 16> xxh3_128(bytespan msg, std::uint64_t seed = 0) noexcept;
}
