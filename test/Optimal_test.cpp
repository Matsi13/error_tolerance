#include <iostream>
#include <fstream>
#include <string>
#include <list>

#include "Macro.h"
#include "Compute.h"
#include "Memory.h"
#include "Communication.h"
#include "Wafer.h"
#include "Generate.h"
#include "Permutation.h"
#include "Distance.h"
#include "Optimal.h"

using namespace std;

int main(){


    string input_path("/data/login_home/lijinxi/error_tolerance/configuration/auto_generate.txt");
    list<Compute> compute_configs;
    list<Memory> memory_configs;
    list<Communication> communication_configs;
    Config input_config;

    Generate(compute_configs, memory_configs, communication_configs, input_path, input_config);

    list<list<Wafer>> results_by_configs;
    list<Wafer> result;

    for (auto compute_idx = compute_configs.begin(); compute_idx != compute_configs.end(); compute_idx++){
        for (auto memory_idx = memory_configs.begin(); memory_idx != memory_configs.end(); memory_idx++){
            for (auto communication_idx = communication_configs.begin(); communication_idx != communication_configs.end(); communication_idx++){
                
                Permutation(*compute_idx, *memory_idx, *communication_idx, input_config.die_padding, input_config.bandwidth_per_area, input_config.memory_bandwidth_ratio, result, input_config.relaxation, input_config.wafer_length, input_config.wafer_width, input_config.threshold);
                results_by_configs.push_back(result);

            }
        }
    }

    list<Wafer> solutions;

    for (auto config_idx = results_by_configs.begin(); config_idx != results_by_configs.end(); config_idx++){
        list<Wafer> wafers_in_config = *config_idx;
        for (auto wafer_idx = wafers_in_config.begin(); wafer_idx != wafers_in_config.end(); wafer_idx++){
            Wafer solution = *wafer_idx;
            solutions.push_back(solution);
        }
    }


    string optimal_input_path("/data/login_home/lijinxi/error_tolerance/configuration/optimal_wafer.txt");
    Wafer optimal = Get_optimal(optimal_input_path);
    
    Arch_error error;

    error.TFLOPS_positive = 0.2;
    error.TFLOPS_negative = 0.2;
    error.SRAM_capacity_positive = 0.2;
    error.SRAM_capacity_negative = 0.2;
    error.DRAM_capacity_positive = 0.2;
    error.DRAM_capacity_negative = 0.2;
    error.memory_bandwidth_positive = 0.2;
    error.memory_bandwidth_negative = 0.2;
    error.communication_bandwidth_positive = 0.2;
    error.communication_bandwidth_negative = 0.2;


    list<Wafer> possible_optimals;
    Possible_optimal(solutions, optimal, error, possible_optimals);

    for (auto wafer_idx = possible_optimals.begin(); wafer_idx != possible_optimals.end(); wafer_idx++){

        Wafer possible_solution = *wafer_idx;
        possible_solution.print();

    }
    
    return 0;
}