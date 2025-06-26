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
#include "Astra_API.h"

#define FACTOR (1024*512)

using namespace std;


int main(){

    float freq = 1e9; // Hz
    float off_chip_bandwidth_per_die = 4; // GB/s
    float param_count = 5e9; // lamma 5B
    float batch_size = 128;
    float prompt_avg = 128;
    float output_avg = 1024;

    string path_workload("/data/login_home/lijinxi/error_tolerance/output/workload");
    string path_physical_network("/data/login_home/lijinxi/error_tolerance/output/physical_network");
    string config("0060");
    
    float TFLOPs = batch_size * param_count  * (prompt_avg + output_avg) / (1 + output_avg) * 1e-12;
    float model_size = param_count * sizeof(float) * 1e-9; //GB
    float kv_cache_size = batch_size * (prompt_avg + output_avg / 2 ) * (param_count/ FACTOR) * sizeof(float)* 1e-9;
    float traffic = batch_size * (prompt_avg + output_avg / 2 )* sizeof(float) * 1e-9;



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


    astra_API(freq, off_chip_bandwidth_per_die, TFLOPs, model_size, kv_cache_size, traffic, solutions, path_workload, path_physical_network, config);

    return 0;
}