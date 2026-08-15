// SPDX-License-Identifier: MIT
#include <cstdint>
#include <iostream>
#include <string_view>

#include "hashlab/checksum_crc16_ccitt.hpp"
#include "hashlab/common.hpp"
#include "hashlab/hash_blake2b.hpp"
#include "hashlab/hash_blake2s.hpp"
#include "hashlab/hash_sha512.hpp"

static bool expect_hex(const char* name, const std::string& actual, std::string_view expected) {
  if (actual == expected) return true;
  std::cerr << name << " mismatch\nexpected: " << expected << "\nactual  : " << actual << '\n';
  return false;
}

int main() {
  const auto empty = hashlab::as_bytes("");
  const auto digits = hashlab::as_bytes("123456789");
  bool ok = true;

  ok &= expect_hex(
      "SHA-512 empty",
      hashlab::hex_lower(hashlab::sha512(empty)),
      "cf83e1357eefb8bdf1542850d66d8007d620e4050b5715dc83f4a921d36ce9ce"
      "47d0d13c5d85f2b0ff8318d2877eec2f63b931bd47417a81a538327af927da3e");
  ok &= expect_hex(
      "BLAKE2b empty",
      hashlab::hex_lower(hashlab::blake2b(empty)),
      "786a02f742015903c6c6fd852552d272912f4740e15847618a86e217f71f5419"
      "d25e1031afee585313896444934eb04b903a685b1448b755d56f701afe9be2ce");
  ok &= expect_hex(
      "BLAKE2s empty",
      hashlab::hex_lower(hashlab::blake2s(empty)),
      "69217a3079908094e11121d042354a7c1f55b6482ca1a51e1b250dfd1ed0eef9");

  const auto crc = hashlab::crc16_ccitt(digits);
  if (crc != 0x29b1u) {
    std::cerr << "CRC16-CCITT mismatch\nexpected: 0x29b1\nactual  : 0x"
              << hashlab::hex_lower_int(crc) << '\n';
    ok = false;
  }

  return ok ? 0 : 1;
}
