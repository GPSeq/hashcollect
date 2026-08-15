// SPDX-License-Identifier: MIT
#pragma once
#include <cstdint>
#include "hashlab/common.hpp"

namespace hashlab {
// Source:
//   CRC-16/CCITT-FALSE parameters: poly 0x1021, init 0xffff.
std::uint16_t crc16_ccitt(bytespan msg, std::uint16_t initial = 0xffff) noexcept;
}
