#ifndef SIMULATED_ANNEALING_H
#define SIMULATED_ANNEALING_H

#include <string>
#include <list>
#include <random>
#include <functional>

#include "Macro.h"
#include "Compute.h"
#include "Memory.h"
#include "Communication.h"
#include "Die.h"
#include "Wafer.h"

using namespace std;

// Callback function type for external simulator (ns-3)
// Takes a wafer configuration and returns execution cycles
using SimulatorCallback = function<float(const Wafer&)>;

// Simulated annealing parameters
struct SA_Config {
    float initial_temperature;
    float cooling_rate;           // Multiplicative cooling (T = T * cooling_rate)
    float min_temperature;
    int iterations_per_temp;
    int max_iterations;
    unsigned int random_seed;
};

// Result of simulated annealing search
struct SA_Result {
    Wafer optimal_wafer;
    float optimal_cycles;
    int iterations_performed;
    int evaluations_performed;
    list<Wafer> visited_wafers;   // Optional: track search trajectory
};

// Generate a neighbor wafer by modifying the current die configuration
Wafer generate_neighbor(const Wafer& current, 
                        const list<Compute>& compute_configs,
                        const list<Memory>& memory_configs, 
                        const list<Communication>& communication_configs,
                        const Config& wafer_config,
                        mt19937& rng);

// Simulated annealing search for optimal wafer
SA_Result simulated_annealing_search(
    const list<Compute>& compute_configs,
    const list<Memory>& memory_configs,
    const list<Communication>& communication_configs,
    const Config& wafer_config,
    const SA_Config& sa_config,
    SimulatorCallback simulator,
    bool track_trajectory = false
);

// Helper: Generate random initial wafer that meets threshold
Wafer generate_random_initial_wafer(
    const list<Compute>& compute_configs,
    const list<Memory>& memory_configs,
    const list<Communication>& communication_configs,
    const Config& wafer_config,
    mt19937& rng
);

// Helper: Generate random permutation string
string generate_random_permutation(
    float compute_edge_size,
    const Memory& memory_unit,
    const Communication& communication_unit,
    float relaxation,
    mt19937& rng
);

#endif
