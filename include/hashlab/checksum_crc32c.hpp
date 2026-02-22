// SPDX-License-Identifier: MIT
#pragma once
#include <cstdint>
#include "hashlab/common.hpp"

namespace hashlab {
// Source:
//   CRC-32C (Castagnoli) polynomial (reflected): 0x82F63B78, used in iSCSI etc.
std::uint32_t crc32c(bytespan msg) noexcept;
}
