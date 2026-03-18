# Simulated Annealing Integration Guide

## How Simulated Annealing Fits Into the Complete Workflow

The error_tolerance project now has **three search strategies**:

### Strategy 1: Exhaustive Search (Original)
```
Permutation.cpp → Generate all feasible wafers → Prune Pareto-optimal → optimal_wafer.txt
```
- **Pros**: Guaranteed Pareto-optimal solutions
- **Cons**: Slow for large design spaces (1000s of evaluations)
- **Use case**: Small to medium design spaces

### Strategy 2: Exhaustive + Analytical → ns-3 (Original)
```
Permutation.cpp → Analytical backend → Top N candidates → ns-3 refinement → Optimal wafer
```
- **Pros**: Fast filtering with analytical backend, accurate with ns-3
- **Cons**: Two-stage process, may miss good solutions in analytical filtering
- **Use case**: Medium design spaces with time constraints

### Strategy 3: Simulated Annealing + ns-3 (NEW)
```
Random initial wafer → SA loop with ns-3 evaluation → Optimal wafer
```
- **Pros**: Direct ns-3 optimization, efficient for large spaces
- **Cons**: Approximate (local optimum), stochastic
- **Use case**: Large design spaces, direct cycle-accurate optimization

---

## Simulated Annealing Workflow in Detail

### Step 1: Load Configuration
```cpp
Generate(compute_configs, memory_configs, communication_configs, config_path, wafer_config);
```
- Reads `auto_generate.txt` or `component_configuration.txt`
- Loads all available component specifications
- Sets wafer parameters and thresholds

### Step 2: Generate Initial Wafer
```cpp
Wafer initial = generate_random_initial_wafer(...);
```
- Randomly selects compute, memory, communication units
- Generates random permutations for each die edge
- Ensures reticle limit and threshold compliance

### Step 3: Evaluate with ns-3
```cpp
float cycles = ns3_simulator_wrapper(wafer, ns3_command_template, temp_dir, call_count);
```

This function:
1. **Generates workload file** (Astra-sim format)
   - Divides model across dies (pipeline parallelism)
   - Specifies forward/backward compute and communication

2. **Generates network files**:
   - `*_config.txt`: ns-3 network configuration
   - `*_network.yml`: Ring topology with bandwidth/latency
   - `*_system.json`: Scheduling policy and collective algorithms
   - `*_remote_memory.json`: Remote memory configuration
   - `*_logical_network.json`: Logical topology dimensions

3. **Executes ns-3 simulation**:
   ```bash
   ./ns3.42-AstraSimNetwork-default \
     --workload-configuration={WORKLOAD} \
     --system-configuration={SYSTEM} \
     --network-configuration={NETWORK} \
     --remote-memory-configuration={REMOTE_MEMORY} \
     --logical-topology-configuration={LOGICAL_NETWORK} \
     --comm-group-configuration=empty
   ```

4. **Parses output** to extract max cycle count

### Step 4: Simulated Annealing Loop
```cpp
while (temperature > min_temperature && iteration < max_iterations) {
    for (int i = 0; i < iterations_per_temp; i++) {
        // Generate neighbor by mutating current wafer
        Wafer neighbor = generate_neighbor(current, ...);
        
        // Evaluate neighbor with ns-3
        float neighbor_cycles = ns3_simulator_wrapper(neighbor, ...);
        
        // Metropolis acceptance criterion
        float delta = neighbor_cycles - current_cycles;
        if (delta < 0 || random() < exp(-delta / temperature)) {
            current = neighbor;
            current_cycles = neighbor_cycles;
            
            // Update best if necessary
            if (current_cycles < best_cycles) {
                best = current;
                best_cycles = current_cycles;
            }
        }
    }
    temperature *= cooling_rate;  // Cool down
}
```

### Step 5: Output Results
- **sa_optimal_wafer.txt**: Best wafer found
- **sa_trajectory.txt**: All visited configurations
- **sa_eval_*.txt**: Individual ns-3 outputs
- **sa_eval_*_*.json**: Configuration files for each evaluation

---

## Mutation Strategies (7 Types)

When generating a neighbor, randomly choose one of:

1. **Change compute unit**: Select different size/TFLOPS
2. **Change memory unit**: Select different capacity/bandwidth
3. **Change communication unit**: Select different bandwidth
4. **Regenerate up permutation**: New random edge configuration
5. **Regenerate down permutation**: New random edge configuration
6. **Regenerate left permutation**: New random edge configuration
7. **Regenerate right permutation**: New random edge configuration

All neighbors must:
- Pass reticle limit check (≤ 33mm × 28mm)
- Meet performance threshold
- Be valid configurations

---

## Configuration File Generation (Automatic)

### system.json
```json
{
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
  "local_mem_bw": <die_memory_bandwidth>,
  "boost_mode": 0
}
```

### network.yml
```yaml
topology: [Ring, Ring]
npus_count: [<columns>, <rows>]
bandwidth: [<die_bandwidth>, <die_bandwidth>]
latency: [10.0, 10.0]
```

### remote_memory.json
```json
{
  "memory-type": "NO_MEMORY_EXPANSION",
  "remote-mem-latency": 100,
  "remote-mem-bw": 10
}
```

