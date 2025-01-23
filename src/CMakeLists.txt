add_library(memory_lib 
Memory.cpp
)

add_library(compute_lib 
Compute.cpp
)

add_library(communication_lib 
Communication.cpp
)

add_library(Die_lib 
Memory.cpp
Compute.cpp
Communication.cpp
Die.cpp
)

add_library(permutation_lib 
Memory.cpp
Compute.cpp
Communication.cpp
Die.cpp
Permutation.cpp
)

add_library(distance_lib 
Memory.cpp
Compute.cpp
Communication.cpp
Die.cpp
Distance.cpp
)