# Error Tolerance: Wafer-Scale Architecture Design Space Exploration

A comprehensive C++ framework for exploring wafer-scale architectures and identifying optimal configurations for AI workloads through design space exploration, analytical modeling, and cycle-accurate simulation.

## What This Project Does

This tool addresses a critical challenge in AI hardware design: **finding optimal wafer-scale architectures that are robust to simulator errors**. It combines three complementary approaches:

1. **Exhaustive Design Space Exploration** - Enumerate all feasible die/wafer configurations
2. **Analytical Workload Modeling** - Model LLM transformer workloads with various parallelization strategies
3. **Cycle-Accurate Simulation** - Validate designs using ns-3 network simulation via astra-sim

The key innovation is **error tolerance analysis**: identifying which architectures remain optimal even when simulator metrics vary within error bounds.

## Core Concepts

### Architecture Hierarchy

```
Wafer (200mm × 200mm)
  ├─ Die Grid (rows × columns)
  │   └─ Die (≤33mm × 28mm reticle limit)
  │       ├─ Compute Core (e.g., 32mm × 24mm, 38.4 TFLOPS)
  │       ├─ Memory Units (HBM, attached to edges)
  │       └─ Communication Units (D2D interconnect, attached to edges)
```

### Key Constraints

- **Reticle Limit**: Die size ≤ 33mm × 28mm (manufacturing constraint)
- **D2D Bandwidth**: Scales from 20 TB/s (small die) to 10 TB/s (large die) based on perimeter
- **Bandwidth Allocation**: Total bandwidth split between memory and communication via `memory_bandwidth_ratio`
- **Performance Threshold**: Minimum TFLOPS, memory capacity, and bandwidth requirements

### Permutation Encoding

Die edges are encoded as strings where:
- `'0'` = Memory unit
- `'1'` = Communication unit  
- `'9'` = Delimiter (empty edge)

Example: `"00011111119"` = 2 memory units + 8 communication units on one edge

## Project Structure

```
error_tolerance/
├── src/                      # C++ implementation (18 files)
│   ├── Permutation.cpp       # Design space exploration & Pareto pruning
│   ├── Die.cpp               # Die modeling with constraints
│   ├── Wafer.cpp             # Wafer aggregation
│   ├── Attention.cpp         # LLM transformer workload modeling
│   ├── Pipeline.cpp          # Pipeline parallelism
│   ├── Recompute.cpp         # Activation checkpointing
│   ├── Analysis.cpp          # Workload-to-architecture matching
│   ├── Distance.cpp          # Error tolerance analysis
│   ├── Astra_API.cpp         # Simulator input generation
│   ├── Simulated_Annealing.cpp # SA optimization
│   └── [other modules]
│
├── include/                  # C++ headers (18 files)
│   ├── Macro.h               # Constants and data structures
│   ├── Die.h, Wafer.h        # Architecture abstractions
│   ├── Attention.h           # Workload specifications
│   └── [other headers]
│
├── test/                     # Unit tests (14 files)
│   ├── *_test.cpp
│   └── CMakeLists.txt
│
├── scripts/                  # Executable programs
│   ├── main.cpp              # Main design space exploration
│   ├── simulated_annealing_main.cpp # SA optimization
│   └── run_simulated_annealing.sh
│
├── configuration/            # Configuration files
│   ├── auto_generate.txt     # Auto-generation parameters
│   ├── component_configuration.txt # Manual component specs
│   └── optimal_wafer.txt     # Output: Pareto-optimal wafers
│
├── workloads/                # Workload specifications
│   └── Llama_3B.txt          # 130-layer LLM workload
│
├── pyscript/                 # Python utilities
│   ├── gen_network.py        # Generate network.yml
│   ├── gen_system.py         # Generate system.json
│   ├── gen_logical_network.py # Generate logical_network.json
│   ├── gen_config_txt.py     # Generate config.txt
│   └── simulated_annealing_ns3.py
│
├── astra/                    # Astra-sim integration
│   ├── astra_analytical_batch.sh
│   ├── astra_ns3_batch.sh
│   ├── analyze_analytical_output.py
│   └── analyze_ns3_output.py
│
├── astra-sim-guide/          # Astra-sim documentation
│   ├── system_json.md
│   ├── network_yml.md
│   └── remote_memory_json.md
│
├── matlab/                   # Analysis scripts
│   ├── performance.m
│   ├── error_fit.m
│   └── [other analysis]
│
├── docs/                     # Documentation (10 files)
│   ├── COMPLETE_WORKFLOW_ANALYSIS.md
│   ├── SIMULATED_ANNEALING_GUIDE.md
│   ├── SA_INTEGRATION_GUIDE.md
│   ├── VALIDATION_GUIDE.md
│   └── [other docs]
│
└── output/                   # Simulation results
    └── 0060/
        ├── wafer/            # Optimal wafer configurations
        ├── workload/         # Astra-sim workload files
        ├── system/           # System configuration (JSON)
        ├── network/          # Network topology (YAML)
        ├── config/           # ns-3 configuration
        ├── logical_network/  # Logical topology
        └── physical_network/ # Physical network topology
```

