#ifndef WAFER_H
#define WAFER_H

#include "Die.h"

using namespace std;

class Wafer{
    public:
        Wafer(){};
        Wafer(float* sizes, Die& die);
        Wafer(const Wafer& obj);
        Wafer& operator = (const Wafer &obj);
        void print(); // print the solution for later visulization

        const float get_size(int idx)const; 
        const float get_TFLOPS()const;
        const float get_SRAM_capacity()const;
        const float get_DRAM_capacity()const;
        const float get_memory_bandwidth()const;
        const float get_communication_bandwidth()const;
        const Die get_die()const;

        void set_die(Die& newdie); // change the die in the wafer and update 
    
    private:
        float sizes[2];
        float TFLOPS;
        float SRAM_capacity;
        float DRAM_capacity;
        float memory_bandwidth;
        float communication_bandwidth;
        Die die;
        int rows; // rows of dies
        int columns; // columns of dies
        void update(); // calculate the performance metrics

};

#endif