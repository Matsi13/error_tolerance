cmake_minimum_required(VERSION 3.13.4)
project(error_tolerance)
set(CMAKE_CXX_STANDARD 23)

set(INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)
include_directories(${INCLUDE_DIR})

add_subdirectory(src)


add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} total_lib)


include(CTest)
add_subdirectory(test)


