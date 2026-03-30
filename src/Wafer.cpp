#include <iostream>
#include <cmath>

#include "Die.h"
#include "Wafer.h"

using namespace std;

Wafer::Wafer(float* sizes, Die& die){
    
    this->sizes[0] = sizes[0];
    this->sizes[1] = sizes[1];
    this->die = die;
    update();
    return;
}


Wafer::Wafer(const Wafer& obj){

    this->sizes[0] = obj.get_size(0);
    this->sizes[1] = obj.get_size(1);
    this->die = obj.get_die();
    update();
    return;

}

 
Wafer& Wafer::operator = (const Wafer &obj){

    if (this != &obj){

        this->sizes[0] = obj.get_size(0);
        this->sizes[1] = obj.get_size(1);
        this->die = obj.get_die();
        update();

    }

    return *this;

}


void Wafer::print(){

    cout << TFLOPS<< " " 
         << SRAM_capacity  << " " 
         << on_die_mem_capacity << " " 
         << memory_bandwidth << " " 
         << communication_bandwidth << " " 
         << rows << " " 
         << columns << " " 
         << sizes[0] << " " 
         << sizes[1] << " ";
         
    this->die.print();
    cout << endl;
    return;

}


const float Wafer::get_size(int idx)const{
    if (idx != 0 && idx != 1) return 0.0f;
    return sizes[idx];

}


const float Wafer::get_TFLOPS()const{

    return TFLOPS;

}


const float Wafer::get_SRAM_capacity()const{

    return SRAM_capacity;

}


const float Wafer::get_on_die_mem_capacity()const{

    return on_die_mem_capacity;

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


const int Wafer::get_rows()const{

    return rows;

}


const int Wafer::get_columns()const{

    return columns;

}


void Wafer::set_die(Die& newdie){

    this->die = newdie;
    update();

    return;

}


void Wafer::update(){
    // rows = wafer length (sizes[0]) / die length (size 0); columns = wafer width (sizes[1]) / die width (size 1)
    float die_sizes[2] = {die.get_size(0), die.get_size(1)};
    this -> rows = floor(sizes[0] / die_sizes[0]);
    this -> columns = floor(sizes[1] / die_sizes[1]);
    int die_num = rows * columns;
    this->TFLOPS = die.get_TFLOPS() * die_num;
    this->SRAM_capacity = die.get_SRAM_capacity() * die_num;
    this->on_die_mem_capacity = die.get_on_die_mem_capacity() * die_num;
    this->memory_bandwidth = die.get_memory_bandwidth() * die_num;
    this->communication_bandwidth = die.get_communication_bandwidth() * die_num;

    return;

}

