// SPDX-License-Identifier: MIT
#pragma once
#include <cstdint>
#include "hashlab/common.hpp"

namespace hashlab {
// Source:
// sdbm hash (public domain heritage; commonly referenced from sdbm database).
std::uint32_t sdbm_32(bytespan msg) noexcept;
}
