set (CMAKE_C_STANDARD 11)
set (CMAKE_EXPORT_COMPILE_COMMANDS ON)
set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pedantic -pedantic-errors -Werror -Wall -Wextra -Wshadow -Wpointer-arith -Wcast-qual -Wformat=2 -Wstrict-prototypes -Wmissing-prototypes")

# check for supported compiler versions
set (IS_GNU_COMPILER ("${CMAKE_C_COMPILER_ID}" STREQUAL "GNU"))
set (IS_CLANG_COMPILER ("${CMAKE_C_COMPILER_ID}" MATCHES "[Cc][Ll][Aa][Nn][Gg]"))
set (C_VERSION_LT_6 ("${CMAKE_C_COMPILER_VERSION}" VERSION_LESS 6))
set (C_VERSION_LT_8 ("${CMAKE_C_COMPILER_VERSION}" VERSION_LESS 8))
if ((${IS_GNU_COMPILER} AND ${C_VERSION_LT_8}) OR (${IS_CLANG_COMPILER} AND ${C_VERSION_LT_6}))
    message (FATAL_ERROR "You must compile this project with g++ >= 8 or clang >= 6.")
endif ()
if (${IS_CLANG_COMPILER})
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wloop-analysis")
endif ()

# add some flags for the Release, Debug, and DebugSan modes
set (CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -flto -funroll-all-loops")
set (CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -ggdb3 -Og")
set (CMAKE_C_FLAGS_DEBUGASAN "${CMAKE_C_FLAGS_DEBUG} -fsanitize=undefined -fsanitize=address")
set (CMAKE_C_FLAGS_DEBUGTSAN "${CMAKE_C_FLAGS_DEBUG} -fsanitize=thread")
set (CMAKE_C_FLAGS_RELASAN "${CMAKE_C_FLAGS_RELEASE} -fsanitize=undefined -fsanitize=address")
set (CMAKE_C_FLAGS_RELTSAN "${CMAKE_C_FLAGS_RELEASE} -fsanitize=thread")
