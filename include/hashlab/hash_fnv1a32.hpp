// SPDX-License-Identifier: MIT
#pragma once
#include <cstdint>
#include "hashlab/common.hpp"

namespace hashlab {
// Source:
//   IETF Internet-Draft "The FNV Non-Cryptographic Hash Algorithm"
//   https://datatracker.ietf.org/doc/html/draft-eastlake-fnv-22
std::uint32_t fnv1a32(bytespan msg) noexcept;
} // namespace hashlab
