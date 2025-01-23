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
target_link_libraries(Die_test Die_lib)
add_test(NAME Die_test COMMAND Die_test)

