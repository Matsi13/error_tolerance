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
    float Compute_padding = 0.5;
    Compute Compute_unit(Compute_sizes, TFLOPS, Compute_padding);

    float Memory_sizes[2] = {5, 5};
    float Memory_bandwidth = 4;
    float Memory_capacity = 24;
    float Memory_padding = 0.2;
    Memory Memory_unit(Memory_sizes, Memory_bandwidth, Memory_capacity, Memory_padding);

    float Communication_sizes[2] = {1, 2};
    float Communication_bandwidth = 1;
    float Communication_padding = 0.3;
    Communication Communication_unit(Communication_sizes, Communication_bandwidth, Communication_padding);

    float Die_padding = 2;
    string up = {MEMORY_UNIT, MEMORY_UNIT};
    string down = {MEMORY_UNIT, MEMORY_UNIT};
    string left = {MEMORY_UNIT, COMMUNICATION_UNIT, COMMUNICATION_UNIT, MEMORY_UNIT};
    string right = {MEMORY_UNIT, COMMUNICATION_UNIT, COMMUNICATION_UNIT, MEMORY_UNIT};

    Die Die_instance(Die_padding, Compute_unit, Memory_unit, Communication_unit, up, down, left, right);
    // cout << Die_instance.get_size(0) << endl;
    // cout << Die_instance.get_size(1) << endl;
    assert(Die_instance.get_size(0) == 26);
    assert(Die_instance.get_size(1) == 30);
    assert(Die_instance.get_padding() == 2);
    assert(Die_instance.get_TFLOPS() == 10);
    assert(Die_instance.get_capacity() == 192);
    assert(Die_instance.get_memory_bandwidth() == 32);
    assert(Die_instance.get_communication_bandwidth() == 4);
    


    return 0;
}