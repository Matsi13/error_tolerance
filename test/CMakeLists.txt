add_executable(Memory_test Memory_test.cpp)
target_link_libraries(Memory_test total_lib)
add_test(NAME Memory_test COMMAND Memory_test)

add_executable(Compute_test Compute_test.cpp)
target_link_libraries(Compute_test total_lib)
add_test(NAME Compute_test COMMAND Compute_test)
