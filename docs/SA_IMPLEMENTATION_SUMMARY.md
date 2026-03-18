# Simulated Annealing Implementation Summary

## What Was Added

I've implemented a complete **simulated annealing** optimization module that uses ns-3 as the direct simulator for finding optimal wafer configurations. This provides an alternative to the exhaustive search approach.

## Files Created

### Core Implementation
1. **`include/Simulated_Annealing.h`** - Header with SA algorithm interface
2. **`src/Simulated_Annealing.cpp`** - Core SA implementation (312 lines)
3. **`simulated_annealing_main.cpp`** - Standalone executable (247 lines)

### Supporting Files
4. **`run_simulated_annealing.sh`** - Shell script to run SA optimization
5. **`pyscript/simulated_annealing_ns3.py`** - Python helper utilities
6. **`test/Simulated_Annealing_test.cpp`** - Unit tests (221 lines)
7. **`SIMULATED_ANNEALING_GUIDE.md`** - Complete documentation (320 lines)

### Build System Updates
8. Updated `src/CMakeLists.txt` - Added `simulated_annealing_lib`
9. Updated `CMakeLists.txt` - Added `simulated_annealing` executable
10. Updated `test/CMakeLists.txt` - Added SA tests
11. Updated `README.md` - Mentioned new SA feature

## Key Features

### Algorithm
- **Stochastic local search** guided by ns-3 cycle-accurate simulation
- **7 mutation types**: Change compute/memory/communication units, regenerate edge permutations
- **Metropolis acceptance**: Always accept improvements, probabilistically accept worse solutions
- **Geometric cooling**: Temperature decreases by `cooling_rate` each iteration
- **Constraint enforcement**: All neighbors pass reticle limit and threshold checks

### Configuration
```cpp
SA_Config {
    initial_temperature = 1000.0f;
    cooling_rate = 0.95f;
    min_temperature = 1.0f;
    iterations_per_temp = 5;
    max_iterations = 200;
    random_seed = 42;
}
```

### ns-3 Integration
- **Simulator callback**: Wraps ns-3 execution and parses cycle counts
- **File generation**: Creates workload and physical network files per evaluation
- **Output parsing**: Extracts max cycle count from ns-3 output

## Usage

### Build
```bash
mkdir build && cd build
cmake ..
make simulated_annealing
```

### Run
```bash
# Using shell script (recommended)
./run_simulated_annealing.sh

# Or directly
./simulated_annealing ./configuration/auto_generate.txt
```

### Output
- `temp_sa/sa_optimal_wafer.txt` - Best configuration found
- `temp_sa/sa_trajectory.txt` - Search trajectory
- `temp_sa/sa_eval_*.txt` - Individual ns-3 outputs

## Workflow Comparison

### Original Workflow
```
Exhaustive Search → Analytical Backend → Top N → ns-3 Backend → Optimal
```

### New SA Workflow
```
Random Initial → ns-3 Evaluation → SA Loop → ns-3 Optimal
```

## Advantages

1. **Direct optimization**: Optimizes actual ns-3 cycles, not analytical estimates
2. **Scalability**: Handles large design spaces (100s of evaluations vs 1000s)
3. **Flexibility**: Easy to add custom constraints or objectives
4. **Anytime algorithm**: Can stop early and still have a good solution

## Integration Points

The SA module integrates with existing components:
- **Generate.cpp**: Loads component configurations
- **Permutation.cpp**: Uses `is_over_threshold()` for validation
- **Die.cpp**: Uses `check_reticle_limit()` and D2D constraints
- **Astra_API.cpp**: File format compatible with existing pipeline

## Next Steps

To use this in your workflow:

1. **Configure ns-3 path** in `run_simulated_annealing.sh`
2. **Adjust SA parameters** in `simulated_annealing_main.cpp` based on your design space size
3. **Run optimization**: `./run_simulated_annealing.sh`
4. **Compare results**: Compare SA optimal with exhaustive search optimal

## Testing

Run unit tests:
```bash
cd build
ctest -R Simulated_Annealing_test -V
```

The test uses a mock simulator to verify:
- Random permutation generation
- Initial wafer generation
- Neighbor generation
- Full SA algorithm execution

## Documentation

See `SIMULATED_ANNEALING_GUIDE.md` for:
- Detailed algorithm explanation
- Parameter tuning guide
- Troubleshooting tips
- Advanced features (multi-start, parallel evaluation, hybrid approaches)

---

The implementation is complete and ready to use. You now have three search strategies:
1. **Exhaustive + Analytical** (existing)
2. **Exhaustive + Analytical → ns-3** (existing)
3. **Simulated Annealing + ns-3** (new)
