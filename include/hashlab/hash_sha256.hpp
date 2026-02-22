// SPDX-License-Identifier: MIT
#pragma once
#include <array>
#include <cstdint>
#include "hashlab/common.hpp"

namespace hashlab {
// Source:
//   FIPS PUB 180-4 (Secure Hash Standard), SHA-256.
//   https://csrc.nist.gov/publications/detail/fips/180/4/final
std::array<std::uint8_t, 32> sha256(bytespan msg) noexcept;
}
