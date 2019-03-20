include_directories ("${PROJECT_SOURCE_DIR}/bint" "${PROJECT_SOURCE_DIR}/curve" "${PROJECT_SOURCE_DIR}/util")

add_library (bls_hash STATIC "${PROJECT_SOURCE_DIR}/util/util.c"
                             "${PROJECT_SOURCE_DIR}/bint/bint.c"
                             "${PROJECT_SOURCE_DIR}/curve/curve.c")

find_library (LIBGMP gmp)
find_library (LIBCRYPTO crypto)

macro (add_bh_exec exec_name)
    add_executable ("${exec_name}" "${exec_name}.c")
    target_link_libraries ("${exec_name}" bls_hash "${LIBGMP}" "${LIBCRYPTO}" ${ARGN})
endmacro (add_bh_exec)
