// SPDX-License-Identifier: MIT
#pragma once
#include <cstdint>
#include "hashlab/common.hpp"

namespace hashlab {
// Source:
//   Adler-32 described in RFC 1950 (zlib format).
//   https://www.rfc-editor.org/rfc/rfc1950.html
std::uint32_t adler32(bytespan msg) noexcept;
}
