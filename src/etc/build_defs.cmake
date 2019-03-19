find_library (LIBGMP gmp)
find_library (LIBCRYPTO crypto)
macro (add_bh_exec exec_name)
    add_executable ("${exec_name}" "${exec_name}.c"
                                   "${PROJECT_SOURCE_DIR}/bhcommon/common.c"
                                   "${PROJECT_SOURCE_DIR}/bint/bint.c")
    target_link_libraries ("${exec_name}" "${LIBGMP}" "${LIBCRYPTO}" ${ARGN})
endmacro (add_bh_exec)
