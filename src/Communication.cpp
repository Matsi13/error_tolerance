#include "Communication.h"


Communication::Communication(float* sizes, float bandwidth, float padding){
    this->sizes[0] = sizes[0];
    this->sizes[1] = sizes[1];
    this->bandwidth = bandwidth;
    this->padding = padding;
}

Communication::Communication(const Communication &obj){
    this->sizes[0] = obj.get_size(0);
    this->sizes[1] = obj.get_size(1);
    this->bandwidth = obj.get_bandwidth();
    this->padding = obj.get_padding();
}

const float Communication::get_size(int index)const{
    return sizes[index];
}

const float Communication::get_bandwidth()const{
    return bandwidth;
}

const float Communication::get_padding()const{
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

