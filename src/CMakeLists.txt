cmake_minimum_required (VERSION 2.8.5)
cmake_policy (SET CMP0054 NEW)
project (bls_hash)

include (etc/build_defs.cmake)
include (etc/build_type.cmake)
include (etc/cflags.cmake)

include (etc/clang_format.cmake)
include (etc/clang_tidy.cmake)
include (etc/cppcheck.cmake)

include_directories ("${PROJECT_SOURCE_DIR}/bint" "${PROJECT_SOURCE_DIR}/bhcommon")

add_bh_exec (hash_and_try)
