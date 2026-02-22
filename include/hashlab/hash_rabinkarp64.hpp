// SPDX-License-Identifier: MIT
#pragma once
#include <cstdint>
#include "hashlab/common.hpp"

namespace hashlab {
// Source:
//  Rabin–Karp rolling hash idea (polynomial hash). Here: base * h + byte (mod 2^64).
std::uint64_t rabinkarp64(bytespan msg, std::uint64_t base = 1315423911ULL) noexcept;
}
