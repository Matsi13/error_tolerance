#include <string>
#include <algorithm>
#include <iostream>

#include "Macro.h"
#include "Die.h"
#include "Compute.h"
#include "Communication.h"
#include "Memory.h"

using namespace std;

Die::Die(float padding, Compute& Compute_unit, Memory& Memory_unit, Communication& Communication_unit, string& up, string& down, string& left, string& right){

    this->padding = padding;
    this->Compute_unit = Compute_unit;
    this->Memory_unit = Memory_unit;
    this->Communication_unit = Communication_unit;
    this->up = up;
    this->down = down;
    this->left = left;
    this->right = right;

    update();
    
}


Die::Die(const Die& obj){

    this->padding = obj.get_padding();
    this->Compute_unit = obj.get_Compute_unit();
    this->Memory_unit = obj.get_Memory_unit();
    this->Communication_unit = obj.get_Communication_unit();
    this->up = obj.get_permutation_up();
    this->down = obj.get_permutation_down();
    this->left = obj.get_permutation_left();
    this->right = obj.get_permutation_right();

    update();
    
}


Die& Die::operator = (const Die &obj){

    if (this != &obj){

        this->padding = obj.get_padding();
        this->Compute_unit = obj.get_Compute_unit();
        this->Memory_unit = obj.get_Memory_unit();
        this->Communication_unit = obj.get_Communication_unit();
        this->up = obj.get_permutation_up();
        this->down = obj.get_permutation_down();
        this->left = obj.get_permutation_left();
        this->right = obj.get_permutation_right();

        update();

    }

    return *this;

}


Die::~Die(){

    up.clear();
    down.clear();
    left.clear();
    right.clear();
    
}


const float Die::get_size(int index)const{
    
    return sizes[index];

}


const float Die::get_padding()const{

    return padding;
    
}


const float Die::get_TFLOPS()const{
    
    return TFLOPS;

}


const float Die::get_DRAM_capacity()const{
    
    return DRAM_capacity;

}


const float Die::get_SRAM_capacity()const{
    
    return SRAM_capacity;

}


const float Die::get_memory_bandwidth()const{

    return memory_bandwidth;
    
}


const float Die::get_communication_bandwidth()const{
    
    return communication_bandwidth;

}


const Compute Die::get_Compute_unit()const{

    return Compute_unit;
    
}


const Memory Die::get_Memory_unit()const{
    
    return Memory_unit;
}


const Communication Die::get_Communication_unit()const{

    return Communication_unit;
    
}


const string Die::get_permutation_up()const{
    
    return up;
    
}


const string Die::get_permutation_down()const{

    return down;
    
}


const string Die::get_permutation_left()const{
    
    return left;

}


const string Die::get_permutation_right()const{
    
    return right;

}


void Die::set_size(float size, int index){

    sizes[index] = size;
    return;

}


void Die::set_TFLOPS(float TFLOPS){

    this->TFLOPS = TFLOPS;
    return;
    
}


void Die::set_DRAM_capacity(float capacity){

    this->DRAM_capacity = capacity;
    return;
    
}


void Die::set_SRAM_capacity(float capacity){

    this->SRAM_capacity = capacity;
    return;
    
}


void Die::set_memory_bandwidth(float memory_bandwidth){

    this->memory_bandwidth = memory_bandwidth;
    return;
    
}


void Die::set_communication_bandwidth(float communication_bandwidth){

    this->communication_bandwidth = communication_bandwidth;
    return;
    
}