### logical_network.json
```json
{
  "topology": ["Ring", "Ring"],
  "npus_count": [<columns>, <rows>],
  "bandwidth": [100.0, 100.0],
  "latency": [10.0, 10.0]
}
```

---

## Temperature Schedule

### Geometric Cooling
```
T(k) = T₀ × α^k
```
where:
- T₀ = initial_temperature (default: 1000.0)
- α = cooling_rate (default: 0.95)
- k = iteration number

### Acceptance Probability
```
P(accept worse) = exp(-ΔE / T)
```
where:
- ΔE = neighbor_cycles - current_cycles
- T = current temperature

**Behavior:**
- High T: Accept almost anything (exploration)
- Low T: Accept only improvements (exploitation)

---

## Tuning Parameters

### For Small Design Spaces (< 100 wafers)
```cpp
sa_config.initial_temperature = 500.0f;
sa_config.cooling_rate = 0.9f;
sa_config.iterations_per_temp = 3;
sa_config.max_iterations = 50;
```

### For Medium Design Spaces (100-1000 wafers)
```cpp
sa_config.initial_temperature = 1000.0f;
sa_config.cooling_rate = 0.95f;
sa_config.iterations_per_temp = 5;
sa_config.max_iterations = 200;
```

### For Large Design Spaces (> 1000 wafers)
```cpp
sa_config.initial_temperature = 2000.0f;
sa_config.cooling_rate = 0.98f;
sa_config.iterations_per_temp = 10;
sa_config.max_iterations = 500;
```

---

## Usage Example

### Build
```bash
mkdir build && cd build
cmake ..
make simulated_annealing
```

### Run
```bash
./simulated_annealing ./configuration/auto_generate.txt \
  "./astra-sim/extern/network_backend/ns-3/build/scratch/ns3.42-AstraSimNetwork-default \
    --workload-configuration={WORKLOAD} \
    --system-configuration={SYSTEM} \
    --network-configuration={NETWORK} \
    --remote-memory-configuration={REMOTE_MEMORY} \
    --logical-topology-configuration={LOGICAL_NETWORK} \
    --comm-group-configuration=empty" \
  ./temp_sa
```

### Output
```
=== Simulated Annealing Wafer Optimization with ns-3 ===
Configuration file: ./configuration/auto_generate.txt
Temporary directory: ./temp_sa

Loading component configurations...
  Loaded 3 compute configurations
  Loaded 1 memory configurations
  Loaded 1 communication configurations

Simulated Annealing Parameters:
  Initial temperature: 1000
  Cooling rate: 0.95

Starting simulated annealing search...

  [Eval 0] Executing ns-3 simulation...
  [Eval 0] Completed: 50000 cycles
  [Eval 1] Executing ns-3 simulation...
  [Eval 1] Completed: 48000 cycles
  [Eval 2] Executing ns-3 simulation...
  [Eval 2] Completed: 52000 cycles
  ...

=== Optimization Results ===
Optimal execution cycles: 42000
Total iterations: 150
Total ns-3 evaluations: 450

Optimal wafer saved to: ./temp_sa/sa_optimal_wafer.txt
Search trajectory saved to: ./temp_sa/sa_trajectory.txt

=== Optimization Complete ===
Configuration files generated in: ./temp_sa
```

---

## Comparison: All Three Strategies

| Aspect | Exhaustive | Exhaustive+Analytical | Simulated Annealing |
|--------|-----------|----------------------|-------------------|
| **Completeness** | All feasible | Top N candidates | Stochastic subset |
| **Optimality** | Pareto-optimal | Approximate | Local optimum |
| **Speed** | Slow (1000s evals) | Medium (100s evals) | Fast (100-500 evals) |
| **Simulator** | Analytical | Analytical → ns-3 | Direct ns-3 |
| **Accuracy** | Approximate | Good | Exact (ns-3) |
| **Use case** | Small spaces | Medium spaces | Large spaces |
| **Robustness** | High | Medium | Medium |

---

## Integration with Existing Workflow

### Option 1: Pure Simulated Annealing
```
SA + ns-3 → Optimal wafer
```

### Option 2: Hybrid Approach
```
Exhaustive + Analytical → Top 10 candidates
↓
SA refinement on each candidate with ns-3
↓
Compare all local optima → Global best
```

### Option 3: Multi-Start SA
```
Run SA 10 times with different seeds
↓
Compare all results
↓
Pick best or most robust
```

---

## Key Advantages of SA Integration

1. **Automatic Configuration Generation**: All astra-sim files generated on-the-fly
2. **Direct ns-3 Optimization**: Optimizes actual cycle counts, not estimates
3. **Flexible Search**: Easily adjustable temperature schedule
4. **Trajectory Tracking**: Understand search behavior
5. **Scalability**: Handles large design spaces efficiently
6. **Reproducibility**: Seed-based randomness for consistent results

---

## Next Steps

1. **Tune parameters** based on your design space size
2. **Run multiple seeds** to ensure robustness
3. **Compare with exhaustive search** on small spaces to validate
4. **Analyze trajectory** to understand search behavior
5. **Integrate with existing workflow** as needed

The simulated annealing module provides a powerful alternative for direct ns-3 optimization while maintaining compatibility with the existing error_tolerance framework.
