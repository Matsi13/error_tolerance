#include <iostream>

#include "Die.h"
#include "Wafer.h"

using namespace std;

Wafer::Wafer(float* sizes, Die& die){
    
    this->sizes[0] = sizes[0];
    this->sizes[1] = sizes[1];
    this->die = die;
    update();
}


Wafer::Wafer(const Wafer& obj){

    this->sizes[0] = obj.get_size(0);
    this->sizes[1] = obj.get_size(1);
    this->die = obj.get_die();
    update();

}


void Wafer::print(){

    cout << TFLOPS<< " " << capacity << " " << memory_bandwidth << " " << communication_bandwidth << endl;

}


const float Wafer::get_size(int idx)const{

    return sizes[idx];

}


const float Wafer::get_TFLOPS()const{

    return TFLOPS;

}


const float Wafer::get_capacity()const{

    return capacity;

}


const float Wafer::get_memory_bandwidth()const{

    return memory_bandwidth;

}


const float Wafer::get_communication_bandwidth()const{

    return communication_bandwidth;
}


const Die Wafer::get_die()const{

    return die;

}

void Wafer::set_die(Die& newdie){

    this->die = newdie;
    update();

}


void Wafer::update(){

    float die_sizes[2] = {die.get_size(0), die.get_size(1)};
    int rows = floor(sizes[0] / die_sizes[0]);
    int columns = floor(sizes[1] / die_sizes[1]);
    int die_num = rows * columns;
    this->TFLOPS = die.get_TFLOPS() * die_num;
    this->capacity = die.get_capacity() * die_num;
    this->memory_bandwidth = die.get_memory_bandwidth() * die_num;
    this->communication_bandwidth = die.get_communication_bandwidth() * die_num;

}

