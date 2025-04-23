add_library(memory_lib 
Memory.cpp
)

add_library(compute_lib 
Compute.cpp
)

add_library(communication_lib 
Communication.cpp
)

add_library(die_lib 
Memory.cpp
Compute.cpp
Communication.cpp
Die.cpp
)

add_library(wafer_lib 
Memory.cpp
Compute.cpp
Communication.cpp
Die.cpp
Wafer.cpp
)

add_library(permutation_lib 
Memory.cpp
Compute.cpp
Communication.cpp
Die.cpp
Wafer.cpp
Permutation.cpp
)

add_library(distance_lib 
Memory.cpp
Compute.cpp
Communication.cpp
Die.cpp
Wafer.cpp
Distance.cpp
)

add_library(scan_lib 
Memory.cpp
Compute.cpp
Communication.cpp
Die.cpp
Wafer.cpp
Scan.cpp
)

add_library(generate_lib 
Memory.cpp
Compute.cpp
Communication.cpp
Generate.cpp
)

add_library(workload_lib
Workload.cpp
)

add_library(attention_lib
Workload.cpp
Attention.cpp
)

