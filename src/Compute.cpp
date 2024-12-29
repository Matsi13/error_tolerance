#include "Compute.h"

using namespace std;

Compute::Compute(float* sizes, float flops){
    this->sizes[0] = sizes[0];
    this->sizes[1] = sizes[1];
    this->flops = flops;
}

float Compute::get_size(int index){
    return sizes[index];
}

float Compute::get_flops(){
    return flops;
}

void Compute::set_size(float size, int index){
    sizes[index] = size;
    return;
}

void Compute::set_flops(float flops){
    this->flops = flops;
    return;
}


