#include <iostream>
#include <cassert>
#include "Memory.h"
using namespace std;

int main(){
    float sizes[2] = {6,10};
    float bandwidth = 10;
    float capacity  = 24;
    float padding = 1;

    Memory HBM(sizes, bandwidth, capacity, padding);

    assert(HBM.get_size(0) == sizes[0]);
    assert(HBM.get_size(1) == sizes[1]);
    assert(HBM.get_bandwidth() == bandwidth);
    assert(HBM.get_capacity() == capacity);
    assert(HBM.get_padding() == padding);

    sizes[0] = 7;
    sizes[1] = 11;
    bandwidth = 9;
    capacity = 20;
    padding = 2;

    HBM.set_size(sizes[0], 0);
    HBM.set_size(sizes[1], 1);
    HBM.set_bandwidth(bandwidth);
    HBM.set_capacity(capacity);
    HBM.set_padding(padding);

    assert(HBM.get_size(0) == sizes[0]);
    assert(HBM.get_size(1) == sizes[1]);
    assert(HBM.get_bandwidth() == bandwidth);
    assert(HBM.get_capacity() == capacity);
    assert(HBM.get_padding() == padding);

    Memory DRAM(HBM);

    assert(DRAM.get_size(0) == sizes[0]);
    assert(DRAM.get_size(1) == sizes[1]);
    assert(DRAM.get_bandwidth() == bandwidth);
    assert(DRAM.get_capacity() == capacity);
    assert(DRAM.get_padding() == padding);

    // test operator =
    
    sizes[0] = 2;
    sizes[1] = 4;
    bandwidth = 20;
    capacity = 5;
    padding = 0.5;

    Memory SRAM(sizes, bandwidth, capacity, padding);
    SRAM = DRAM;
    
    assert(DRAM.get_size(0) == SRAM.get_size(0));
    assert(DRAM.get_size(1) == SRAM.get_size(1));
    assert(DRAM.get_bandwidth() == SRAM.get_bandwidth());
    assert(DRAM.get_capacity() == SRAM.get_capacity());
    assert(DRAM.get_padding() == SRAM.get_padding());



    return 0;
}