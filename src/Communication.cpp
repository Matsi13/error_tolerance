#include "Communication.h"


Communication::Communication(float* sizes, float bandwidth, float padding){
    this->sizes[0] = sizes[0];
    this->sizes[1] = sizes[1];
    this->bandwidth = bandwidth;
    this->padding = padding;
}

float Communication::get_size(int index){
    return sizes[index];
}

float Communication::get_bandwidth(){
    return bandwidth;
}

float Communication::get_padding(){
    return padding;
}

void Communication::set_size(float size, int index){
    sizes[index] = size;
    return;
}

void Communication::set_bandwidth(float bandwidth){
    this->bandwidth = bandwidth;
    return;
}

void Communication::set_padding(float padding){
    this->padding = padding;
    return;
}

