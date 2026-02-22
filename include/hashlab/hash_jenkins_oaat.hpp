// SPDX-License-Identifier: MIT
#pragma once
#include <cstdint>
#include "hashlab/common.hpp"

namespace hashlab {
// Source:
// Bob Jenkins, "one-at-a-time" hash; see also lookup3.c notes
// https://burtleburtle.net/bob/c/lookup3.c
std::uint32_t jenkins_oaat32(bytespan msg) noexcept;
}
