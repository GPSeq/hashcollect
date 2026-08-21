# hashcollect

[![hashcollect](https://github.com/GPSeq/hashcollect/actions/workflows/c-cpp.yml/badge.svg?branch=main)](https://github.com/GPSeq/hashcollect/actions/workflows/c-cpp.yml)    

Hashcollect contains a collection of hash functions and checksums in C++20.

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

The demo uses one function for numeric and string inputs. Numeric values are
converted to stable little-endian storage, while strings are exposed directly
as a byte span:

```cpp
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <span>
#include <string_view>
#include <type_traits>
#include <typeinfo>
#include <vector>

#include "hashlab/common.hpp"
#include "hashlab/hash_xxh3.hpp"
#include "hashlab/hash_sha256.hpp"

template <typename T>
void hash(T value) {
  std::vector<std::byte> byte_storage;
  hashlab::bytespan bytes;

  if constexpr (
      std::is_same_v<std::remove_cvref_t<T>, int> ||
      std::is_same_v<std::remove_cvref_t<T>, float>) {
    byte_storage = hashlab::to_le_bytes(value);
    bytes = byte_storage;
  } else {
    bytes = hashlab::as_bytes(value);
  }

  const auto sha = hashlab::sha256(bytes);

  std::cout << "Value: " << value
            << " | type: " << typeid(value).name() << '\n';
  std::cout << "XXH3-64: 0x"
            << hashlab::hex_lower_int(hashlab::xxh3_64(bytes)) << '\n';
  std::cout << "SHA-256: 0x"
            << hashlab::hex_lower(std::span<const std::uint8_t>(sha)) << '\n';
}

int main() {
  hash<int>(123456789);
  hash<float>(3.1415926f);
  hash<std::string_view>("XXNNMMMACAAZT");
}
```

`typeid(value).name()` is implementation-defined. For example, AppleClang
prints `i` for `int` and `f` for `float`.

The complete demo in `apps/main.cpp` applies the same byte span to every
included hash and checksum.

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

## Demo output (excerpt)

```text
_______________________________________________________
Value: 123456789 | type: i
XXH3-64
  Value  : 0xc18468994276fb47
SHA-256
  Value  : 0x72de837c74b40716d430c711eebde10ff965fcc4a70c98e63a233ff36eebd6a1

_______________________________________________________
Value: 3.14159 | type: f
XXH3-64
  Value  : 0x92b50403bc48a675
SHA-256
  Value  : 0xf1af6ed5ca0314eaaccafa1bb8b213e19029ad6720a86c5f65f4a9df137e430a

_______________________________________________________
Value: XXNNMMMACAAZT | type: NSt3__117basic_string_viewIcNS_11char_traitsIcEEEE
XXH3-64
  Value  : 0x5ebe13a9f2964c71
SHA-256
  Value  : 0x949f5b06138eb5652e3a5ccb7d655f4e288b5393ea9d59661f16332b0f75954a
```

Please note the license from the sources included in the code.