void Die::update_size(){
    
    float max_length = 0; // the size from left to right
    float max_width = 0; // the size from top to bottom
    
    float components_padding = max(Memory_unit.get_padding(), Communication_unit.get_padding()); // the padding between Memory unit and Communication unit

    float length_left2right = Compute_unit.get_size(0); // the length calculated by left and right components
    float length_up2down = Compute_unit.get_size(0); // the length calculated by up and down components
    float width_up2down = Compute_unit.get_size(1); // the width calculated by up and down components
    float width_left2right = Compute_unit.get_size(1); // the width calculated by up and down components
    
    if (!up.empty()){

        float length = 0, width = 0;

        for (int i = 0; i < up.size(); i++){

            switch(up[i]){
                case MEMORY_UNIT : {
                    length += Memory_unit.get_size(0);
                    width = max(width, Memory_unit.get_size(1));
                    break;
                }
                case COMMUNICATION_UNIT : {
                    length += Communication_unit.get_size(0);
                    width = max(width, Communication_unit.get_size(1));
                    break;
                }
                default : break;
            }
            
        }

        length += (up.size() - 1) * components_padding;

        length_up2down = max(length_left2right, length);
        width_up2down += width;

        // cout << "length_up2down in up: " << length_up2down << endl;
        // cout << "width_up2down in up: " << width_up2down << endl;

    }

    if (!down.empty()){

        float length = 0, width = 0;

        for (int i = 0; i < down.size(); i++){

            switch(down[i]){
                case MEMORY_UNIT : {
                    length += Memory_unit.get_size(0);
                    width = max(width, Memory_unit.get_size(1));
                    break;
                }
                case COMMUNICATION_UNIT : {
                    length += Communication_unit.get_size(0);
                    width = max(width, Communication_unit.get_size(1));
                    break;
                }
                default : break;
            }
            
        }

        length += (down.size() - 1) * components_padding;

        length_up2down = max(length_left2right, length);
        width_up2down += width;

        // cout << "length_up2down in down: " << length_up2down << endl;
        // cout << "width_up2down in down: " << width_up2down << endl;

    }

    if (!left.empty()){

        float length = 0, width = 0;

        for (int i = 0; i < left.size(); i++){

            switch(left[i]){
                case MEMORY_UNIT : {
                    width += Memory_unit.get_size(0);
                    length = max(length, Memory_unit.get_size(1));
                    break;
                }
                case COMMUNICATION_UNIT : {
                    width += Communication_unit.get_size(0);
                    length = max(length, Communication_unit.get_size(1));
                    break;
                }
                default : break;
            }
            
        }

        width += (left.size() - 1) * components_padding;

        length_left2right += length;
        width_left2right = max(width, width_left2right);

        // cout << "length_left2right in left: " << length_left2right << endl;
        // cout << "width_left2right in left: " << width_left2right << endl;

    }

    if (!right.empty()){

        float length = 0, width = 0;

        for (int i = 0; i < right.size(); i++){

            switch(right[i]){
                case MEMORY_UNIT : {
                    width += Memory_unit.get_size(0);
                    length = max(length, Memory_unit.get_size(1));
                    break;
                }
                case COMMUNICATION_UNIT : {
                    width += Communication_unit.get_size(0);
                    length = max(length, Communication_unit.get_size(1));
                    break;
                }
                default : break;
            }
            
        }

        width += (right.size() - 1) * components_padding;

        length_left2right += length;
        width_left2right = max(width, width_left2right);

        // cout << "length_left2right in right: " << length_left2right << endl;
        // cout << "width_left2right in right: " << width_left2right << endl;

    }

    max_length = max(length_left2right, length_up2down) + 2 * get_padding();
    max_width = max(width_left2right, width_up2down) + 2 * get_padding();

    set_size(max_length, 0);
    set_size(max_width, 1);
    return;

}


void Die::update_TFLOPS(){

    set_TFLOPS(Compute_unit.get_TFLOPS());
    return;

}


void Die::update_DRAM_capacity(){

    float capacity = 0;

    if (!up.empty()){

        for (int i = 0; i < up.size(); i++){

            if (up[i] == MEMORY_UNIT){
                capacity += Memory_unit.get_capacity();
            }
            
        }

    }

    if (!down.empty()){

        for (int i = 0; i < down.size(); i++){

            if (down[i] == MEMORY_UNIT){
                capacity += Memory_unit.get_capacity();
            }
            
        }

    }

    if (!right.empty()){

        for (int i = 0; i < right.size(); i++){

            if (right[i] == MEMORY_UNIT){
                capacity += Memory_unit.get_capacity();
            }
            
        }

    }

    if (!left.empty()){

        for (int i = 0; i < left.size(); i++){

            if (left[i] == MEMORY_UNIT){
                capacity += Memory_unit.get_capacity();
            }
            
        }

    }
    
    set_DRAM_capacity(capacity);
    return;

}


