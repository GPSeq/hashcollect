// SPDX-License-Identifier: MIT
#pragma once
#include <cstdint>
#include "hashlab/common.hpp"

namespace hashlab {
// Source:
//   Buzhash rolling hash (commonly attributed to Buzbee); used for content-defined chunking variants.
// This is a simple buzhash-style mix over bytes (not windowed rolling here).
std::uint32_t buzhash32(bytespan msg) noexcept;
}
