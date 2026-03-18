# Simulated Annealing for Wafer Optimization

## Overview

This module implements **simulated annealing** as an alternative search strategy for finding optimal wafer-scale architectures. Unlike the exhaustive Permutation search, simulated annealing uses a stochastic local search guided by ns-3 cycle-accurate simulation.

## Motivation

The existing workflow has two approaches:
1. **Exhaustive search** → Analytical backend → ns-3 refinement
2. **New: Simulated annealing** → Direct ns-3 optimization

Simulated annealing is useful when:
- The design space is too large for exhaustive enumeration
- You want to directly optimize for ns-3 cycle counts (not analytical estimates)
- You're willing to trade optimality guarantees for faster convergence

## Algorithm

### Simulated Annealing Basics

```
1. Start with random wafer configuration
2. Evaluate with ns-3 simulator
3. Loop until temperature cools:
   a. Generate neighbor configuration (mutate current)
   b. Evaluate neighbor with ns-3
   c. If better: accept
   d. If worse: accept with probability exp(-ΔE/T)
   e. Cool temperature: T = T × cooling_rate
4. Return best configuration found
```

### Neighbor Generation

A neighbor is created by randomly mutating one aspect of the current wafer:
- **Change compute unit** (select different size/TFLOPS)
- **Change memory unit** (select different capacity/bandwidth)
- **Change communication unit** (select different bandwidth)
- **Regenerate edge permutation** (up/down/left/right)

All neighbors must:
- Pass reticle limit check (≤ 33mm × 28mm)
- Meet performance threshold
- Be valid configurations

### Acceptance Criterion

```cpp
if (neighbor_cycles < current_cycles) {
    accept = true;  // Always accept improvements
} else {
    float delta = neighbor_cycles - current_cycles;
    float prob = exp(-delta / temperature);
    accept = (random() < prob);  // Probabilistically accept worse solutions
}
```

This allows escaping local minima early (high T) while converging later (low T).

## Implementation

### C++ Core (`Simulated_Annealing.h/cpp`)

**Key Functions:**

- `generate_random_initial_wafer()`: Creates valid starting configuration
- `generate_neighbor()`: Mutates current wafer (7 mutation types)
- `simulated_annealing_search()`: Main SA loop with ns-3 callback
- `generate_random_permutation()`: Creates random edge permutation

**Configuration (`SA_Config`):**
```cpp
struct SA_Config {
    float initial_temperature;    // e.g., 1000.0
    float cooling_rate;           // e.g., 0.95 (geometric cooling)
    float min_temperature;        // e.g., 1.0 (stopping criterion)
    int iterations_per_temp;      // e.g., 5 (neighbors per temperature)
    int max_iterations;           // e.g., 200 (hard limit)
    unsigned int random_seed;     // For reproducibility
};
```

**Result (`SA_Result`):**
```cpp
struct SA_Result {
    Wafer optimal_wafer;          // Best configuration found
    float optimal_cycles;         // ns-3 cycle count
    int iterations_performed;     // Total iterations
    int evaluations_performed;    // Total ns-3 calls
    list<Wafer> visited_wafers;   // Search trajectory (optional)
};
```

### Main Program (`simulated_annealing_main.cpp`)

**Usage:**
```bash
./simulated_annealing <config_file> [ns3_command_template] [temp_dir]
```

**Example:**
```bash
./simulated_annealing ./configuration/auto_generate.txt \
    "./astra-sim --workload={WORKLOAD} --network={NETWORK} > {OUTPUT}" \
    ./temp_sa
```

**Workflow:**
1. Load component configurations (Compute/Memory/Communication units)
2. Set SA parameters (temperature, cooling, iterations)
3. Create ns-3 simulator callback wrapper
4. Run simulated annealing
5. Save optimal wafer and search trajectory

### ns-3 Simulator Wrapper

The `ns3_simulator_wrapper()` function:
1. Generates unique workload and network files for the wafer
2. Executes ns-3 simulation via system call
3. Parses output to extract max cycle count
4. Returns cycles as fitness value

**File Generation:**
- `sa_eval_XXXXXX_workload.txt`: Workload specification
- `sa_eval_XXXXXX_physical_network.txt`: Network topology
- `sa_eval_XXXXXX_ns3_output.txt`: Simulation output

## Building

### CMake Integration

The simulated annealing module is integrated into the build system:

```cmake
# In src/CMakeLists.txt
add_library(simulated_annealing_lib
    Simulated_Annealing.cpp
    Memory.cpp
    Compute.cpp
    Communication.cpp
    Die.cpp
    Wafer.cpp
    Permutation.cpp
)

# In CMakeLists.txt
add_executable(simulated_annealing simulated_annealing_main.cpp)
target_link_libraries(simulated_annealing 
    simulated_annealing_lib 
    generate_lib 
    astra_API_lib
)
```

### Build Commands

```bash
mkdir build && cd build
cmake ..
make simulated_annealing
```

## Usage

