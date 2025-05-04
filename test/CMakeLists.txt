add_executable(Memory_test Memory_test.cpp)
target_link_libraries(Memory_test memory_lib)
add_test(NAME Memory_test COMMAND Memory_test)

add_executable(Compute_test Compute_test.cpp)
target_link_libraries(Compute_test compute_lib)
add_test(NAME Compute_test COMMAND Compute_test)

add_executable(Communication_test Communication_test.cpp)
target_link_libraries(Communication_test communication_lib)
add_test(NAME Communication_test COMMAND Communication_test)

add_executable(Die_test Die_test.cpp)
target_link_libraries(Die_test die_lib)
add_test(NAME Die_test COMMAND Die_test)

add_executable(Permutation_test Permutation_test.cpp)
target_link_libraries(Permutation_test permutation_lib)
add_test(NAME Permutation_test COMMAND Permutation_test)

add_executable(Scan_test Scan_test.cpp)
target_link_libraries(Scan_test scan_lib permutation_lib)
add_test(NAME Scan_test COMMAND Scan_test)

add_executable(Generate_test Generate_test.cpp)
target_link_libraries(Generate_test generate_lib permutation_lib)
add_test(NAME Generate_test COMMAND Generate_test)

add_executable(Attention_test Attention_test.cpp)
target_link_libraries(Attention_test attention_lib)
add_test(NAME Attention_test COMMAND Attention_test)

add_executable(Pipeline_test Pipeline_test.cpp)
target_link_libraries(Pipeline_test pipeline_lib)
add_test(NAME Pipeline_test COMMAND Pipeline_test)

add_executable(Recompute_test Recompute_test.cpp)
target_link_libraries(Recompute_test recompute_lib)
add_test(NAME Recompute_test COMMAND Recompute_test)

