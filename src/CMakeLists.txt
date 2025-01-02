add_library(memory_lib 
Memory.cpp
)

add_library(compute_lib 
Compute.cpp
)

add_library(communication_lib 
Communication.cpp
)

add_library(total_lib
Memory.cpp
Compute.cpp
Communication.cpp
)