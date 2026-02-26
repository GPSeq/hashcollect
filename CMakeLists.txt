cmake_minimum_required(VERSION 3.20)
project(hashcollect LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(hashcollect
  src/hash_fnv1a32.cpp
  src/hash_fnv1a64.cpp
  src/hash_djb2.cpp
  src/hash_sdbm.cpp
  src/hash_jenkins_oaat.cpp
  src/hash_murmur2_32.cpp
  src/hash_murmur3_x86_32.cpp
  src/hash_murmur3_x64_128.cpp
  src/hash_xxhash32.cpp
  src/hash_xxhash64.cpp
  src/hash_siphash24.cpp
  src/checksum_crc32.cpp
  src/checksum_crc32c.cpp
  src/checksum_adler32.cpp
  src/checksum_crc64_ecma.cpp
  src/hash_pearson64.cpp
  src/hash_buzhash32.cpp
  src/hash_rabinkarp64.cpp
  src/hash_md5.cpp
  src/hash_sha256.cpp
  src/rolling_hash.cpp
)

target_include_directories(hashcollect PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_executable(hashcollect_demo apps/main.cpp)
target_link_libraries(hashcollect_demo PRIVATE hashcollect)

if (MSVC)
  target_compile_options(hashcollect PRIVATE /W4 /permissive-)
  target_compile_options(hashcollect_demo PRIVATE /W4 /permissive-)
else()
  target_compile_options(hashcollect PRIVATE -Wall -Wextra -Wpedantic)
  target_compile_options(hashcollect_demo PRIVATE -Wall -Wextra -Wpedantic)
endif()
