// SPDX-License-Identifier: MIT
#pragma once
#include <array>
#include <cstdint>
#include "hashlab/common.hpp"

namespace hashlab {
// Source:
//   RFC 1321 (MD5 Message-Digest Algorithm).
//   https://www.rfc-editor.org/rfc/rfc1321.html
std::array<std::uint8_t, 16> md5(bytespan msg) noexcept;
}
