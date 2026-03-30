#include <cassert>
#include <iostream>
#include <list>

#include "Simulated_Annealing.h"
#include "Generate.h"

using namespace std;

// Mock simulator for testing (returns random cycles)
float mock_simulator(const Wafer& wafer) {
    // Simple fitness function based on wafer metrics
    float tflops = wafer.get_TFLOPS();
    float mem_bw = wafer.get_memory_bandwidth();
    float comm_bw = wafer.get_communication_bandwidth();
    
    // Lower is better (minimize cycles)
    // Favor high TFLOPS and high bandwidth
    float cycles = 100000.0f / (tflops + mem_bw + comm_bw + 1.0f);
    
    return cycles;
}

void test_random_permutation_generation() {
    cout << "Testing random permutation generation..." << endl;
    
    float memory_sizes[2] = {5.0f, 5.0f};
    Memory memory_unit(memory_sizes, 10.0f, 4.0f, 0.1f);
    
    float comm_sizes[2] = {1.0f, 2.0f};
    Communication comm_unit(comm_sizes, 10.0f, 0.1f);
    
    mt19937 rng(42);
    
    for (int i = 0; i < 5; i++) {
        string perm = generate_random_permutation(16.0f, memory_unit, comm_unit, 0.0f, rng);
        cout << "  Generated permutation: '" << perm << "'" << endl;
    }
    
    cout << "✓ Random permutation generation test passed" << endl << endl;
}

void test_initial_wafer_generation() {
    cout << "Testing initial wafer generation..." << endl;
    
    list<Compute> compute_configs;
    list<Memory> memory_configs;
    list<Communication> communication_configs;
    Config wafer_config;
    
    // Create simple test configurations
    float compute_sizes[2] = {16.0f, 12.0f};
    Compute compute(compute_sizes, 10.0f, 5.0f, 0.0f);
    compute_configs.push_back(compute);
    
    float memory_sizes[2] = {5.0f, 5.0f};
    Memory memory(memory_sizes, 10.0f, 4.0f, 0.1f);
    memory_configs.push_back(memory);
    
    float comm_sizes[2] = {1.0f, 2.0f};
    Communication comm(comm_sizes, 10.0f, 0.1f);
    communication_configs.push_back(comm);
    
    wafer_config.wafer_length = 120.0f;
    wafer_config.wafer_width = 120.0f;
    wafer_config.die_padding = 0.5f;
    wafer_config.bandwidth_per_area = 1.0f;
    wafer_config.memory_bandwidth_ratio = 0.6f;
    wafer_config.relaxation = 0.0f;
    wafer_config.threshold.TFLOPS = 0.0f;
    wafer_config.threshold.SRAM_capacity = 0.0f;
    wafer_config.threshold.on_die_mem_capacity = 0.0f;
    wafer_config.threshold.memory_bandwidth = 0.0f;
    wafer_config.threshold.communication_bandwidth = 0.0f;
    
    mt19937 rng(42);
    
    for (int i = 0; i < 3; i++) {
        Wafer wafer = generate_random_initial_wafer(compute_configs, memory_configs, 
                                                     communication_configs, wafer_config, rng);
        cout << "  Generated wafer " << i << ": ";
        cout << "TFLOPS=" << wafer.get_TFLOPS() 
             << ", rows=" << wafer.get_rows() 
             << ", cols=" << wafer.get_columns() << endl;
    }
    
    cout << "✓ Initial wafer generation test passed" << endl << endl;
}

