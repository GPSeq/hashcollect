// SPDX-License-Identifier: MIT
#pragma once
#include <cstdint>
#include "hashlab/common.hpp"

namespace hashlab {
// Source:
//   Peter K. Pearson, "Fast Hashing of Variable-Length Text Strings" (CACM, 1990).
std::uint64_t pearson64(bytespan msg) noexcept;
}
