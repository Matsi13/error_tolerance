#include <iostream>
#include <cmath>
#include <random>
#include <algorithm>
#include <limits>

#include "Simulated_Annealing.h"
#include "Permutation.h"

using namespace std;

// Generate a random permutation string that fits along an edge
string generate_random_permutation(
    float compute_edge_size,
    const Memory& memory_unit,
    const Communication& communication_unit,
    float relaxation,
    mt19937& rng
) {
    float memory_size = memory_unit.get_size(0);
    float communication_size = communication_unit.get_size(0);
    float component_padding = max(memory_unit.get_padding(), communication_unit.get_padding());
    
    string permutation = "";
    float current_size = 0;
    
    uniform_real_distribution<float> unit_choice(0.0f, 1.0f);
    uniform_real_distribution<float> stop_prob(0.0f, 1.0f);
    
    while (true) {
        float remain_size = compute_edge_size - current_size + relaxation;
        
        if (remain_size < 0) break;
        
        // Randomly decide to stop adding units (higher probability as we fill up)
        float fill_ratio = current_size / (compute_edge_size + relaxation);
        if (stop_prob(rng) < fill_ratio * 0.7f) break;
        
        bool can_add_memory = (remain_size >= memory_size + component_padding);
        bool can_add_comm = (remain_size >= communication_size + component_padding);
        
        if (!can_add_memory && !can_add_comm) break;
        
        // Randomly choose between memory and communication
        if (can_add_memory && can_add_comm) {
            if (unit_choice(rng) < 0.5f) {
                permutation.push_back(MEMORY_UNIT);
                current_size += memory_size + component_padding;
            } else {
                permutation.push_back(COMMUNICATION_UNIT);
                current_size += communication_size + component_padding;
            }
        } else if (can_add_memory) {
            permutation.push_back(MEMORY_UNIT);
            current_size += memory_size + component_padding;
        } else {
            permutation.push_back(COMMUNICATION_UNIT);
            current_size += communication_size + component_padding;
        }
    }
    
    // Sort to maintain canonical form
    sort(permutation.begin(), permutation.end());
    
    return permutation;
}

// Generate a random initial wafer
Wafer generate_random_initial_wafer(
    const list<Compute>& compute_configs,
    const list<Memory>& memory_configs,
    const list<Communication>& communication_configs,
    const Config& wafer_config,
    mt19937& rng
) {
    uniform_int_distribution<int> compute_dist(0, compute_configs.size() - 1);
    uniform_int_distribution<int> memory_dist(0, memory_configs.size() - 1);
    uniform_int_distribution<int> comm_dist(0, communication_configs.size() - 1);
    
    // Randomly select units
    auto compute_it = compute_configs.begin();
    advance(compute_it, compute_dist(rng));
    Compute compute_unit = *compute_it;
    
    auto memory_it = memory_configs.begin();
    advance(memory_it, memory_dist(rng));
    Memory memory_unit = *memory_it;
    
    auto comm_it = communication_configs.begin();
    advance(comm_it, comm_dist(rng));
    Communication comm_unit = *comm_it;
    
    // Generate random permutations for each edge
    string up = generate_random_permutation(compute_unit.get_size(0), memory_unit, comm_unit, wafer_config.relaxation, rng);
    string down = generate_random_permutation(compute_unit.get_size(0), memory_unit, comm_unit, wafer_config.relaxation, rng);
    string left = generate_random_permutation(compute_unit.get_size(1), memory_unit, comm_unit, wafer_config.relaxation, rng);
    string right = generate_random_permutation(compute_unit.get_size(1), memory_unit, comm_unit, wafer_config.relaxation, rng);
    
    // Create die and wafer
    Die die(wafer_config.die_padding, wafer_config.bandwidth_per_area, wafer_config.memory_bandwidth_ratio,
            compute_unit, memory_unit, comm_unit, up, down, left, right);
    
    // Check reticle limit
    if (!die.check_reticle_limit()) {
        // If exceeds limit, try with empty permutations
        string empty = "";
        Die simple_die(wafer_config.die_padding, wafer_config.bandwidth_per_area, wafer_config.memory_bandwidth_ratio,
                      compute_unit, memory_unit, comm_unit, empty, empty, empty, empty);
        float wafer_sizes[2] = {wafer_config.wafer_length, wafer_config.wafer_width};
        return Wafer(wafer_sizes, simple_die);
    }
    
    float wafer_sizes[2] = {wafer_config.wafer_length, wafer_config.wafer_width};
    Wafer wafer(wafer_sizes, die);
    
    return wafer;
}

