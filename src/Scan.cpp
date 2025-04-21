#include <iostream>
#include <fstream>
#include <list>
#include <string>

#include "Macro.h"
#include "Compute.h"
#include "Memory.h"
#include "Communication.h"
#include "Die.h"
#include "Wafer.h"
#include "Scan.h"

using namespace std;


// read the configs file and search for solutions
void Scan(list<Compute>& compute_configs, list<Memory>& memory_configs, list<Communication>& communication_configs, string input_path, Config& input_config){
    
    ifstream input(input_path, ios::in);
    string title;

    float compute_sizes[2];
    float TFLOPS;
    float SRAM_capacity;
    float compute_padding;

    float memory_sizes[2];
    float capacity;
    float memory_bandwidth;
    float memory_padding;

    float communication_sizes[2];
    float communication_bandwidth;
    float communication_padding;

    int num;
 
    if (!input.is_open()){
        cout << "No existing configuration file!" << endl;
    }

    input >> title;
    input >> input_config.wafer_length;
    input >> title;
    input >> input_config.wafer_width;
    input >> title;
    input >> input_config.die_padding;
    input >> title;
    input >> input_config.relaxation;
    
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

    input >> title;
    input >> num;
    for (int i = 0; i < num; i++){
        input >> title;
        input >> compute_sizes[0] >> compute_sizes[1];
        input >> title;
        input >> TFLOPS;
        input >> title;
        input >> SRAM_capacity;
        input >> title;
        input >> compute_padding;
        Compute Compute_unit(compute_sizes, TFLOPS, compute_padding, SRAM_capacity);
        compute_configs.push_back(Compute_unit);
    }

    input >> title;
    input >> num;
    for (int i = 0; i < num; i++){
        input >> title;
        input >> memory_sizes[0] >> memory_sizes[1];
        input >> title;
        input >> capacity;
        input >> title;
        input >> memory_bandwidth;
        input >> title;
        input >> memory_padding;
        Memory Memory_unit(memory_sizes, memory_bandwidth, capacity, memory_padding);
        memory_configs.push_back(Memory_unit);
    }

    input >> title;
    input >> num;
    for (int i = 0; i < num; i++){
        input >> title;
        input >> communication_sizes[0] >> communication_sizes[1];
        input >> title;
        input >> communication_bandwidth;
        input >> title;
        input >> communication_padding;
        Communication Communication_unit(communication_sizes, communication_bandwidth, communication_padding);
        communication_configs.push_back(Communication_unit);
    }

    input.close();

    

    


    return;
}