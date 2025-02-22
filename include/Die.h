#ifndef DIE_H 
#define DIE_H

#include <string>
#include <algorithm>

#include "Compute.h"
#include "Communication.h"
#include "Memory.h"
#include "Macro.h"

using namespace std;

class Die{
    public: 
        Die(){};
        Die(float padding, Compute& Compute_unit, Memory& Memory_unit, Communication& Communication_unit, string& up, string& down, string& left, string& right);
        Die(const Die& obj);
        ~Die();

        const float get_size(int index)const;
        const float get_padding()const;
        const float get_tflops()const;
        const float get_capacity()const;
        const float get_memory_bandwidth()const;
        const float get_communication_bandwidth()const;
        const Compute get_Compute_unit()const;
        const Memory get_Memory_unit()const;
        const Communication get_Communication_unit()const;
        const string get_permutation_up()const;
        const string get_permutation_down()const;
        const string get_permutation_left()const;
        const string get_permutation_right()const;

        void set_padding(float padding);   
        void set_Compute_unit(Compute &new_Compute_unit); 
        void set_Memory_unit(Memory &new_Memory_unit);
        void set_Communication_unit(Communication &new_Communication_unit);   
        void set_permutation_up(string &new_permutation);
        void set_permutation_down(string &new_permutation);
        void set_permutation_left(string &new_permutation);
        void set_permutation_right(string &new_permutation);

        void print();

    private:
        float sizes[2];
        float padding;
        float tflops;
        float capacity;
        float memory_bandwidth;
        float communication_bandwidth;
        Compute Compute_unit;
        Memory Memory_unit;
        Communication Communication_unit;

        // unit permutation on four edges of the die is represented by a string
        string up; 
        string down; 
        string left;
        string right;

        void set_size(float size, int index);
        void set_tflops(float tflops);
        void set_capacity(float capacity);
        void set_memory_bandwidth(float memory_bandwidth);
        void set_communication_bandwidth(float communication_bandwidth);
        
        void update_size(); // update size based on all units
        void update_tflops(); // update tflops based on compute unit
        void update_capacity(); // update capacity based on memory units
        void update_memory_bandwidth(); // update memory bandwidth based on based on memory units
        void update_communication_bandwidth(); // update communication bandwidth based on communication units
        void update(); // update size, padding, tflops, capacity and bandwidth based on the new permutation


};

#endif