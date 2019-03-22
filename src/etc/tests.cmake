enable_testing()

add_test(NAME t_hash_and_try COMMAND "${PROJECT_SOURCE_DIR}/test/run_test.sh" "${CMAKE_BINARY_DIR}/hash_and_try" "${PROJECT_SOURCE_DIR}/test/test.sage" "try")
add_test(NAME t_hash_svdw_1 COMMAND "${PROJECT_SOURCE_DIR}/test/run_test.sh" "${CMAKE_BINARY_DIR}/hash_svdw_1" "${PROJECT_SOURCE_DIR}/test/test.sage" "1")
add_test(NAME t_hash_svdw_2 COMMAND "${PROJECT_SOURCE_DIR}/test/run_test.sh" "${CMAKE_BINARY_DIR}/hash_svdw_2" "${PROJECT_SOURCE_DIR}/test/test.sage" "2")
add_test(NAME t_hash_svdw_rG COMMAND "${PROJECT_SOURCE_DIR}/test/run_test.sh" "${CMAKE_BINARY_DIR}/hash_svdw_rG" "${PROJECT_SOURCE_DIR}/test/test.sage" "rG")
