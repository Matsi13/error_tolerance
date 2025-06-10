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
#include "Analysis.h"

using namespace std;

int main(){

    int negative_flag;
    int iters;
    float stride;

    cin >> negative_flag >> iters >> stride;

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

    int index_mid = (solutions.size() + 1) / 2 - 1;

    auto idx = solutions.begin();
    advance(idx, index_mid);
    Wafer optimal = *idx;
    
    Workload_error error;

    error.TFLOPs_positive = 0;
    error.TFLOPs_negative = 0;
    error.paramsize_positive = 0;
    error.paramsize_negative = 0;
    error.access_positive = 0;
    error.access_negative = 0;
    error.traffic_positive = 0;
    error.traffic_negative = 0;

    Workload transformer_workload(4e8, 2e9, 2e10, 2e-5);
    float off_wafer_bandwidth = 100;
    

    for(int i = 0; i < iters; i++){

        error.paramsize_positive = i * stride;
        error.paramsize_negative = -negative_flag * i * stride;
        list<Wafer> possible_optimals;

        Possible_optimal_wafers(transformer_workload, solutions, possible_optimals, error, off_wafer_bandwidth);
        cout << error.paramsize_positive << " " << possible_optimals.size() << endl;

    }

    // for(int i = 0; i < 20; i++){

    //     error.TFLOPs_positive = i * 0.01;
    //     error.TFLOPs_negative = -i * 0.01;

    //         for (int j = 0; j < 20; j++){

    //             list<Wafer> possible_optimals;
    //             error.paramsize_positive = j * 0.01;
    //             error.paramsize_negative = -j * 0.01;
    //             // cout << i << " " << j << " " << endl;

    //             Possible_optimal_wafers(transformer_workload, solutions, possible_optimals, error, off_wafer_bandwidth);
    //             cout << error.TFLOPs_positive << " " << error.paramsize_positive << " " << possible_optimals.size() << endl;

    //         }
        
    // }

    return 0;
}