## Workflow

### Phase 1: Design Space Exploration

```
Configuration (auto_generate.txt)
    ↓
Generate.cpp / Scan.cpp
    ↓ (Load component specs)
Permutation.cpp
    ↓ (Enumerate all valid die permutations)
    ├─ Check reticle limit (33mm × 28mm)
    ├─ Apply D2D bandwidth constraints
    ├─ Prune dominated solutions (Pareto optimization)
    └─ Filter by performance threshold
    ↓
Output: optimal_wafer.txt (Pareto-optimal wafers)
```

### Phase 2: Workload Modeling

```
Attention.cpp (Model transformer blocks)
    ├─ Prefill/decode phases
    ├─ Training/inference modes
    └─ Calculate: TFLOPs, paramsize, access, traffic
    ↓
Pipeline.cpp (Merge layers with 1F1B schedule)
    ↓
Recompute.cpp (Activation checkpointing)
    ↓
Output: Workload metrics
```

### Phase 3: Astra-sim Integration

```
Astra_API.cpp (For each wafer)
    ├─ Generate workload.txt (DATA format)
    └─ Generate physical_network.txt (ns-3 format)
    ↓
Python scripts (pyscript/)
    ├─ gen_network.py → network.yml (Ring topology)
    ├─ gen_system.py → system.json (Scheduling policy)
    ├─ gen_logical_network.py → logical_network.json
    └─ gen_config_txt.py → config.txt (ns-3 config)
    ↓
Output: Complete astra-sim configuration
```

### Phase 4: Simulation & Analysis

```
ns-3 Backend (Cycle-accurate simulation)
    ↓
analyze_ns3_output.py (Parse results)
    ↓
Output: Optimal wafer with cycle count
```

## Three Search Strategies

| Strategy | Approach | Speed | Accuracy | Use Case |
|----------|----------|-------|----------|----------|
| **Exhaustive** | Enumerate all feasible wafers | Slow | Guaranteed Pareto-optimal | Small design spaces |
| **Exhaustive + Analytical → ns-3** | Filter with analytical backend, refine with ns-3 | Medium | Good | Medium design spaces |
| **Simulated Annealing + ns-3** | Stochastic local search with ns-3 | Fast | Approximate | Large design spaces |

### Simulated Annealing Features

- **7 mutation types**: Change compute/memory/communication units, regenerate edge permutations
- **Metropolis criterion**: Accept improvements always, worse solutions probabilistically
- **Geometric cooling**: Temperature decreases by `cooling_rate` each iteration
- **Direct ns-3 optimization**: Optimizes actual cycle counts, not estimates
- **Automatic configuration generation**: All astra-sim files generated on-the-fly

## Quick Start

### Build

```bash
mkdir build && cd build
cmake ..
make
```

### Run Design Space Exploration

```bash
# Exhaustive search
./build/error_tolerance

# Simulated annealing with ns-3
./scripts/run_simulated_annealing.sh
```

### Generate Astra-sim Configuration Files

```bash
cd pyscript
python gen_network.py
python gen_system.py
python gen_logical_network.py
python gen_config_txt.py
```

### Run Astra-sim Simulation

```bash
# Analytical backend
./astra/astra_analytical_batch.sh

# ns-3 backend
./astra/astra_ns3_batch.sh
```

## Configuration

### auto_generate.txt

Specifies component specifications and wafer parameters:

