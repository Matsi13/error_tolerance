#!/usr/bin/env bash

# Simulated Annealing Wafer Optimization Script
# This script runs simulated annealing to find optimal wafer configuration using ns-3 simulator

# Configuration
config_file="./configuration/auto_generate.txt"
temp_dir="./temp_sa"
astra_sim_path="/home/matsi/workspace/astra-sim"
system_config="./configuration/default_system.json"
remote_memory_config="./configuration/default_remote_memory.json"
logical_network_config="./configuration/default_logical_network.json"

# Create necessary directories
mkdir -p "$temp_dir"

# ns-3 command template
# Placeholders: {WORKLOAD}, {NETWORK}, {OUTPUT}
ns3_command_template="$astra_sim_path/extern/network_backend/ns-3/build/scratch/ns3.42-AstraSimNetwork-default \
    --workload-configuration={WORKLOAD} \
    --system-configurations=$system_config \
    --network-configuration={NETWORK} \
    --remote-memory-configuration=$remote_memory_config \
    --logical-topology-configuration=$logical_network_config \
    --comm-group-configuration=empty \
    > {OUTPUT} 2>&1"

echo "=== Simulated Annealing Wafer Optimization ==="
echo "Configuration file: $config_file"
echo "Temporary directory: $temp_dir"
echo "Astra-sim path: $astra_sim_path"
echo ""

# Check if executable exists
if [ ! -f "./simulated_annealing" ]; then
    echo "Error: simulated_annealing executable not found!"
    echo "Please build the project first:"
    echo "  mkdir build && cd build"
    echo "  cmake .."
    echo "  make simulated_annealing"
    exit 1
fi

# Run simulated annealing
./simulated_annealing "$config_file" "$ns3_command_template" "$temp_dir"

echo ""
echo "=== Optimization Complete ==="
echo "Results saved in: $temp_dir"
echo "  - sa_optimal_wafer.txt: Best wafer configuration found"
echo "  - sa_trajectory.txt: Search trajectory (all visited configurations)"
echo "  - sa_eval_*.txt: Individual ns-3 simulation outputs"
