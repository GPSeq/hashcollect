// SPDX-License-Identifier: MIT
#pragma once
#include <cstdint>
#include "hashlab/common.hpp"

namespace hashlab {

    std::uint64_t rolling_hash(bytespan msg) noexcept;
}