// SPDX-License-Identifier: MIT
#pragma once
#include <cstdint>
#include "hashlab/common.hpp"

namespace hashlab {

// Rabin-Karp-style polynomial rolling hash: h = h * base + byte (mod 2^64).
std::uint64_t rolling_hash(bytespan msg, std::uint64_t base = 1315423911ULL) noexcept;
}
