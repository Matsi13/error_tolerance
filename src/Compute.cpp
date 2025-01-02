#include "Compute.h"

using namespace std;

Compute::Compute(float* sizes, float tflops, float padding){
    this->sizes[0] = sizes[0];
    this->sizes[1] = sizes[1];
    this->tflops = tflops;
    this->padding = padding;
}

Compute::Compute(const Compute &obj){
    this->sizes[0] = obj.get_size(0);
    this->sizes[1] = obj.get_size(1);
    this->tflops = obj.get_tflops();
    this->padding = obj.get_padding();
}

Compute& Compute::operator = (const Compute &obj){
    this->sizes[0] = obj.get_size(0);
    this->sizes[1] = obj.get_size(1);
    this->tflops = obj.get_tflops();
    this->padding = obj.get_padding();
    return *this;
}

const float Compute::get_size(int index)const{
    return sizes[index];
}

const float Compute::get_tflops()const{
    return tflops;
}

const float Compute::get_padding()const{
    return padding;
}

void Compute::set_size(float size, int index){
    sizes[index] = size;
    return;
}

void Compute::set_tflops(float tflops){
    this->tflops = tflops;
    return;
}

void Compute::set_padding(float padding){
    this->padding = padding;
    return;
}


