#include <iostream>
#include <cassert>
#include "Compute.h"
using namespace std;

int main(){
    float sizes[2] = {6,10};
    float flops = 100;

    Compute core(sizes, flops);

    assert(core.get_size(0) == sizes[0]);
    assert(core.get_size(1) == sizes[1]);
    assert(core.get_flops() == flops);

    sizes[0] = 7;
    sizes[1] = 11;
    flops = 200;

    core.set_size(sizes[0], 0);
    core.set_size(sizes[1], 1);
    core.set_flops(flops);

    assert(core.get_size(0) == sizes[0]);
    assert(core.get_size(1) == sizes[1]);
    assert(core.get_flops() == flops);
    
    return 0;
}