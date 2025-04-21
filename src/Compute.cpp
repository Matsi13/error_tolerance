#include "Compute.h"

using namespace std;

Compute::Compute(float* sizes, float TFLOPS, float capacity, float padding){

    this->sizes[0] = sizes[0];
    this->sizes[1] = sizes[1];
    this->TFLOPS = TFLOPS;
    this->padding = padding;
    this->capacity = capacity;

}


Compute::Compute(const Compute &obj){

    this->sizes[0] = obj.get_size(0);
    this->sizes[1] = obj.get_size(1);
    this->TFLOPS = obj.get_TFLOPS();
    this->padding = obj.get_padding();
    this->capacity = obj.get_capacity();

}


Compute& Compute::operator = (const Compute &obj){

    if(this != &obj){
        this->sizes[0] = obj.get_size(0);
        this->sizes[1] = obj.get_size(1);
        this->TFLOPS = obj.get_TFLOPS();
        this->padding = obj.get_padding();
        this->capacity = obj.get_capacity();
    }
    
    return *this;

}


const float Compute::get_size(int index)const{

    return sizes[index];

}


const float Compute::get_TFLOPS()const{

    return TFLOPS;

}


const float Compute::get_padding()const{

    return padding;

}


const float Compute::get_capacity()const{

    return capacity;

}


void Compute::set_size(float size, int index){

    sizes[index] = size;
    return;

}


void Compute::set_TFLOPS(float TFLOPS){

    this->TFLOPS = TFLOPS;
    return;

}


void Compute::set_padding(float padding){

    this->padding = padding;
    return;
    
}

void Compute::set_capacity(float capacity){

    this->capacity = capacity;
    return;
    
}