```
wafer length: 200mm
wafer width: 200mm
compute chip: 12-15mm length, 10-13mm width, 0.05 TFLOPS/mm²
memory unit: 5-8mm length, 5-7mm width, 0.16 GB/mm²
communication unit: 1-2mm length, 2-3mm width, 10 GB/s per mm
min TFLOPS: 200
min SRAM: 100 GB
min DRAM: 100 GB
min memory bandwidth: 100 GB/s
min communication bandwidth: 200 GB/s
```

### Workload Specification (Llama_3B.txt)

Defines transformer workload in astra-sim format:

```
HYBRID_TRANSFORMER
130                          # Number of layers
layer_0 -1 TFLOPs ALLREDUCE comm_size ...
layer_1 -1 TFLOPs ALLREDUCE comm_size ...
...
```

## Key Algorithms

### Pareto Pruning

Keeps only non-dominated solutions:
- Solution A dominates B if A is better in all metrics
- Reduces 1000s of configurations to ~17 Pareto-optimal wafers

### Error Tolerance Analysis

Identifies robust architectures:
1. Find optimal wafer with simulator
2. Calculate error bounds (±10% TFLOPS, ±5% bandwidth, etc.)
3. Identify alternative wafers within error bounds
4. Robust solution = few alternatives (less sensitive to errors)

### D2D Bandwidth Scaling

Models physical interconnect limitations:
- Small die (20mm × 20mm): 20 TB/s
- Large die (33mm × 28mm): 10 TB/s
- Linear interpolation based on perimeter

## Output Format

### optimal_wafer.txt

Each line represents one Pareto-optimal wafer:

```
TFLOPS SRAM DRAM mem_bw comm_bw rows cols wafer_len wafer_wid
compute_len compute_wid compute_tflops compute_sram compute_pad
mem_len mem_wid mem_cap mem_bw mem_pad
comm_len comm_wid comm_bw comm_pad
up down left right
die_pad band_per_area mem_bw_ratio
final_mem_bw final_comm_bw
```

Example:
```
1152 576 921.6 1440 14700 5 6 200 200 32 24 38.4 19.2 0.1 8 8 10.24 16 0.1 1 2 10 0.1 9 00011111119 1111111111111111111119 1111111111111111111119 0.5 1 0.6 48 490
```

## Documentation

- **[Complete Workflow Analysis](docs/COMPLETE_WORKFLOW_ANALYSIS.md)** - Full system architecture and data flow
- **[Simulated Annealing Guide](docs/SIMULATED_ANNEALING_GUIDE.md)** - SA algorithm, tuning, and advanced features
- **[SA Integration Guide](docs/SA_INTEGRATION_GUIDE.md)** - How SA fits into the complete workflow
- **[Validation Guide](docs/VALIDATION_GUIDE.md)** - Testing and validation procedures
- **[Performance Model](docs/performance_model.md)** - Analytical performance modeling details
- **[Workload to Astra-sim](docs/workload_to_astra_sim.md)** - Workload file format specification

## Dependencies

- **C++ Compiler**: C++11 or later
- **CMake**: 3.10 or later
- **Python**: 3.6+ (for utility scripts)
- **Astra-sim**: For cycle-accurate simulation (optional)
- **ns-3**: Network simulator backend (optional)

## Key Features

✓ Exhaustive design space exploration with Pareto pruning  
✓ Analytical LLM workload modeling (attention, pipeline, recompute)  
✓ Error tolerance analysis for robust architecture selection  
✓ Automatic astra-sim configuration file generation  
✓ Simulated annealing for large design spaces  
✓ Cycle-accurate ns-3 simulation integration  
✓ Comprehensive unit tests (14 test files)  
✓ Python utilities for batch processing  
✓ MATLAB analysis scripts  

## Example: Llama 3B Optimization

The project includes a complete example optimizing for a 130-layer Llama 3B model:

1. **Exploration**: Generates 17 Pareto-optimal wafer configurations
2. **Modeling**: Analyzes Llama 3B workload (prefill/decode, training/inference)
3. **Simulation**: Runs ns-3 on each configuration
4. **Analysis**: Identifies optimal and robust architectures

## Contributing

1. Follow existing code structure and naming conventions
2. Add unit tests for new features
3. Update documentation
4. Run all tests before submitting

## License

[Specify your license]

## Citation

If you use this tool in your research, please cite:

```
[Add citation information]
```

## Contact

[Specify contact information]

---

**Last Updated**: March 2026  
**Project Status**: Active Development  
**Latest Features**: Simulated Annealing Integration, Error Tolerance Analysis