void Die::update_SRAM_capacity(){

    float capacity = Compute_unit.get_capacity();
    set_SRAM_capacity(capacity);

}


void Die::update_memory_bandwidth(){

    float memory_bandwidth = 0;

    if (!up.empty()){

        for (int i = 0; i < up.size(); i++){

            if (up[i] == MEMORY_UNIT){
                memory_bandwidth += Memory_unit.get_bandwidth();
            }
            
        }

    }

    if (!down.empty()){

        for (int i = 0; i < down.size(); i++){

            if (down[i] == MEMORY_UNIT){
                memory_bandwidth += Memory_unit.get_bandwidth();
            }
            
        }

    }

    if (!right.empty()){

        for (int i = 0; i < right.size(); i++){

            if (right[i] == MEMORY_UNIT){
                memory_bandwidth += Memory_unit.get_bandwidth();
            }
            
        }

    }

    if (!left.empty()){

        for (int i = 0; i < left.size(); i++){

            if (left[i] == MEMORY_UNIT){
                memory_bandwidth += Memory_unit.get_bandwidth();
            }
            
        }

    }

    set_memory_bandwidth(memory_bandwidth);
    return;

}


void Die::update_communication_bandwidth(){

    float communication_bandwidth = 0;

    if (!up.empty()){

        for (int i = 0; i < up.size(); i++){

            if (up[i] == COMMUNICATION_UNIT){
                communication_bandwidth += Communication_unit.get_bandwidth();
            }
            
       }

    }

    if (!down.empty()){

        for (int i = 0; i < down.size(); i++){

            if (down[i] == COMMUNICATION_UNIT){
                communication_bandwidth += Communication_unit.get_bandwidth();
            }
            
        }

    }

    if (!right.empty()){

        for (int i = 0; i < right.size(); i++){

            if (right[i] == COMMUNICATION_UNIT){
                communication_bandwidth += Communication_unit.get_bandwidth();
            }
            
        }

    }

    if (!left.empty()){

        for (int i = 0; i < left.size(); i++){

            if (left[i] == COMMUNICATION_UNIT){
                communication_bandwidth += Communication_unit.get_bandwidth();
            }
            
        }

    }

    set_communication_bandwidth(communication_bandwidth);
    return;

}


void Die::update(){

    update_size(); 
    update_TFLOPS(); 
    update_DRAM_capacity(); 
    update_SRAM_capacity();
    update_memory_bandwidth(); 
    update_communication_bandwidth(); 
    return;

}


void Die::set_padding(float padding){

    this->padding = padding;
    update_size();
    return;

}


void Die::set_Compute_unit(Compute& new_Compute_unit){

    this->Compute_unit = new_Compute_unit;
    update_size();
    update_TFLOPS();
    update_SRAM_capacity();
    return;
    
}


void Die::set_Memory_unit(Memory& new_Memory_unit){

    this->Memory_unit = new_Memory_unit;
    update_size();
    update_DRAM_capacity();
    update_memory_bandwidth();
    return;

}


void Die::set_Communication_unit(Communication& new_Communication_unit){

    this->Communication_unit = new_Communication_unit;
    update_size();
    update_communication_bandwidth();
    return;

}


void Die::set_permutation_up(string& new_permutation){

    this->up = new_permutation;
    update();

}


void Die::set_permutation_down(string& new_permutation){

    this->down = new_permutation;
    update();
    
}


void Die::set_permutation_left(string& new_permutation){

    this->left = new_permutation;
    update();
    
}


void Die::set_permutation_right(string& new_permutation){

    this->right = new_permutation;
    update();
    
}


void Die::print(){

    cout << "up: " << up << endl;
    cout << "down" << down << endl;
    cout << "left" << left << endl;
    cout << "right" << right << endl;
    return;
    
}
