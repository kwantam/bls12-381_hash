file (GLOB LIB_SOURCES "*.c")
add_library (bint STATIC ${LIB_SOURCES})
set (CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -O3 -flto -funroll-all-loops")