// Generate a neighbor by modifying the current wafer
Wafer generate_neighbor(
    const Wafer& current,
    const list<Compute>& compute_configs,
    const list<Memory>& memory_configs,
    const list<Communication>& communication_configs,
    const Config& wafer_config,
    mt19937& rng
) {
    Die current_die = current.get_die();
    Compute compute_unit = current_die.get_Compute_unit();
    Memory memory_unit = current_die.get_Memory_unit();
    Communication comm_unit = current_die.get_Communication_unit();
    
    string up = current_die.get_permutation_up();
    string down = current_die.get_permutation_down();
    string left = current_die.get_permutation_left();
    string right = current_die.get_permutation_right();
    
    uniform_int_distribution<int> mutation_type(0, 6);
    int mutation = mutation_type(rng);
    
    switch (mutation) {
        case 0: // Change compute unit
        {
            uniform_int_distribution<int> compute_dist(0, compute_configs.size() - 1);
            auto it = compute_configs.begin();
            advance(it, compute_dist(rng));
            compute_unit = *it;
            break;
        }
        case 1: // Change memory unit
        {
            uniform_int_distribution<int> memory_dist(0, memory_configs.size() - 1);
            auto it = memory_configs.begin();
            advance(it, memory_dist(rng));
            memory_unit = *it;
            break;
        }
        case 2: // Change communication unit
        {
            uniform_int_distribution<int> comm_dist(0, communication_configs.size() - 1);
            auto it = communication_configs.begin();
            advance(it, comm_dist(rng));
            comm_unit = *it;
            break;
        }
        case 3: // Regenerate up permutation
            up = generate_random_permutation(compute_unit.get_size(0), memory_unit, comm_unit, wafer_config.relaxation, rng);
            break;
        case 4: // Regenerate down permutation
            down = generate_random_permutation(compute_unit.get_size(0), memory_unit, comm_unit, wafer_config.relaxation, rng);
            break;
        case 5: // Regenerate left permutation
            left = generate_random_permutation(compute_unit.get_size(1), memory_unit, comm_unit, wafer_config.relaxation, rng);
            break;
        case 6: // Regenerate right permutation
            right = generate_random_permutation(compute_unit.get_size(1), memory_unit, comm_unit, wafer_config.relaxation, rng);
            break;
    }
    
    // Create new die
    Die new_die(wafer_config.die_padding, wafer_config.bandwidth_per_area, wafer_config.memory_bandwidth_ratio,
                compute_unit, memory_unit, comm_unit, up, down, left, right);
    
    // Check reticle limit - if exceeds, return current wafer unchanged
    if (!new_die.check_reticle_limit()) {
        return current;
    }
    
    // Check threshold - if doesn't meet, return current wafer unchanged
    float wafer_sizes[2] = {wafer_config.wafer_length, wafer_config.wafer_width};
    Wafer new_wafer(wafer_sizes, new_die);
    
    if (!is_over_threshold(new_wafer, wafer_config.threshold)) {
        return current;
    }
    
    return new_wafer;
}

// Main simulated annealing algorithm
SA_Result simulated_annealing_search(
    const list<Compute>& compute_configs,
    const list<Memory>& memory_configs,
    const list<Communication>& communication_configs,
    const Config& wafer_config,
    const SA_Config& sa_config,
    SimulatorCallback simulator,
    bool track_trajectory
) {
    SA_Result result;
    result.iterations_performed = 0;
    result.evaluations_performed = 0;
    result.optimal_cycles = numeric_limits<float>::infinity();
    
    // Initialize random number generator
    mt19937 rng(sa_config.random_seed);
    uniform_real_distribution<float> acceptance_prob(0.0f, 1.0f);
    
    // Generate initial solution
    cout << "Generating initial wafer configuration..." << endl;
    Wafer current_wafer = generate_random_initial_wafer(compute_configs, memory_configs, communication_configs, wafer_config, rng);
    
    // Evaluate initial solution
    cout << "Evaluating initial configuration with ns-3..." << endl;
    float current_cycles = simulator(current_wafer);
    result.evaluations_performed++;
    
    // Initialize best solution
    Wafer best_wafer = current_wafer;
    float best_cycles = current_cycles;
    result.optimal_wafer = best_wafer;
    result.optimal_cycles = best_cycles;
    
    if (track_trajectory) {
        result.visited_wafers.push_back(current_wafer);
    }
    
    cout << "Initial cycles: " << current_cycles << endl;
    cout << "Starting simulated annealing..." << endl;
    
    // Simulated annealing loop
    float temperature = sa_config.initial_temperature;
    int iteration = 0;
    
    while (temperature > sa_config.min_temperature && iteration < sa_config.max_iterations) {
        
        for (int i = 0; i < sa_config.iterations_per_temp; i++) {
            iteration++;
            
            if (iteration % 10 == 0) {
                cout << "Iteration " << iteration 
                     << ", Temperature: " << temperature 
                     << ", Best cycles: " << best_cycles 
                     << ", Current cycles: " << current_cycles << endl;
            }
            
            // Generate neighbor
            Wafer neighbor_wafer = generate_neighbor(current_wafer, compute_configs, memory_configs, communication_configs, wafer_config, rng);
            
            // Evaluate neighbor
            float neighbor_cycles = simulator(neighbor_wafer);
            result.evaluations_performed++;
            
            // Calculate acceptance probability
            float delta = neighbor_cycles - current_cycles;
            bool accept = false;
            
            if (delta < 0) {
                // Better solution - always accept
                accept = true;
            } else {
                // Worse solution - accept with probability exp(-delta/T)
                float prob = exp(-delta / temperature);
                accept = (acceptance_prob(rng) < prob);
            }
            
            if (accept) {
                current_wafer = neighbor_wafer;
                current_cycles = neighbor_cycles;
                
                if (track_trajectory) {
                    result.visited_wafers.push_back(current_wafer);
                }
                
                // Update best if necessary
                if (current_cycles < best_cycles) {
                    best_wafer = current_wafer;
                    best_cycles = current_cycles;
                    result.optimal_wafer = best_wafer;
                    result.optimal_cycles = best_cycles;
                    
                    cout << "  New best found! Cycles: " << best_cycles << endl;
                }
            }
            
            if (iteration >= sa_config.max_iterations) break;
        }
        
        // Cool down
        temperature *= sa_config.cooling_rate;
    }
    
    result.iterations_performed = iteration;
    
    cout << "\nSimulated annealing completed!" << endl;
    cout << "Total iterations: " << result.iterations_performed << endl;
    cout << "Total evaluations: " << result.evaluations_performed << endl;
    cout << "Optimal cycles: " << result.optimal_cycles << endl;
    
    return result;
}
