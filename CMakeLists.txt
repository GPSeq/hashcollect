cmake_minimum_required(VERSION 3.20)
project(hashcollect LANGUAGES CXX)

if (CMAKE_SOURCE_DIR STREQUAL PROJECT_SOURCE_DIR)
  set(HASHCOLLECT_IS_TOP_LEVEL ON)
else()
  set(HASHCOLLECT_IS_TOP_LEVEL OFF)
endif()

option(HASHCOLLECT_BUILD_DEMO "Build the hashcollect demo executable" ${HASHCOLLECT_IS_TOP_LEVEL})
option(HASHCOLLECT_BUILD_TESTS "Build hashcollect tests" ${HASHCOLLECT_IS_TOP_LEVEL})

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
  src/hash_murmur3_x86_128.cpp
  src/hash_murmur3_x64_128.cpp
  src/hash_xxhash32.cpp
  src/hash_xxhash64.cpp
  src/hash_xxh3.cpp
  src/hash_siphash24.cpp
  src/checksum_crc32.cpp
  src/checksum_crc32c.cpp
  src/checksum_crc16_ccitt.cpp
  src/checksum_adler32.cpp
  src/checksum_fletcher32.cpp
  src/checksum_crc64_ecma.cpp
  src/hash_pearson64.cpp
  src/hash_buzhash32.cpp
  src/hash_rabinkarp64.cpp
  src/hash_md5.cpp
  src/hash_sha256.cpp
  src/hash_sha512.cpp
  src/hash_blake2b.cpp
  src/hash_blake2s.cpp
  src/rolling_hash.cpp
)

target_include_directories(hashcollect PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
add_library(hashcollect::hashcollect ALIAS hashcollect)

if (HASHCOLLECT_BUILD_DEMO)
  add_executable(hashcollect_demo apps/main.cpp)
  target_link_libraries(hashcollect_demo PRIVATE hashcollect::hashcollect)
endif()

if (HASHCOLLECT_BUILD_TESTS)
  enable_testing()
  add_executable(hashcollect_tests tests/hash_vectors.cpp)
  target_link_libraries(hashcollect_tests PRIVATE hashcollect::hashcollect)
  add_test(NAME hashcollect_vectors COMMAND hashcollect_tests)
endif()

if (MSVC)
  target_compile_options(hashcollect PRIVATE /W4 /permissive-)
  if (HASHCOLLECT_BUILD_DEMO)
    target_compile_options(hashcollect_demo PRIVATE /W4 /permissive-)
  endif()
  if (HASHCOLLECT_BUILD_TESTS)
    target_compile_options(hashcollect_tests PRIVATE /W4 /permissive-)
  endif()
else()
  target_compile_options(hashcollect PRIVATE -Wall -Wextra -Wpedantic)
  if (HASHCOLLECT_BUILD_DEMO)
    target_compile_options(hashcollect_demo PRIVATE -Wall -Wextra -Wpedantic)
  endif()
  if (HASHCOLLECT_BUILD_TESTS)
    target_compile_options(hashcollect_tests PRIVATE -Wall -Wextra -Wpedantic)
  endif()
endif()
