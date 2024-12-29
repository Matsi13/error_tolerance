#include "Memory.h"

using namespace std;

Memory::Memory(float* sizes, float bandwidth, float capacity, float padding){
    this->sizes[0] = sizes[0];
    this->sizes[1] = sizes[1];
    this->bandwidth = bandwidth;
    this->capacity = capacity;
    this->padding = padding;
}

float Memory::get_size(int index){
    return sizes[index];
}

float Memory::get_bandwidth(){
    return bandwidth;
}

float Memory::get_capacity(){
    return capacity;
}

float Memory::get_padding(){
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

