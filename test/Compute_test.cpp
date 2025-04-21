#include <iostream>
#include <cassert>
#include "Compute.h"
using namespace std;

int main(){
    float sizes[2] = {6,10};
    float TFLOPS = 10;
    float SRAM_capacity = 5;
    float padding = 0.5;

    // test construct function

    Compute core(sizes, TFLOPS, SRAM_capacity, padding);

    assert(core.get_size(0) == sizes[0]);
    assert(core.get_size(1) == sizes[1]);
    assert(core.get_TFLOPS() == TFLOPS);
    assert(core.get_capacity() == SRAM_capacity);
    assert(core.get_padding() == padding);

    sizes[0] = 7;
    sizes[1] = 11;
    TFLOPS = 20;
    SRAM_capacity = 10;
    padding = 1;

    // test functions to set parameters

    core.set_size(sizes[0], 0);
    core.set_size(sizes[1], 1);
    core.set_TFLOPS(TFLOPS);
    core.set_capacity(SRAM_capacity);
    core.set_padding(padding);

    assert(core.get_size(0) == sizes[0]);
    assert(core.get_size(1) == sizes[1]);
    assert(core.get_TFLOPS() == TFLOPS);
    assert(core.get_capacity() == SRAM_capacity);
    assert(core.get_padding() == padding);

    //test copy construct function

    Compute copy(core);

    assert(copy.get_size(0) == sizes[0]);
    assert(copy.get_size(1) == sizes[1]);
    assert(copy.get_TFLOPS() == TFLOPS);
    assert(core.get_capacity() == SRAM_capacity);
    assert(copy.get_padding() == padding);

    // test operator =

    sizes[0] = 8;
    sizes[1] = 10;
    TFLOPS = 15;
    SRAM_capacity = 8;
    padding = 0.4;

    Compute alias(sizes, TFLOPS, SRAM_capacity, padding);
    alias = copy;
    assert(copy.get_size(0) == alias.get_size(0));
    assert(copy.get_size(1) == alias.get_size(1));
    assert(copy.get_TFLOPS() == alias.get_TFLOPS());
    assert(copy.get_capacity() == alias.get_capacity());
    assert(copy.get_padding() == alias.get_padding());


    
    return 0;
}