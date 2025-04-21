#include <list>
#include <string>
#include <fstream>
#include <iostream>

#include "Macro.h"
#include "Compute.h"
#include "Memory.h"
#include "Communication.h"
#include "Scan.h"
#include "Wafer.h"
#include "Permutation.h"

using namespace std;

int main(){

    string input_path("D:\\VS projects\\error_tolerance\\configuration\\component_configuration.txt");
    // string output_path("D:\\VS projects\\error_tolerance\\out\\output\\scan_wafer.txt");
    list<Compute> compute_configs;
    list<Memory> memory_configs;
    list<Communication> communication_configs;
    Config input_config;

    list<list<Wafer>> results_by_configs;
    list<Wafer> result;

    Scan(compute_configs, memory_configs, communication_configs, input_path, input_config);

    for (auto compute_idx = compute_configs.begin(); compute_idx != compute_configs.end(); compute_idx++){
        for (auto memory_idx = memory_configs.begin(); memory_idx != memory_configs.end(); memory_idx++){
            for (auto communication_idx = communication_configs.begin(); communication_idx != communication_configs.end(); communication_idx++){
                
                Permutation(*compute_idx, *memory_idx, *communication_idx, input_config.die_padding, input_config.bandwidth_per_area, input_config.memory_bandwidth_ratio, result, input_config.relaxation, input_config.wafer_length, input_config.wafer_width, input_config.threshold);
                results_by_configs.push_back(result);

            }
        }
    }
    
    int config_num = 0, wafer_num = 0;
    for(auto config_idx = results_by_configs.begin(); config_idx != results_by_configs.end(); config_idx++){

        config_num++;
        cout << "config number " << config_num << endl;
        list<Wafer> result_by_config = *config_idx;

        for(auto wafer_idx = result_by_config.begin(); wafer_idx != result_by_config.end(); wafer_idx++){

            cout << "wafer number" << wafer_num << endl;
            Wafer solution = *wafer_idx;
            solution.print();

        }

    }

    return 0;
}