### 1. Prepare Configuration

Create or use existing configuration file (e.g., `auto_generate.txt`):
```
wafer length : 200
wafer width : 200
die padding : 0.5
...
available compute chip types : 1
TFLOPS per area : 0.05
compute chip length : 12 1 15
...
```

### 2. Run Simulated Annealing

**Option A: Using shell script**
```bash
chmod +x run_simulated_annealing.sh
./run_simulated_annealing.sh
```

**Option B: Direct execution**
```bash
./simulated_annealing ./configuration/auto_generate.txt
```

### 3. Adjust Parameters

Edit `simulated_annealing_main.cpp` to tune SA parameters:
```cpp
SA_Config sa_config;
sa_config.initial_temperature = 1000.0f;  // Higher = more exploration
sa_config.cooling_rate = 0.95f;           // Slower cooling = more thorough
sa_config.min_temperature = 1.0f;
sa_config.iterations_per_temp = 5;        // More = slower but better
sa_config.max_iterations = 200;           // Increase for larger spaces
sa_config.random_seed = 42;               // Change for different runs
```

### 4. Analyze Results

**Output files in `temp_dir`:**
- `sa_optimal_wafer.txt`: Best wafer configuration (same format as `optimal_wafer.txt`)
- `sa_trajectory.txt`: All visited configurations during search
- `sa_eval_*.txt`: Individual ns-3 simulation outputs

**Parse optimal wafer:**
```bash
# First line contains wafer metrics
head -n 1 temp_sa/sa_optimal_wafer.txt
# Format: TFLOPS SRAM DRAM mem_bw comm_bw rows cols ...
```

## Comparison with Exhaustive Search

| Aspect | Exhaustive Search | Simulated Annealing |
|--------|------------------|---------------------|
| **Completeness** | Explores all feasible configs | Explores subset stochastically |
| **Optimality** | Guaranteed Pareto-optimal | Approximate (local optimum) |
| **Speed** | Slow (1000s of configs) | Fast (100s of evaluations) |
| **Simulator** | Analytical → ns-3 | Direct ns-3 |
| **Use case** | Small design spaces | Large design spaces |

## Tuning Guide

### Temperature Schedule

**Initial Temperature:**
- Too high: Accepts almost everything (random walk)
- Too low: Gets stuck in local minimum
- Rule of thumb: Set so ~50% of worse neighbors are accepted initially

**Cooling Rate:**
- Fast cooling (0.9): Quick convergence, may miss optimum
- Slow cooling (0.99): Thorough search, very slow
- Recommended: 0.95 for balance

### Iterations

**Iterations per temperature:**
- More iterations = better exploration at each temperature
- Typical: 5-10 for small neighborhoods, 20-50 for large

**Max iterations:**
- Safety limit to prevent infinite loops
- Set based on available time budget

### Neighbor Generation

Current implementation has 7 mutation types (equal probability). You can:
- **Weight mutations**: Prefer edge permutation changes over unit swaps
- **Adaptive neighborhoods**: Larger mutations at high T, smaller at low T
- **Constraint-aware**: Only generate valid neighbors (current approach)

## Advanced Features

### Multi-Start

Run SA multiple times with different seeds and pick the best:
```bash
for seed in {1..10}; do
    ./simulated_annealing config.txt ns3_cmd temp_sa_$seed
done
# Compare results and pick best
```

### Parallel Evaluation

Modify the wrapper to batch ns-3 simulations:
- Generate N neighbors
- Run N ns-3 simulations in parallel
- Accept best neighbor

### Hybrid Approach

1. Run exhaustive search with analytical backend → get top 100 wafers
2. Use SA to refine each with ns-3 → find local optimum
3. Compare all local optima → global best

## Troubleshooting

**Problem: ns-3 simulations fail**
- Check ns-3 executable path in `run_simulated_annealing.sh`
- Verify workload/network file formats
- Check ns-3 output files for error messages

**Problem: SA converges too quickly**
- Increase initial temperature
- Decrease cooling rate
- Increase iterations per temperature

**Problem: SA doesn't improve**
- Check if initial solution is already good
- Verify neighbor generation creates diverse solutions
- Increase max iterations

**Problem: All neighbors rejected**
- Threshold too strict → relax performance requirements
- Reticle limit too tight → check die size constraints
- Neighbor generation too aggressive → reduce mutation magnitude

## Future Enhancements

1. **Adaptive cooling**: Adjust cooling rate based on acceptance ratio
2. **Tabu search**: Remember recent configurations to avoid cycles
3. **Population-based**: Maintain multiple solutions (genetic algorithm)
4. **Surrogate models**: Use analytical backend to pre-filter neighbors
5. **Multi-objective**: Optimize for multiple metrics (cycles, power, area)

## References

- Kirkpatrick, S., et al. (1983). "Optimization by Simulated Annealing"
- Aarts, E., & Korst, J. (1989). "Simulated Annealing and Boltzmann Machines"
