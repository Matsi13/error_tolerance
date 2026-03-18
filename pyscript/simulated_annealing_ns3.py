import os
import sys
import subprocess
import json
import re

def create_system_json(mem_bandwidth, output_path):
    """Create system.json configuration file"""
    system_config = {
        "scheduling-policy": "LIFO",
        "intra-dimension-scheduling": "FIFO",
        "inter-dimension-scheduling": "baseline",
        "endpoint-delay": 10,
        "active-chunks-per-dimension": 1,
        "preferred-dataset-splits": 1,
        "all-reduce-implementation": ["ring", "ring"],
        "all-gather-implementation": ["ring", "ring"],
        "reduce-scatter-implementation": ["ring", "ring"],
        "all-to-all-implementation": ["ring", "ring"],
        "local_mem_bw": mem_bandwidth,
        "boost_mode": 0
    }
    
    with open(output_path, 'w') as f:
        json.dump(system_config, f, indent=2)

def create_logical_network_json(rows, columns, output_path):
    """Create logical_network.json configuration file"""
    logical_config = {
        "topology": ["Ring", "Ring"],
        "npus_count": [columns, rows],
        "bandwidth": [100.0, 100.0],
        "latency": [10.0, 10.0]
    }
    
    with open(output_path, 'w') as f:
        json.dump(logical_config, f, indent=2)

def create_remote_memory_json(output_path):
    """Create remote_memory.json configuration file"""
    remote_config = {
        "remote_memory_enabled": False,
        "remote_memory_latency": 100,
        "remote_memory_bandwidth": 10.0
    }
    
    with open(output_path, 'w') as f:
        json.dump(remote_config, f, indent=2)

def run_ns3_simulation(workload_file, network_file, system_file, remote_file, logical_file, 
                       astra_sim_path, output_file):
    """Run ns-3 simulation and return max cycle count"""
    
    ns3_executable = os.path.join(astra_sim_path, 
                                  "extern/network_backend/ns-3/build/scratch/ns3.42-AstraSimNetwork-default")
    
    if not os.path.exists(ns3_executable):
        print(f"Error: ns-3 executable not found at {ns3_executable}")
        return None
    
    cmd = [
        ns3_executable,
        f"--workload-configuration={workload_file}",
        f"--system-configurations={system_file}",
        f"--network-configuration={network_file}",
        f"--remote-memory-configuration={remote_file}",
        f"--logical-topology-configuration={logical_file}",
        "--comm-group-configuration=empty"
    ]
    
    try:
        with open(output_file, 'w') as f:
            subprocess.run(cmd, stdout=f, stderr=subprocess.STDOUT, check=True)
        
        # Parse output to get max cycles
        max_cycles = 0
        with open(output_file, 'r') as f:
            for line in f:
                match = re.search(r'sys\[(\d+)\] finished, (\d+) cycles', line)
                if match:
                    cycles = int(match.group(2))
                    max_cycles = max(max_cycles, cycles)
        
        return max_cycles
    
    except subprocess.CalledProcessError as e:
        print(f"Error running ns-3 simulation: {e}")
        return None

def simulated_annealing_with_ns3(config_file, astra_sim_path, temp_dir, 
                                 initial_temp=1000.0, cooling_rate=0.95, 
                                 min_temp=1.0, iterations_per_temp=5, max_iterations=200):
    """
    Run simulated annealing optimization using ns-3 as the simulator
    
    This is a Python wrapper that can be used as an alternative to the C++ implementation
    """
    
    print("=== Python Simulated Annealing with ns-3 ===")
    print(f"Config file: {config_file}")
    print(f"Astra-sim path: {astra_sim_path}")
    print(f"Temp directory: {temp_dir}")
    print()
    
    # Create temp directory
    os.makedirs(temp_dir, exist_ok=True)
    
    # Create default configuration files
    system_file = os.path.join(temp_dir, "default_system.json")
    remote_file = os.path.join(temp_dir, "default_remote_memory.json")
    logical_file = os.path.join(temp_dir, "default_logical_network.json")
    
    create_system_json(100.0, system_file)  # Default memory bandwidth
    create_remote_memory_json(remote_file)
    create_logical_network_json(4, 4, logical_file)  # Default 4x4 grid
    
    print("Configuration files created")
    print(f"  System: {system_file}")
    print(f"  Remote memory: {remote_file}")
    print(f"  Logical network: {logical_file}")
    print()
    
    # Note: The actual SA algorithm would need to:
    # 1. Generate initial wafer configuration
    # 2. Create workload and network files
    # 3. Call run_ns3_simulation()
    # 4. Generate neighbors and evaluate
    # 5. Accept/reject based on Metropolis criterion
    
    print("Note: This is a template. The full implementation requires:")
    print("  1. Wafer configuration generation (from C++ or reimplemented in Python)")
    print("  2. Workload file generation based on wafer specs")
    print("  3. Physical network file generation")
    print("  4. Integration with the SA algorithm")
    print()
    print("For now, use the C++ implementation: ./simulated_annealing")

if __name__ == "__main__":
    if len(sys.argv) < 3:
        print("Usage: python simulated_annealing_ns3.py <config_file> <astra_sim_path> [temp_dir]")
        print("Example: python simulated_annealing_ns3.py ./configuration/auto_generate.txt /home/user/astra-sim ./temp_sa")
        sys.exit(1)
    
    config_file = sys.argv[1]
    astra_sim_path = sys.argv[2]
    temp_dir = sys.argv[3] if len(sys.argv) > 3 else "./temp_sa"
    
    simulated_annealing_with_ns3(config_file, astra_sim_path, temp_dir)
