# Error Tolerance Project - Complete Workflow Analysis

## Overview

The error_tolerance project is a **wafer-scale architecture design space exploration and optimization tool** that:
1. Explores feasible die/wafer configurations
2. Generates multiple candidate wafers
3. Creates simulation input files for astra-sim
4. Runs ns-3 cycle-accurate simulations
5. Identifies optimal and robust architectures

---

## Complete Workflow Pipeline

```
┌─────────────────────────────────────────────────────────────────┐
│ PHASE 1: DESIGN SPACE EXPLORATION (C++)                         │
└─────────────────────────────────────────────────────────────────┘
                              ↓
    Configuration Files (auto_generate.txt or component_configuration.txt)
                              ↓
    ┌─────────────────────────────────────────────────────────────┐
    │ Generate.cpp / Scan.cpp                                     │
    │ - Load component specs (Compute, Memory, Communication)     │
    │ - Parse wafer parameters and thresholds                     │
    └─────────────────────────────────────────────────────────────┘
                              ↓
    ┌─────────────────────────────────────────────────────────────┐
    │ Permutation.cpp                                             │
    │ - Enumerate all valid die permutations                      │
    │ - Apply reticle limit constraints (33mm × 28mm)             │
    │ - Apply D2D bandwidth constraints                           │
    │ - Prune dominated solutions (Pareto optimization)           │
    │ - Filter by performance threshold                           │
    └─────────────────────────────────────────────────────────────┘
                              ↓
    Output: List of Pareto-optimal Wafers
    Saved to: optimal_wafer.txt (17 wafers in example)

┌─────────────────────────────────────────────────────────────────┐
│ PHASE 2: WORKLOAD ANALYSIS (C++)                                │
└─────────────────────────────────────────────────────────────────┘
                              ↓
    Workload Specifications (Attention.cpp, Pipeline.cpp, etc.)
                              ↓
    ┌─────────────────────────────────────────────────────────────┐
    │ Attention.cpp                                               │
    │ - Model LLM transformer blocks                              │
    │ - Calculate TFLOPs, paramsize, access, traffic              │
    │ - Support prefill/decode phases                             │
    │ - Support training/inference modes                          │
    └─────────────────────────────────────────────────────────────┘
                              ↓
    Output: Workload metrics (compute, memory, communication)

┌─────────────────────────────────────────────────────────────────┐
│ PHASE 3: ASTRA-SIM INPUT GENERATION (C++ + Python)              │
└─────────────────────────────────────────────────────────────────┘
                              ↓
    ┌─────────────────────────────────────────────────────────────┐
    │ Astra_API.cpp                                               │
    │ - For each wafer, generate:                                 │
    │   * Workload file (DATA format)                             │
    │   * Physical network file (ns-3 format)                     │
    └─────────────────────────────────────────────────────────────┘
                              ↓
    Output Files (for each wafer):
    - 0060_000000_workload.txt (Astra-sim workload format)
    - 0060_000000_physical_network.txt (ns-3 network topology)
                              ↓
    ┌─────────────────────────────────────────────────────────────┐
    │ Python Scripts (pyscript/)                                  │
    │ - gen_network.py: Generate network.yml files                │
    │ - gen_system.py: Generate system.json files                 │
    │ - gen_logical_network.py: Generate logical_network.json     │
    │ - gen_config_txt.py: Generate config.txt files              │
    └─────────────────────────────────────────────────────────────┘
                              ↓
    Output Files (for each wafer):
    - 0060_000000_network.yml (Ring topology, bandwidth, latency)
    - 0060_000000_system.json (Scheduling policy, collective algorithms)
    - 0060_000000_logical_network.json (Logical dimensions)
    - 0060_000000_config.txt (ns-3 configuration)

┌─────────────────────────────────────────────────────────────────┐
│ PHASE 4: NS-3 SIMULATION                                        │
└─────────────────────────────────────────────────────────────────┘
                              ↓
    For each wafer configuration:
    ┌─────────────────────────────────────────────────────────────┐
    │ ns3.42-AstraSimNetwork-default                              │
    │ - Input: workload, system.json, network.yml, config.txt     │
    │ - Simulate: Cycle-accurate network simulation               │
    │ - Output: ns3_output.txt (cycle counts)                     │
    └─────────────────────────────────────────────────────────────┘
                              ↓
    Output: Cycle counts for each wafer

┌─────────────────────────────────────────────────────────────────┐
│ PHASE 5: RESULT ANALYSIS (Python)                               │
└─────────────────────────────────────────────────────────────────┘
                              ↓
    ┌─────────────────────────────────────────────────────────────┐
    │ analyze_ns3_output.py                                       │
    │ - Parse ns3_output.txt files                                │
    │ - Extract max cycle counts                                  │
    │ - Identify optimal wafer                                    │
    └─────────────────────────────────────────────────────────────┘
                              ↓
    Output: Optimal wafer configuration with cycle count
```

---

## Data Flow Example (0060 Configuration)

### Input: optimal_wafer.txt (17 Pareto-optimal wafers)

Each line represents one wafer configuration:
```
TFLOPS SRAM DRAM mem_bw comm_bw rows cols wafer_len wafer_wid 
compute_len compute_wid compute_tflops compute_sram compute_pad 
mem_len mem_wid mem_cap mem_bw mem_pad 
comm_len comm_wid comm_bw comm_pad 
up down left right 
die_pad band_per_area mem_bw_ratio 
final_mem_bw final_comm_bw
```

