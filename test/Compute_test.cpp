#include <iostream>
#include <cassert>
#include "Compute.h"
using namespace std;

int main(){
    float sizes[2] = {6,10};
    float tflops = 10;
    float padding = 0.5;

    // test construct function

    Compute core(sizes, tflops, padding);

    assert(core.get_size(0) == sizes[0]);
    assert(core.get_size(1) == sizes[1]);
    assert(core.get_tflops() == tflops);
    assert(core.get_padding() == padding);

    sizes[0] = 7;
    sizes[1] = 11;
    tflops = 20;
    padding = 1;

    // test functions to set parameters

    core.set_size(sizes[0], 0);
    core.set_size(sizes[1], 1);
    core.set_tflops(tflops);
    core.set_padding(padding);

    assert(core.get_size(0) == sizes[0]);
    assert(core.get_size(1) == sizes[1]);
    assert(core.get_tflops() == tflops);
    assert(core.get_padding() == padding);

    //test copy construct function

    Compute copy(core);

    assert(copy.get_size(0) == sizes[0]);
    assert(copy.get_size(1) == sizes[1]);
    assert(copy.get_tflops() == tflops);
    assert(copy.get_padding() == padding);

    // test operator =

    sizes[0] = 8;
    sizes[1] = 10;
    tflops = 15;
    padding = 0.4;

    Compute alias(sizes, tflops, padding);
    alias = copy;
    assert(copy.get_size(0) == alias.get_size(0));
    assert(copy.get_size(1) == alias.get_size(1));
    assert(copy.get_tflops() == alias.get_tflops());
    assert(copy.get_padding() == alias.get_padding());


    
    return 0;
}