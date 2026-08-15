# hashcollect

Collection of hash functions and checksums in C++20.

## Build the demo

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j
./build/hashcollect_demo
```

To build only the library:

```bash
cmake -S . -B build -DHASHCOLLECT_BUILD_DEMO=OFF -DHASHCOLLECT_BUILD_TESTS=OFF
cmake --build build -j
```

## Use from another CMake project

If this repository is inside your project, for example at `external/hashcollect`:

```cmake
add_subdirectory(external/hashcollect)

add_executable(my_app main.cpp)
target_link_libraries(my_app PRIVATE hashcollect::hashcollect)
```

With `FetchContent`:

```cmake
include(FetchContent)

FetchContent_Declare(
  hashcollect
  GIT_REPOSITORY https://github.com/lutfia95/hashcollect.git
  GIT_TAG main
)

FetchContent_MakeAvailable(hashcollect)

add_executable(my_app main.cpp)
target_link_libraries(my_app PRIVATE hashcollect::hashcollect)
```

## C++ usage

```cpp
#include <cstdint>
#include <iostream>
#include <string_view>

#include "hashlab/common.hpp"
#include "hashlab/hash_xxh3.hpp"
#include "hashlab/hash_blake2b.hpp"
#include "hashlab/hash_sha512.hpp"
#include "hashlab/checksum_crc16_ccitt.hpp"

int main() {
  std::string_view text = "ACCGTAAATTTGCCCCCCX";
  hashlab::bytespan bytes = hashlab::as_bytes(text);

  std::uint64_t fast = hashlab::xxh3_64(bytes);
  auto blake = hashlab::blake2b(bytes);
  auto sha = hashlab::sha512(bytes);
  std::uint16_t crc = hashlab::crc16_ccitt(bytes);

  std::cout << "XXH3-64: 0x" << hashlab::hex_lower_int(fast) << '\n';
  std::cout << "BLAKE2b: 0x" << hashlab::hex_lower(blake) << '\n';
  std::cout << "SHA-512: 0x" << hashlab::hex_lower(sha) << '\n';
  std::cout << "CRC16  : 0x" << hashlab::hex_lower_int(crc) << '\n';
}
```

For non-string values, convert to stable little-endian bytes first:

```cpp
std::int32_t value = 123456789;
auto bytes = hashlab::to_le_bytes(value);
auto digest = hashlab::sha256(bytes);
```

## Included algorithms

Non-cryptographic hashes:

```text
FNV-1a-32, FNV-1a-64, DJB2-32, SDBM-32, Jenkins-OAAT-32,
Murmur2-32, Murmur3-x86-32, Murmur3-x86-128, Murmur3-x64-128,
xxHash32, xxHash64, XXH3-64, XXH3-128, Pearson64, Buzhash32,
RabinKarp64, RollingHash64
```

Keyed or cryptographic hashes:

```text
SipHash-2-4, MD5, SHA-256, SHA-512, BLAKE2b, BLAKE2s
```

Checksums:

```text
CRC16-CCITT, CRC32-IEEE, CRC32C, CRC64-ECMA, Adler32, Fletcher32
```

## Demo output

```text
Int: 123456789
Float: 3.14159
String: ACCGTAAATTTGCCCCCCX
FNV-1a-32
  int  : 0xa93bb346
  float: 0x87aa7230
  str  : 0x7746e3e4
FNV-1a-64
  int  : 0x09ddda58da364874
  float: 0x87e96bd298024c2a
  str  : 0x370f8518ed8aa727
DJB2-32
  int  : 0x49076c7c
  float: 0x97e3d47c
  str  : 0xf73ae30b
SDBM-32
  int  : 0x64a05c36
  float: 0x6cbd79a6
  str  : 0xf210aadf
Jenkins-OAAT-32
  int  : 0x42bcd882
  float: 0x11219ecf
  str  : 0xcdf8de43
Murmur2-32
  int  : 0xd3357788
  float: 0x7fd565f8
  str  : 0x0d77355a
Murmur3-x86-32
  int  : 0xaf2621bf
  float: 0x62fe623f
  str  : 0xaa7ea954
Murmur3-x64-128
  int  : 0xe4acbe68835748ffdcfe39b87ee4259e
  float: 0x20c3df807e3d6a25fa3fb5336e67b3f2
  str  : 0xcd00742eac0e49755ec767b40a9085fc
Murmur3-x86-128
  int  : 0x43ff3ba724596b1324596b1324596b13
  float: 0x23c77b9f42cd142f42cd142f42cd142f
  str  : 0x5a1e5ffe1a781e91e7a452ebe95b2d78
