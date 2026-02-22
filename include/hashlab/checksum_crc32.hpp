// SPDX-License-Identifier: MIT
#pragma once
#include <cstdint>
#include "hashlab/common.hpp"

namespace hashlab {
// Source:
//   CRC-32 used by gzip (RFC 1952); standard reflected polynomial 0xEDB88320
//   https://www.rfc-editor.org/rfc/rfc1952.html
std::uint32_t crc32_ieee(bytespan msg) noexcept;
}
