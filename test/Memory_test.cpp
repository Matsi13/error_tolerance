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

    return 0;
}