enable_testing()

add_test(NAME t_hash_and_check COMMAND "${PROJECT_SOURCE_DIR}/test/run_test.sh" "${CMAKE_BINARY_DIR}/apps/hash_and_check" "${PROJECT_SOURCE_DIR}/test/test.sage" "hac")

add_test(NAME t_hash_svdw_1 COMMAND "${PROJECT_SOURCE_DIR}/test/run_test.sh" "${CMAKE_BINARY_DIR}/apps/hash_svdw_1" "${PROJECT_SOURCE_DIR}/test/test.sage" "1")
add_test(NAME t_hash_svdw_2 COMMAND "${PROJECT_SOURCE_DIR}/test/run_test.sh" "${CMAKE_BINARY_DIR}/apps/hash_svdw_2" "${PROJECT_SOURCE_DIR}/test/test.sage" "2")
add_test(NAME t_hash_svdw_rG COMMAND "${PROJECT_SOURCE_DIR}/test/run_test.sh" "${CMAKE_BINARY_DIR}/apps/hash_svdw_rG" "${PROJECT_SOURCE_DIR}/test/test.sage" "rG")

add_test(NAME t_hash_swu_1 COMMAND "${PROJECT_SOURCE_DIR}/test/run_test.sh" "${CMAKE_BINARY_DIR}/apps/hash_swu_1" "${PROJECT_SOURCE_DIR}/test/test.sage" "u1")
add_test(NAME t_hash_swu_2 COMMAND "${PROJECT_SOURCE_DIR}/test/run_test.sh" "${CMAKE_BINARY_DIR}/apps/hash_swu_2" "${PROJECT_SOURCE_DIR}/test/test.sage" "u2")
add_test(NAME t_hash_swu_rG COMMAND "${PROJECT_SOURCE_DIR}/test/run_test.sh" "${CMAKE_BINARY_DIR}/apps/hash_swu_rG" "${PROJECT_SOURCE_DIR}/test/test.sage" "urG")
