// SPDX-License-Identifier: MIT
#pragma once
#include <cstdint>
#include "hashlab/common.hpp"

namespace hashlab {
// Source:
//   CRC-64/ECMA-182 polynomial 0x42F0E1EBA9EA3693 (normal form).
//   ECMA-182 standard.
std::uint64_t crc64_ecma(bytespan msg) noexcept;
}
