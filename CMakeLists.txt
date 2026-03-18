cmake_minimum_required(VERSION 3.13.4)
project(error_tolerance)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_BUILD_TYPE Debug)

set(INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)
include_directories(${INCLUDE_DIR})

add_subdirectory(src)


add_executable(${PROJECT_NAME} main.cpp)
# target_link_libraries(${PROJECT_NAME} total_lib)

add_executable(simulated_annealing simulated_annealing_main.cpp)
target_link_libraries(simulated_annealing simulated_annealing_lib generate_lib astra_API_lib)


include(CTest)
add_subdirectory(test)


