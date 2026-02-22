// SPDX-License-Identifier: MIT
#pragma once

#include <algorithm>
#include <array>
#include <bit>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <span>
#include <string>
#include <string_view>
#include <type_traits>
#include <vector>

namespace hashlab {

using bytespan = std::span<const std::byte>;

inline bytespan as_bytes(std::string_view s) noexcept {
  return bytespan{reinterpret_cast<const std::byte*>(s.data()), s.size()};
}

template <class T>
inline std::vector<std::byte> to_le_bytes(T v) {
  static_assert(std::is_trivially_copyable_v<T>);
  std::array<std::byte, sizeof(T)> a = std::bit_cast<std::array<std::byte, sizeof(T)>>(v);
  if constexpr (std::endian::native == std::endian::little) {
    return {a.begin(), a.end()};
  } else {
    std::reverse(a.begin(), a.end());
    return {a.begin(), a.end()};
  }
}

inline std::uint32_t load_le32(const void* p) noexcept {
  std::uint32_t v;
  std::memcpy(&v, p, sizeof(v));
  if constexpr (std::endian::native == std::endian::big) {
    v = (v >> 24) |
        ((v >> 8) & 0x0000FF00u) |
        ((v << 8) & 0x00FF0000u) |
        (v << 24);
  }
  return v;
}

inline std::uint64_t load_le64(const void* p) noexcept {
  std::uint64_t v;
  std::memcpy(&v, p, sizeof(v));
  if constexpr (std::endian::native == std::endian::big) {
    v = (v >> 56) |
        ((v >> 40) & 0x000000000000FF00ULL) |
        ((v >> 24) & 0x0000000000FF0000ULL) |
        ((v >> 8)  & 0x00000000FF000000ULL) |
        ((v << 8)  & 0x000000FF00000000ULL) |
        ((v << 24) & 0x0000FF0000000000ULL) |
        ((v << 40) & 0x00FF000000000000ULL) |
        (v << 56);
  }
  return v;
}

inline void store_le32(void* p, std::uint32_t v) noexcept {
  if constexpr (std::endian::native == std::endian::big) {
    v = (v >> 24) |
        ((v >> 8) & 0x0000FF00u) |
        ((v << 8) & 0x00FF0000u) |
        (v << 24);
  }
  std::memcpy(p, &v, sizeof(v));
}

inline void store_le64(void* p, std::uint64_t v) noexcept {
  if constexpr (std::endian::native == std::endian::big) {
    v = (v >> 56) |
        ((v >> 40) & 0x000000000000FF00ULL) |
        ((v >> 24) & 0x0000000000FF0000ULL) |
        ((v >> 8)  & 0x00000000FF000000ULL) |
        ((v << 8)  & 0x000000FF00000000ULL) |
        ((v << 24) & 0x0000FF0000000000ULL) |
        ((v << 40) & 0x00FF000000000000ULL) |
        (v << 56);
  }
  std::memcpy(p, &v, sizeof(v));
}

inline std::string hex_lower(std::span<const std::uint8_t> b) {
  static constexpr char kHex[] = "0123456789abcdef";
  std::string out;
  out.reserve(b.size() * 2);
  for (auto v : b) {
    out.push_back(kHex[(v >> 4) & 0xFu]);
    out.push_back(kHex[v & 0xFu]);
  }
  return out;
}

inline std::string hex_lower(bytespan b) {
  std::string out;
  out.reserve(b.size() * 2);
  static constexpr char kHex[] = "0123456789abcdef";
  for (auto x : b) {
    auto v = static_cast<std::uint8_t>(x);
    out.push_back(kHex[(v >> 4) & 0xFu]);
    out.push_back(kHex[v & 0xFu]);
  }
  return out;
}

template <class UInt>
inline std::string hex_lower_int(UInt v) {
  static_assert(std::is_unsigned_v<UInt>);
  std::array<std::uint8_t, sizeof(UInt)> b{};
  std::memcpy(b.data(), &v, sizeof(UInt));
  if constexpr (std::endian::native == std::endian::big) std::reverse(b.begin(), b.end());
  return hex_lower(std::span<const std::uint8_t>(b.data(), b.size()));
}

}
