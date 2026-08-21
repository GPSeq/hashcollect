// SPDX-License-Identifier: MIT
#include <array>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <string_view>

#include "hashlab/checksum_adler32.hpp"
#include "hashlab/checksum_crc16_ccitt.hpp"
#include "hashlab/checksum_crc32.hpp"
#include "hashlab/checksum_crc32c.hpp"
#include "hashlab/checksum_crc64_ecma.hpp"
#include "hashlab/checksum_fletcher32.hpp"
#include "hashlab/common.hpp"
#include "hashlab/hash_blake2b.hpp"
#include "hashlab/hash_blake2s.hpp"
#include "hashlab/hash_buzhash32.hpp"
#include "hashlab/hash_djb2.hpp"
#include "hashlab/hash_fnv1a32.hpp"
#include "hashlab/hash_fnv1a64.hpp"
#include "hashlab/hash_jenkins_oaat.hpp"
#include "hashlab/hash_md5.hpp"
#include "hashlab/hash_murmur2_32.hpp"
#include "hashlab/hash_murmur3_x64_128.hpp"
#include "hashlab/hash_murmur3_x86_128.hpp"
#include "hashlab/hash_murmur3_x86_32.hpp"
#include "hashlab/hash_pearson64.hpp"
#include "hashlab/hash_rabinkarp64.hpp"
#include "hashlab/hash_sdbm.hpp"
#include "hashlab/hash_sha256.hpp"
#include "hashlab/hash_sha512.hpp"
#include "hashlab/hash_siphash24.hpp"
#include "hashlab/hash_xxh3.hpp"
#include "hashlab/hash_xxhash32.hpp"
#include "hashlab/hash_xxhash64.hpp"
#include "hashlab/rolling_hash.hpp"

static bool expect_hex(const char* name, const std::string& actual, std::string_view expected) {
  if (actual == expected) return true;
  std::cerr << name << " mismatch\nexpected: " << expected << "\nactual  : " << actual << '\n';
  return false;
}

template <typename UInt>
static bool expect_value(const char* name, UInt actual, std::uint64_t expected) {
  const auto actual_value = static_cast<std::uint64_t>(actual);
  if (actual_value == expected) return true;
  std::cerr << name << " mismatch\nexpected: 0x" << std::hex
            << expected << "\nactual  : 0x" << actual_value << std::dec << '\n';
  return false;
}

int main() {
  const auto empty = hashlab::as_bytes("");
  const auto digits = hashlab::as_bytes("123456789");
  const std::array<std::uint8_t, 16> sipkey = {
      0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
  bool ok = true;

  // Non-cryptographic hashes.
  ok &= expect_value("FNV-1a-32", hashlab::fnv1a32(digits), 0xbb86b11cu);
  ok &= expect_value("FNV-1a-64", hashlab::fnv1a64(digits), 0x06d5573923c6cdfcULL);
  ok &= expect_value("DJB2-32", hashlab::djb2_32(digits), 0x35cdbb82u);
  ok &= expect_value("SDBM-32", hashlab::sdbm_32(digits), 0x68a07035u);
  ok &= expect_value("Jenkins-OAAT-32", hashlab::jenkins_oaat32(digits), 0xc66b58c5u);
  ok &= expect_value("Murmur2-32", hashlab::murmur2_32(digits), 0x9362de66u);
  ok &= expect_value("Murmur3-x86-32", hashlab::murmur3_x86_32(digits), 0xb4fef382u);
  ok &= expect_hex(
      "Murmur3-x86-128",
      hashlab::hex_lower(hashlab::murmur3_x86_128(digits)),
      "bb7658c652159a11d7e5e3c5a48c16a9");
  ok &= expect_hex(
      "Murmur3-x64-128",
      hashlab::hex_lower(hashlab::murmur3_x64_128(digits)),
      "a4cc66db5e64843c05a11e3ac7faf899");
  ok &= expect_value("xxHash32", hashlab::xxhash32(digits), 0x937bad67u);
  ok &= expect_value("xxHash64", hashlab::xxhash64(digits), 0x8cb841db40e6ae83ULL);
  ok &= expect_value("XXH3-64", hashlab::xxh3_64(digits), 0x72dcb18b67a17dffULL);
  ok &= expect_hex(
      "XXH3-128",
      hashlab::hex_lower(hashlab::xxh3_128(digits)),
      "ff7da1678bb1dc728d19986413517f34");
  ok &= expect_value("Pearson64", hashlab::pearson64(digits), 0x6883644936ce55c9ULL);
  ok &= expect_value("Buzhash32", hashlab::buzhash32(digits), 0x5e5c8ca2u);
  ok &= expect_value("RabinKarp64", hashlab::rabinkarp64(digits), 0xab4b1b2f47bde8b5ULL);
  ok &= expect_value("RollingHash64", hashlab::rolling_hash(digits), 0xab4b1b2f47bde8b5ULL);

  // Checksums.
  ok &= expect_value("CRC16-CCITT", hashlab::crc16_ccitt(digits),
                     static_cast<std::uint16_t>(0x29b1u));
  ok &= expect_value("CRC32-IEEE", hashlab::crc32_ieee(digits), 0xcbf43926u);
  ok &= expect_value("CRC32C", hashlab::crc32c(digits), 0xe3069283u);
  ok &= expect_value("Adler32", hashlab::adler32(digits), 0x091e01deu);
  ok &= expect_value("Fletcher32", hashlab::fletcher32(digits), 0x09df09d5u);
  ok &= expect_value("CRC64-ECMA", hashlab::crc64_ecma(digits), 0x995dc9bbdf1939faULL);

  // Keyed and cryptographic hashes.
  ok &= expect_value("SipHash-2-4", hashlab::siphash24(digits, sipkey),
                     0xca60fc96020efefdULL);
  ok &= expect_hex(
      "MD5 digits",
      hashlab::hex_lower(hashlab::md5(digits)),
      "25f9e794323b453885f5181f1b624d0b");
  ok &= expect_hex(
      "SHA-256 digits",
      hashlab::hex_lower(hashlab::sha256(digits)),
      "15e2b0d3c33891ebb0f1ef609ec419420c20e320ce94c65fbc8c3312448eb225");
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

  return ok ? 0 : 1;
}