void test_neighbor_generation() {
    cout << "Testing neighbor generation..." << endl;
    
    list<Compute> compute_configs;
    list<Memory> memory_configs;
    list<Communication> communication_configs;
    Config wafer_config;
    
    // Create multiple configurations for neighbor generation
    for (int i = 0; i < 3; i++) {
        float compute_sizes[2] = {12.0f + i * 2.0f, 10.0f + i * 2.0f};
        Compute compute(compute_sizes, 8.0f + i * 2.0f, 4.0f + i, 0.0f);
        compute_configs.push_back(compute);
        
        float memory_sizes[2] = {5.0f, 5.0f};
        Memory memory(memory_sizes, 8.0f + i * 2.0f, 3.0f + i, 0.1f);
        memory_configs.push_back(memory);
        
        float comm_sizes[2] = {1.0f, 2.0f};
        Communication comm(comm_sizes, 8.0f + i * 2.0f, 0.1f);
        communication_configs.push_back(comm);
    }
    
    wafer_config.wafer_length = 120.0f;
    wafer_config.wafer_width = 120.0f;
    wafer_config.die_padding = 0.5f;
    wafer_config.bandwidth_per_area = 1.0f;
    wafer_config.memory_bandwidth_ratio = 0.6f;
    wafer_config.relaxation = 0.0f;
    wafer_config.threshold.TFLOPS = 0.0f;
    wafer_config.threshold.SRAM_capacity = 0.0f;
    wafer_config.threshold.on_die_mem_capacity = 0.0f;
    wafer_config.threshold.memory_bandwidth = 0.0f;
    wafer_config.threshold.communication_bandwidth = 0.0f;
    
    mt19937 rng(42);
    
    Wafer current = generate_random_initial_wafer(compute_configs, memory_configs, 
                                                   communication_configs, wafer_config, rng);
    
    cout << "  Current wafer: TFLOPS=" << current.get_TFLOPS() << endl;
    
    for (int i = 0; i < 5; i++) {
        Wafer neighbor = generate_neighbor(current, compute_configs, memory_configs, 
                                           communication_configs, wafer_config, rng);
        cout << "  Neighbor " << i << ": TFLOPS=" << neighbor.get_TFLOPS() << endl;
    }
    
    cout << "✓ Neighbor generation test passed" << endl << endl;
}

void test_simulated_annealing() {
    cout << "Testing simulated annealing algorithm..." << endl;
    
    list<Compute> compute_configs;
    list<Memory> memory_configs;
    list<Communication> communication_configs;
    Config wafer_config;
    
    // Create test configurations
    for (int i = 0; i < 3; i++) {
        float compute_sizes[2] = {12.0f + i * 2.0f, 10.0f + i * 2.0f};
        Compute compute(compute_sizes, 8.0f + i * 2.0f, 4.0f + i, 0.0f);
        compute_configs.push_back(compute);
        
        float memory_sizes[2] = {5.0f, 5.0f};
        Memory memory(memory_sizes, 8.0f + i * 2.0f, 3.0f + i, 0.1f);
        memory_configs.push_back(memory);
        
        float comm_sizes[2] = {1.0f, 2.0f};
        Communication comm(comm_sizes, 8.0f + i * 2.0f, 0.1f);
        communication_configs.push_back(comm);
    }
    
    wafer_config.wafer_length = 120.0f;
    wafer_config.wafer_width = 120.0f;
    wafer_config.die_padding = 0.5f;
    wafer_config.bandwidth_per_area = 1.0f;
    wafer_config.memory_bandwidth_ratio = 0.6f;
    wafer_config.relaxation = 0.0f;
    wafer_config.threshold.TFLOPS = 0.0f;
    wafer_config.threshold.SRAM_capacity = 0.0f;
    wafer_config.threshold.on_die_mem_capacity = 0.0f;
    wafer_config.threshold.memory_bandwidth = 0.0f;
    wafer_config.threshold.communication_bandwidth = 0.0f;
    
    SA_Config sa_config;
    sa_config.initial_temperature = 100.0f;
    sa_config.cooling_rate = 0.9f;
    sa_config.min_temperature = 1.0f;
    sa_config.iterations_per_temp = 3;
    sa_config.max_iterations = 30;
    sa_config.random_seed = 42;
    
    cout << "  Running SA with mock simulator..." << endl;
    
    SA_Result result = simulated_annealing_search(
        compute_configs,
        memory_configs,
        communication_configs,
        wafer_config,
        sa_config,
        mock_simulator,
        true  // track trajectory
    );
    
    cout << "  Results:" << endl;
    cout << "    Optimal cycles: " << result.optimal_cycles << endl;
    cout << "    Iterations: " << result.iterations_performed << endl;
    cout << "    Evaluations: " << result.evaluations_performed << endl;
    cout << "    Trajectory length: " << result.visited_wafers.size() << endl;
    
    assert(result.iterations_performed > 0);
    assert(result.evaluations_performed > 0);
    assert(result.optimal_cycles > 0);
    
    cout << "✓ Simulated annealing test passed" << endl << endl;
}

int main() {
    cout << "=== Simulated Annealing Module Tests ===" << endl << endl;
    
    test_random_permutation_generation();
    test_initial_wafer_generation();
    test_neighbor_generation();
    test_simulated_annealing();
    
    cout << "=== All tests passed! ===" << endl;
    
    return 0;
}
