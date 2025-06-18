#include <iostream>
#include <fstream>
#include <list>
#include <string>
#include <cmath>

#include "Macro.h"
#include "Compute.h"
#include "Memory.h"
#include "Communication.h"
#include "Die.h"
#include "Wafer.h"

using namespace std;

Wafer Get_optimal(string input_path){

    ifstream input(input_path, ios::in);

    if(!input.is_open()){
        cout << "No existing optimal wafer file!" << endl;
    }
    
    float total_TFLOPS, total_SRAM_capacity, total_DRAM_capacity, total_memory_bandwidth, total_communication_bandwidth;
    int rows, columns;
    float Wafer_sizes[2];
    float Compute_sizes[2], TFLOPS, SRAM_capacity, Compute_padding;
    float Memory_sizes[2], DRAM_capacity, Memory_bandwidth, Memory_padding;
    float Communication_sizes[2], Communication_bandwidth, Communication_padding;
    string up, down, left, right;
    float die_padding, bandwidth_per_area, memory_bandwidth_ratio;
    
    input >> total_TFLOPS >> total_SRAM_capacity >> total_DRAM_capacity >> total_memory_bandwidth >> total_communication_bandwidth;
    input >> rows >> columns >> Wafer_sizes[0] >> Wafer_sizes[1];
    input >> Compute_sizes[0] >> Compute_sizes[1] >> TFLOPS >> SRAM_capacity >> Compute_padding;
    input >> Memory_sizes[0] >> Memory_sizes[1] >> DRAM_capacity >> Memory_bandwidth >> Memory_padding;
    input >> Communication_sizes[0] >> Communication_sizes[1] >> Communication_bandwidth >> Communication_padding;
    input >> up >> down >> left >> right;
    input >> die_padding >> bandwidth_per_area >> memory_bandwidth_ratio;
    up.pop_back();
    down.pop_back();
    left.pop_back();
    right.pop_back();

    Compute Compute_unit(Compute_sizes, TFLOPS, SRAM_capacity, Compute_padding);
    Memory Memory_unit(Memory_sizes, Memory_bandwidth, DRAM_capacity, Memory_padding);
    Communication Communication_unit(Communication_sizes, Communication_bandwidth, Communication_padding);
    
    Die Die_instance(die_padding, bandwidth_per_area, memory_bandwidth_ratio, Compute_unit, Memory_unit, Communication_unit, up, down, left, right);
    Wafer optimal(Wafer_sizes, Die_instance);
    return optimal;
    
}