xxHash32
  int  : 0xee850ab2
  float: 0x8067d265
  str  : 0x601f9da4
xxHash64
  int  : 0xcafc526fc5f65083
  float: 0xd6d32d2aa2ab0b6e
  str  : 0xe8b5873b0f3e94ab
XXH3-64
  int  : 0xc18468994276fb47
  float: 0x92b50403bc48a675
  str  : 0x84a8fd734312c7db
XXH3-128
  int  : 0xc18468994276fb47d4bbae2c2e78334d
  float: 0x92b50403bc48a675c758a5624670712e
  str  : 0x84a8fd734312c7db1d9ab94f3db2201f
SipHash-2-4
  int  : 0x4811969c17cb24d5
  float: 0x90a58661f1a48268
  str  : 0xf53cb0c80e394594
CRC16-CCITT
  int  : 0x26e5
  float: 0xc997
  str  : 0xf652
CRC32-IEEE
  int  : 0x9e6cdf18
  float: 0x00749634
  str  : 0x6a7ba70c
CRC32C
  int  : 0xc5a603f5
  float: 0x9ef65d6d
  str  : 0x7c2bb8c0
Adler32
  int  : 0x45017c03
  float: 0x73016b04
  str  : 0x5305cb34
Fletcher32
  int  : 0xd470a186
  float: 0x50235ffd
  str  : 0x72e4b1a9
CRC64-ECMA
  int  : 0xbfcdcd79aeeac099
  float: 0xbf08ae0fc06a156e
  str  : 0x69fbe6fedd8aa2c3
Pearson64
  int  : 0x1347f4101f275f5a
  float: 0xeb06fc79152b57a6
  str  : 0x923b4267a9f0703c
Buzhash32
  int  : 0x2fcb91e0
  float: 0xc237c988
  str  : 0x1612a655
RabinKarp64
  int  : 0xe4a1de05090bca86
  float: 0xd4308a256157f6c6
  str  : 0x82937cb31436593a
RollingHash64
  int  : 0xe4a1de05090bca86
  float: 0xd4308a256157f6c6
  str  : 0x82937cb31436593a
MD5
  int  : 0x623ad5eda301ae082d466141a7f6dcaf
  float: 0xad14c956e63f2313841f85c817947889
  str  : 0x4e5b0b3b13b87e107f119cb03dabbf7c
SHA-256
  int  : 0x72de837c74b40716d430c711eebde10ff965fcc4a70c98e63a233ff36eebd6a1
  float: 0xf1af6ed5ca0314eaaccafa1bb8b213e19029ad6720a86c5f65f4a9df137e430a
  str  : 0x1e976226fbce9774aab5dcd3548f8a9d9ae21c57f2b2d0a82579ea4523915714
SHA-512
  int  : 0x0e624dc64f6c00f56f5c78a856634d30e0dd8e210e8e43456ef12f8df77aae015aafcc24be2eef8e48175a3734b283b93fdb34ddd187352b4865cb645e448750
  float: 0xa7812f35897bbf8983e6e5acf92e4a0db4ad46069a920aa667ac8582e0a2d7dcf31e97f699c9a6a4721e07cf76d28c1f05fd2ba2917339c295cb3f1185f526d8
  str  : 0x9052c78e3316e9993fc9902b38908bf3d362aecbb50999ee29d3bee06b60c3726eaa45817bed43f1d9529b0ed4e36701ff6c0e8194d969f4929df8d0c63d09c8
BLAKE2b
  int  : 0xfd6b570ce158f1efdb094f1bb8d0279fd3eedc35baac61ace2415ebe1382370d19afcfae4f5fdefee01e381e841d80ee8fa2973c09eb68dd2dec59c581face90
  float: 0x8c02ea63bae3d07cbee66c1cdb0811425dd6761c184d7bf1e7a7fbc4f6402d7e7ae0969ff6c333008d66f900dbde96f050442a9c55a4614911d217daf9aae914
  str  : 0xd95bc05026052e947c934c3dfdea60dc46e4d934a91baf5272f6d34277db9cc384ce97b3ef1185f1225a65b86a9981a25b89bc940eb8e2dee94b77d315ef4fce
BLAKE2s
  int  : 0x22dc42db3f46ebfa7449f05e79ff16d7d64d5a09398a2a7837c4a0bdd730b838
  float: 0x281613428fe2ceb844d04f5c0341ef678b0b39dc96bdacf40f2bbe42d759aba5
  str  : 0xf1157c8a39b5c399b77601c298ed8cc1271add48a619d3d3e72588e261f2a5c9
```

Please note the license from the sources included in the code.
