#include <iostream>
#include <fstream>
#include <list>
#include <string>
#include <cmath>

#include "Macro.h"
#include "Compute.h"
#include "Memory.h"
#include "Communication.h"

using namespace std;

void Generate(list<Compute>& compute_configs, list<Memory>& memory_configs, list<Communication>& communication_configs, string input_path, Config& input_config){

    ifstream input(input_path, ios::in);
    if(!input.is_open()){
        cout << "No existing configutation generator file!" << endl;
    }
    
    string title;
    int num;
    
    // read wafer configs
    input >> title;
    input >> input_config.wafer_length;
    input >> title;
    input >> input_config.wafer_width;
    input >> title;
    input >> input_config.die_padding;
    input >> title;
    input >> input_config.relaxation;

    // read performace threshold
    input >> title;
    input >> input_config.threshold.TFLOPS;
    input >> title;
    input >> input_config.threshold.SRAM_capacity;
    input >> title;
    input >> input_config.threshold.DRAM_capacity;
    input >> title;
    input >> input_config.threshold.memory_bandwidth;
    input >> title;
    input >> input_config.threshold.communication_bandwidth;

    // generate compute configs
    input >> title;
    input >> num;

    float TFLOPS_per_area;
    float SRAM_capacity_per_area;
    float compute_length_min;
    float compute_length_max;
    float compute_length_stride;
    float compute_width_min;
    float compute_width_max;
    float compute_width_stride;
    float compute_padding;

    for (int i = 0; i < num; i++){

        input >> title;
        input >> TFLOPS_per_area;
        input >> title;
        input >> SRAM_capacity_per_area;
        input >> title;
        input >> compute_length_min >> compute_length_stride >> compute_length_max;
        input >> title;
        input >> compute_width_min >> compute_width_stride >> compute_width_max;
        input >> title;
        input >> compute_padding;

        int length_num = (compute_length_min == compute_length_max) ? 1 : floor((compute_length_max - compute_length_min) / compute_length_stride);
        int width_num = (compute_width_min == compute_width_max) ? 1 : floor((compute_width_max - compute_width_min) / compute_width_stride);

        for (int j = 0; j < length_num; j++){

            float compute_length = compute_length_min + j * compute_length_stride;

            for(int k = 0; k < width_num; k++){

                float compute_width = compute_width_min + k * compute_width_stride;
                float TFLOPS = compute_length * compute_width * TFLOPS_per_area;
                float SRAM_capacity = compute_length * compute_width * SRAM_capacity_per_area;
                float compute_sizes[2] = {compute_length, compute_width};
                Compute compute_instance(compute_sizes, TFLOPS, compute_padding, SRAM_capacity);
                compute_configs.push_back(compute_instance);

            }
        }

    }

    // generate memory configs
    input >> title;
    input >> num;

    float capacity_per_area;
    float memory_bandwidth_per_length;
    float memory_length_min;
    float memory_length_max;
    float memory_length_stride;
    float memory_width_min;
    float memory_width_max;
    float memory_width_stride;
    float memory_padding;

    for (int i = 0; i < num; i++){

        input >> title;
        input >> capacity_per_area;
        input >> title;
        input >> memory_bandwidth_per_length;
        input >> title;
        input >> memory_length_min >> memory_length_stride >> memory_length_max;
        input >> title;
        input >> memory_width_min >> memory_width_stride >> memory_width_max;
        input >> title;
        input >> memory_padding;

        int length_num = (memory_length_min == memory_length_max) ? 1 : floor((memory_length_max - memory_length_min) / memory_length_stride);
        int width_num = (memory_width_min == memory_width_max) ? 1 : floor((memory_width_max - memory_width_min) / memory_width_stride);

        for (int j = 0; j < length_num; j++){
            
            float memory_length = memory_length_min + j * memory_length_stride;

            for (int k = 0; k < width_num; k++){

                float memory_width = memory_width_min + k * memory_width_stride;
                float memory_capacity = capacity_per_area * memory_length * memory_width;
                float memory_bandwidth = memory_bandwidth_per_length * memory_length;
                float memory_sizes[2] = {memory_length, memory_width};
                Memory memory_instance(memory_sizes, memory_bandwidth, memory_capacity, memory_padding);
                memory_configs.push_back(memory_instance);

            }
        }
    }

    input >> title;
    input >> num;

    float communication_bandwidth_per_length;
    float communication_length_min, communication_length_max, communication_length_stride;
    float communication_width_min, communication_width_max, communication_width_stride;
    float communication_padding;

    for (int i = 0; i < num; i++){

        input >> title;
        input >> communication_bandwidth_per_length;
        input >> title;
        input >> communication_length_min >> communication_length_stride >> communication_length_max;
        input >> title;
        input >> communication_width_min >> communication_width_stride >> communication_width_max;
        input >> title;
        input >> communication_padding;

        int length_num = (communication_length_min == communication_length_max) ? 1 : floor((communication_length_max - communication_length_min) / communication_length_stride);
        int width_num = (communication_width_min == communication_width_max) ? 1 : floor((communication_width_max - communication_width_min) / communication_width_stride);

        for (int j = 0; j < length_num; j++){

            float communication_length = communication_length_min + j * communication_length_stride;
            
            for(int k = 0; k < width_num; k++){

                float communication_width = communication_width_min + k * communication_width_stride;
                float communication_sizes[2] = {communication_length, communication_width};
                float communication_bandwidth = communication_bandwidth_per_length * communication_length;
                Communication communication_instance(communication_sizes, communication_bandwidth, communication_padding);
                communication_configs.push_back(communication_instance);

            }
        }        
    }
}