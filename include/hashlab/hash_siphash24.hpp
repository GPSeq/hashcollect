// SPDX-License-Identifier: MIT
#pragma once
#include <array>
#include <cstdint>
#include "hashlab/common.hpp"

namespace hashlab {
// Source:
//   SipHash: "SipHash: a fast short-input PRF", Aumasson & Bernstein (2012)
//   Reference code widely published by authors.
// Note: requires 128-bit key.
std::uint64_t siphash24(bytespan msg, std::array<std::uint8_t, 16> key) noexcept;
}
