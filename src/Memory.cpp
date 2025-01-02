#include "Memory.h"

using namespace std;

Memory::Memory(float* sizes, float bandwidth, float capacity, float padding){
    this->sizes[0] = sizes[0];
    this->sizes[1] = sizes[1];
    this->bandwidth = bandwidth;
    this->capacity = capacity;
    this->padding = padding;
}

Memory::Memory(const Memory &obj){
    this->sizes[0] = obj.get_size(0);
    this->sizes[1] = obj.get_size(1);
    this->padding = obj.get_padding();
    this->bandwidth = obj.get_bandwidth();
    this->capacity = obj.get_capacity();
}

Memory& Memory::operator = (const Memory &obj){
    this->sizes[0] = obj.get_size(0);
    this->sizes[1] = obj.get_size(1);
    this->capacity = obj.get_capacity();
    this->bandwidth = obj.get_bandwidth();
    this->padding = obj.get_padding();
    return *this;
}


const float Memory::get_size(int index)const{
    return sizes[index];
}

const float Memory::get_bandwidth()const{
    return bandwidth;
}

const float Memory::get_capacity()const{
    return capacity;
}

const float Memory::get_padding()const{
    return padding;
}

void Memory::set_size(float size, int index){
    sizes[index] = size;
    return;
}

void Memory::set_bandwidth(float bandwidth){
    this->bandwidth = bandwidth;
    return;
}

void Memory::set_capacity(float capacity){
    this->capacity = capacity;
    return;
}

void Memory::set_padding(float padding){
    this->padding = padding;
    return;
}

