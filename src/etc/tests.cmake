enable_testing()

add_test(NAME t_hash_and_check COMMAND "${PROJECT_SOURCE_DIR}/test/run_test.sh" "${CMAKE_BINARY_DIR}/apps/hash_and_check" "${PROJECT_SOURCE_DIR}/test/test.sage" hac)
add_test(NAME t_hash_and_check_fo COMMAND "${PROJECT_SOURCE_DIR}/test/run_test.sh" "${CMAKE_BINARY_DIR}/apps/hash_and_check" "${PROJECT_SOURCE_DIR}/test/test.sage" hac f)

add_test(NAME t_hash_svdw_1 COMMAND "${PROJECT_SOURCE_DIR}/test/run_test.sh" "${CMAKE_BINARY_DIR}/apps/hash_svdw_1" "${PROJECT_SOURCE_DIR}/test/test.sage" 1)
add_test(NAME t_hash_svdw_2 COMMAND "${PROJECT_SOURCE_DIR}/test/run_test.sh" "${CMAKE_BINARY_DIR}/apps/hash_svdw_2" "${PROJECT_SOURCE_DIR}/test/test.sage" 2)
add_test(NAME t_hash_svdw_rG COMMAND "${PROJECT_SOURCE_DIR}/test/run_test.sh" "${CMAKE_BINARY_DIR}/apps/hash_svdw_rG" "${PROJECT_SOURCE_DIR}/test/test.sage" rG)

add_test(NAME t_hash_svdw_1_fo COMMAND "${PROJECT_SOURCE_DIR}/test/run_test.sh" "${CMAKE_BINARY_DIR}/apps/hash_svdw_1" "${PROJECT_SOURCE_DIR}/test/test.sage" 1 f)
add_test(NAME t_hash_svdw_2_fo COMMAND "${PROJECT_SOURCE_DIR}/test/run_test.sh" "${CMAKE_BINARY_DIR}/apps/hash_svdw_2" "${PROJECT_SOURCE_DIR}/test/test.sage" 2 f)
add_test(NAME t_hash_svdw_rG_fo COMMAND "${PROJECT_SOURCE_DIR}/test/run_test.sh" "${CMAKE_BINARY_DIR}/apps/hash_svdw_rG" "${PROJECT_SOURCE_DIR}/test/test.sage" rG f)

add_test(NAME t_hash_svdw_1_ct COMMAND "${PROJECT_SOURCE_DIR}/test/run_test.sh" "${CMAKE_BINARY_DIR}/apps/hash_svdw_1" "${PROJECT_SOURCE_DIR}/test/test.sage" 1 c)
add_test(NAME t_hash_svdw_2_ct COMMAND "${PROJECT_SOURCE_DIR}/test/run_test.sh" "${CMAKE_BINARY_DIR}/apps/hash_svdw_2" "${PROJECT_SOURCE_DIR}/test/test.sage" 2 c)

add_test(NAME t_hash_swu_1 COMMAND "${PROJECT_SOURCE_DIR}/test/run_test.sh" "${CMAKE_BINARY_DIR}/apps/hash_swu_1" "${PROJECT_SOURCE_DIR}/test/test.sage" u1)
add_test(NAME t_hash_swu_2 COMMAND "${PROJECT_SOURCE_DIR}/test/run_test.sh" "${CMAKE_BINARY_DIR}/apps/hash_swu_2" "${PROJECT_SOURCE_DIR}/test/test.sage" u2)
add_test(NAME t_hash_swu_rG COMMAND "${PROJECT_SOURCE_DIR}/test/run_test.sh" "${CMAKE_BINARY_DIR}/apps/hash_swu_rG" "${PROJECT_SOURCE_DIR}/test/test.sage" urG)

add_test(NAME t_hash_swu_1_ct COMMAND "${PROJECT_SOURCE_DIR}/test/run_test.sh" "${CMAKE_BINARY_DIR}/apps/hash_swu_1" "${PROJECT_SOURCE_DIR}/test/test.sage" u1 c)
add_test(NAME t_hash_swu_2_ct COMMAND "${PROJECT_SOURCE_DIR}/test/run_test.sh" "${CMAKE_BINARY_DIR}/apps/hash_swu_2" "${PROJECT_SOURCE_DIR}/test/test.sage" u2 c)

add_test(NAME t_hash2_and_check COMMAND "${PROJECT_SOURCE_DIR}/test/run_test.sh" "${CMAKE_BINARY_DIR}/apps/hash2_and_check" "${PROJECT_SOURCE_DIR}/test/g2_test.sage" hac)
add_test(NAME t_hash2_and_check_fo COMMAND "${PROJECT_SOURCE_DIR}/test/run_test.sh" "${CMAKE_BINARY_DIR}/apps/hash2_and_check" "${PROJECT_SOURCE_DIR}/test/g2_test.sage" hac f)

add_test(NAME t_hash2_svdw_1 COMMAND "${PROJECT_SOURCE_DIR}/test/run_test.sh" "${CMAKE_BINARY_DIR}/apps/hash2_svdw_1" "${PROJECT_SOURCE_DIR}/test/g2_test.sage" 1 "" 128)
add_test(NAME t_hash2_svdw_2 COMMAND "${PROJECT_SOURCE_DIR}/test/run_test.sh" "${CMAKE_BINARY_DIR}/apps/hash2_svdw_2" "${PROJECT_SOURCE_DIR}/test/g2_test.sage" 2 "" 64)

add_custom_target (bench COMMAND "${PROJECT_SOURCE_DIR}/test/run_bench.sh" "${CMAKE_BINARY_DIR}")
