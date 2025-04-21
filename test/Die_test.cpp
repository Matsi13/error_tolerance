#include <iostream>
#include <cassert>
#include <string>
#include <algorithm>

#include "Macro.h"
#include "Compute.h"
#include "Memory.h"
#include "Communication.h"
#include "Die.h"

using namespace std;

int main(){

    float Compute_sizes[2] = {12,16};
    float TFLOPS = 10;
    float SRAM_capacity = 5;
    float Compute_padding = 0.5;
    Compute Compute_unit(Compute_sizes, TFLOPS, SRAM_capacity, Compute_padding);

    float Memory_sizes[2] = {5, 5};
    float Memory_bandwidth = 4;
    float Memory_capacity = 24;
    float Memory_padding = 0.2;
    Memory Memory_unit(Memory_sizes, Memory_bandwidth, Memory_capacity, Memory_padding);

    float Communication_sizes[2] = {1, 2};
    float Communication_bandwidth = 1;
    float Communication_padding = 0.3;
    Communication Communication_unit(Communication_sizes, Communication_bandwidth, Communication_padding);

    float die_padding = 2;
    float bandwidth_per_area = 1;
    float memory_bandwidth_ratio = 0.6;
    string up = {MEMORY_UNIT, MEMORY_UNIT};
    string down = {MEMORY_UNIT, MEMORY_UNIT};
    string left = {MEMORY_UNIT, COMMUNICATION_UNIT, COMMUNICATION_UNIT, MEMORY_UNIT};
    string right = {MEMORY_UNIT, COMMUNICATION_UNIT, COMMUNICATION_UNIT, MEMORY_UNIT};

    Die Die_instance(die_padding, bandwidth_per_area, memory_bandwidth_ratio, Compute_unit, Memory_unit, Communication_unit, up, down, left, right);
    // cout << Die_instance.get_size(0) << endl;
    // cout << Die_instance.get_size(1) << endl;
    assert(Die_instance.get_size(0) == 26);
    assert(Die_instance.get_size(1) == 30);
    assert(Die_instance.get_padding() == 2);
    assert(Die_instance.get_TFLOPS() == 10);
    assert(Die_instance.get_SRAM_capacity() == 5);
    assert(Die_instance.get_DRAM_capacity() == 192);
    assert(Die_instance.get_memory_bandwidth() == 32);
    assert(Die_instance.get_communication_bandwidth() == 4);
    


    return 0;
}