Example line (wafer 4):
```
1152 576 921.6 1440 14700 5 6 200 200 32 24 38.4 19.2 0.1 8 8 10.24 16 0.1 1 2 10 0.1 
9 00011111119 1111111111111111111119 1111111111111111111119 
0.5 1 0.6 48 490
```

Interpretation:
- **TFLOPS**: 1152 (total compute)
- **SRAM**: 576 GB (total on-chip memory)
- **DRAM**: 921.6 GB (total HBM)
- **mem_bw**: 1440 GB/s (memory bandwidth)
- **comm_bw**: 14700 GB/s (communication bandwidth)
- **rows**: 5, **cols**: 6 (5×6 die grid)
- **Wafer size**: 200×200 mm
- **Compute core**: 32×24 mm, 38.4 TFLOPS, 19.2 GB SRAM
- **Memory unit**: 8×8 mm, 10.24 GB capacity, 16 GB/s bandwidth
- **Communication unit**: 1×2 mm, 10 GB/s bandwidth
- **Permutations**: up="9" (empty), down="00011111119" (2 memory units), left/right similar

### Generated Files (for each wafer)

**0060_000000_workload.txt** (Astra-sim format):
```
DATA
5
layer_0 -1 1116072 ALLREDUCE 0 1116072 NONE 0 1116072 ALLREDUCE 5 10
layer_1 -1 1116072 ALLREDUCE 0 1116072 NONE 0 1116072 ALLREDUCE 5 10
...
```

**0060_000000_system.json** (Scheduling configuration):
```json
{
  "scheduling-policy": "LIFO",
  "intra-dimension-scheduling": "FIFO",
  "inter-dimension-scheduling": "baseline",
  "endpoint-delay": 10,
  "active-chunks-per-dimension": 1,
  "all-reduce-implementation": ["ring", "ring"],
  "local_mem_bw": 112.0,
  "boost_mode": 0
}
```

**0060_000000_network.yml** (Network topology):
```yaml
topology: [Ring, Ring]
npus_count: [6, 5]
bandwidth: [125.0, 125.0]
latency: [10.0, 10.0]
```

**0060_000000_config.txt** (ns-3 configuration):
- Network backend settings
- Congestion control parameters
- Buffer sizes
- Topology file path

---

## New: Simulated Annealing Integration

The simulated annealing module provides an **alternative search strategy**:

```
Random Initial Wafer
        ↓
    ns-3 Evaluation
        ↓
    Simulated Annealing Loop:
    - Generate neighbor (mutate configuration)
    - Evaluate with ns-3
    - Accept/reject based on Metropolis criterion
    - Cool temperature
        ↓
    Optimal Wafer Found
```

**Advantages:**
- Direct optimization of ns-3 cycle counts
- Handles large design spaces efficiently
- Generates all required astra-sim configuration files automatically
- Tracks search trajectory for analysis

---

## Key Insights

### 1. Permutation Encoding
- Strings like "00011111119" represent unit placement on die edges
- '0' = Memory unit, '1' = Communication unit
- '9' = Delimiter (being replaced with space + "." for empty)
- Sorted to maintain canonical form

### 2. Wafer Metrics Calculation
- **TFLOPS**: die_TFLOPS × (rows × columns)
- **Bandwidth**: die_bandwidth × (rows × columns)
- **Capacity**: die_capacity × (rows × columns)

### 3. Constraint Enforcement
- **Reticle limit**: Die size ≤ 33mm × 28mm
- **D2D bandwidth**: Scales from 20 TB/s (small) to 10 TB/s (large)
- **Threshold filtering**: Only wafers meeting minimum performance

### 4. Astra-sim Integration
- **Workload format**: DATA + layer specifications
- **Network topology**: 2D Ring (torus)
- **Scheduling**: LIFO + FIFO + baseline
- **Collective algorithms**: Ring-based for all-reduce/all-gather

---

## File Organization

```
error_tolerance/
├── src/
│   ├── Permutation.cpp (design space exploration)
│   ├── Astra_API.cpp (simulator input generation)
│   ├── Simulated_Annealing.cpp (SA optimization)
│   └── ... (other modules)
├── pyscript/
│   ├── gen_network.py (generate network.yml)
│   ├── gen_system.py (generate system.json)
│   ├── gen_logical_network.py (generate logical_network.json)
│   └── gen_config_txt.py (generate config.txt)
├── output/0060/
│   ├── wafer/ (optimal_wafer.txt)
│   ├── workload/ (workload files)
│   ├── system/ (system.json files)
│   ├── network/ (network.yml files)
│   ├── config/ (config.txt files)
│   ├── logical_network/ (logical_network.json files)
│   └── physical_network/ (physical_network.txt files)
└── configuration/
    ├── auto_generate.txt (auto-generation config)
    └── component_configuration.txt (manual config)
```

---

## Workflow Summary

1. **Explore**: Generate 17 Pareto-optimal wafers from design space
2. **Analyze**: Model workload (LLM transformer blocks)
3. **Generate**: Create astra-sim input files for each wafer
4. **Simulate**: Run ns-3 cycle-accurate simulation
5. **Optimize**: Identify optimal and robust architectures
6. **Alternative**: Use simulated annealing for direct ns-3 optimization

This integrated approach combines analytical exploration with cycle-accurate simulation to find optimal wafer-scale architectures for AI workloads.
