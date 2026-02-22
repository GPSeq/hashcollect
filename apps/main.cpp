// SPDX-License-Identifier: MIT
#include <array>
#include <cstdint>
#include <iostream>
#include <string_view>
#include <vector>

#include "hashlab/common.hpp"
#include "hashlab/hash_fnv1a32.hpp"
#include "hashlab/hash_fnv1a64.hpp"
#include "hashlab/hash_djb2.hpp"
#include "hashlab/hash_sdbm.hpp"
#include "hashlab/hash_jenkins_oaat.hpp"
#include "hashlab/hash_murmur2_32.hpp"
#include "hashlab/hash_murmur3_x86_32.hpp"
#include "hashlab/hash_murmur3_x64_128.hpp"
#include "hashlab/hash_xxhash32.hpp"
#include "hashlab/hash_xxhash64.hpp"
#include "hashlab/hash_siphash24.hpp"
#include "hashlab/checksum_crc32.hpp"
#include "hashlab/checksum_crc32c.hpp"
#include "hashlab/checksum_adler32.hpp"
#include "hashlab/checksum_crc64_ecma.hpp"
#include "hashlab/hash_pearson64.hpp"
#include "hashlab/hash_buzhash32.hpp"
#include "hashlab/hash_rabinkarp64.hpp"
#include "hashlab/hash_md5.hpp"
#include "hashlab/hash_sha256.hpp"

static std::string hex_arr(auto const& arr) {

  return hashlab::hex_lower(std::span<const std::uint8_t>(arr.data(), arr.size()));
}

int main() {
  const std::int32_t i = 123456789;
  const float f = 3.1415926f;
  const std::string_view s = "ACCGTAAATTTGCCCCCCX";
  std::cout << "Int: " << i << '\n';
  std::cout << "Float: " << f << '\n';
  std::cout << "String: "<< s << '\n';
  const auto bi = hashlab::to_le_bytes(i);
  const auto bf = hashlab::to_le_bytes(f);
  const auto bs = hashlab::as_bytes(s);

  const std::array<std::uint8_t, 16> sipkey = {0,1,2,3,4,5,6,7, 8,9,10,11,12,13,14,15};

  auto show_u32 = [&](const char* name, auto fn) {
    std::cout << name << "\n";
    std::cout << "  int  : 0x" << hashlab::hex_lower_int((std::uint32_t)fn(bi)) << "\n";
    std::cout << "  float: 0x" << hashlab::hex_lower_int((std::uint32_t)fn(bf)) << "\n";
    std::cout << "  str  : 0x" << hashlab::hex_lower_int((std::uint32_t)fn(bs)) << "\n";
  };
  auto show_u64 = [&](const char* name, auto fn) {
    std::cout << name << "\n";
    std::cout << "  int  : 0x" << hashlab::hex_lower_int((std::uint64_t)fn(bi)) << "\n";
    std::cout << "  float: 0x" << hashlab::hex_lower_int((std::uint64_t)fn(bf)) << "\n";
    std::cout << "  str  : 0x" << hashlab::hex_lower_int((std::uint64_t)fn(bs)) << "\n";
  };

  show_u32("FNV-1a-32", hashlab::fnv1a32);
  show_u64("FNV-1a-64", hashlab::fnv1a64);

  show_u32("DJB2-32", hashlab::djb2_32);
  show_u32("SDBM-32", hashlab::sdbm_32);
  show_u32("Jenkins-OAAT-32", hashlab::jenkins_oaat32);

  std::cout << "Murmur2-32\n";
  std::cout << "  int  : 0x" << hashlab::hex_lower_int(hashlab::murmur2_32(bi)) << "\n";
  std::cout << "  float: 0x" << hashlab::hex_lower_int(hashlab::murmur2_32(bf)) << "\n";
  std::cout << "  str  : 0x" << hashlab::hex_lower_int(hashlab::murmur2_32(bs)) << "\n";

  std::cout << "Murmur3-x86-32\n";
  std::cout << "  int  : 0x" << hashlab::hex_lower_int(hashlab::murmur3_x86_32(bi)) << "\n";
  std::cout << "  float: 0x" << hashlab::hex_lower_int(hashlab::murmur3_x86_32(bf)) << "\n";
  std::cout << "  str  : 0x" << hashlab::hex_lower_int(hashlab::murmur3_x86_32(bs)) << "\n";

  std::cout << "Murmur3-x64-128\n";
  std::cout << "  int  : 0x" << hex_arr(hashlab::murmur3_x64_128(bi)) << "\n";
  std::cout << "  float: 0x" << hex_arr(hashlab::murmur3_x64_128(bf)) << "\n";
  std::cout << "  str  : 0x" << hex_arr(hashlab::murmur3_x64_128(bs)) << "\n";

  std::cout << "xxHash32\n";
  std::cout << "  int  : 0x" << hashlab::hex_lower_int(hashlab::xxhash32(bi)) << "\n";
  std::cout << "  float: 0x" << hashlab::hex_lower_int(hashlab::xxhash32(bf)) << "\n";
  std::cout << "  str  : 0x" << hashlab::hex_lower_int(hashlab::xxhash32(bs)) << "\n";

  std::cout << "xxHash64\n";
  std::cout << "  int  : 0x" << hashlab::hex_lower_int(hashlab::xxhash64(bi)) << "\n";
  std::cout << "  float: 0x" << hashlab::hex_lower_int(hashlab::xxhash64(bf)) << "\n";
  std::cout << "  str  : 0x" << hashlab::hex_lower_int(hashlab::xxhash64(bs)) << "\n";

  std::cout << "SipHash-2-4\n";
  std::cout << "  int  : 0x" << hashlab::hex_lower_int(hashlab::siphash24(bi, sipkey)) << "\n";
  std::cout << "  float: 0x" << hashlab::hex_lower_int(hashlab::siphash24(bf, sipkey)) << "\n";
  std::cout << "  str  : 0x" << hashlab::hex_lower_int(hashlab::siphash24(bs, sipkey)) << "\n";

  show_u32("CRC32-IEEE", hashlab::crc32_ieee);
  show_u32("CRC32C", hashlab::crc32c);
  show_u32("Adler32", hashlab::adler32);

  show_u64("CRC64-ECMA", hashlab::crc64_ecma);
  show_u64("Pearson64", hashlab::pearson64);
  show_u32("Buzhash32", hashlab::buzhash32);
  //show_u64("RabinKarp64", hashlab::rabinkarp64);
  show_u64("RabinKarp64", [](hashlab::bytespan b) noexcept {
      return hashlab::rabinkarp64(b);
    });

  std::cout << "MD5\n";
  std::cout << "  int  : 0x" << hex_arr(hashlab::md5(bi)) << "\n";
  std::cout << "  float: 0x" << hex_arr(hashlab::md5(bf)) << "\n";
  std::cout << "  str  : 0x" << hex_arr(hashlab::md5(bs)) << "\n";

  std::cout << "SHA-256\n";
  std::cout << "  int  : 0x" << hex_arr(hashlab::sha256(bi)) << "\n";
  std::cout << "  float: 0x" << hex_arr(hashlab::sha256(bf)) << "\n";
  std::cout << "  str  : 0x" << hex_arr(hashlab::sha256(bs)) << "\n";

  return 0;
}
