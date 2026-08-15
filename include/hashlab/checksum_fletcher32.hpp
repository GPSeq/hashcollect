// SPDX-License-Identifier: MIT
#pragma once
#include <cstdint>
#include "hashlab/common.hpp"

namespace hashlab {
// Source:
//   Fletcher's checksum, 32-bit variant over big-endian 16-bit words.
std::uint32_t fletcher32(bytespan msg) noexcept;
}
