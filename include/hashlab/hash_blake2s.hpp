// SPDX-License-Identifier: MIT
#pragma once
#include <array>
#include <cstdint>
#include "hashlab/common.hpp"

namespace hashlab {
// Source:
//   RFC 7693, The BLAKE2 Cryptographic Hash and Message Authentication Code.
//   https://www.rfc-editor.org/rfc/rfc7693
std::array<std::uint8_t, 32> blake2s(bytespan msg) noexcept;
}
