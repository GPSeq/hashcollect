// SPDX-License-Identifier: MIT
#pragma once
#include <cstdint>
#include "hashlab/common.hpp"

namespace hashlab {
//Source:
// Daniel J. Bernstein (djb2), widely circulated; original attributed to DJB.
std::uint32_t djb2_32(bytespan msg) noexcept;
}
