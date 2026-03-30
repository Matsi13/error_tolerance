#ifndef WAFER_H
#define WAFER_H

#include "Die.h"

using namespace std;

// Memory hierarchy modelled inside Wafer (aggregated over all dies):
//   SRAM_capacity       - total on-core SRAM across all dies
//   on_die_mem_capacity - total on-die eDRAM/SRAM chiplet capacity across all dies
//   HBM                 - off-wafer; NOT modelled here

class Wafer{
    public:
        Wafer(){};
        Wafer(float* sizes, Die& die);
        Wafer(const Wafer& obj);
        Wafer& operator = (const Wafer &obj);
        void print(); // print the solution for later visualisation

        const float get_size(int idx)const; 
        const float get_TFLOPS()const;
        const float get_SRAM_capacity()const;         // total on-core SRAM
        const float get_on_die_mem_capacity()const;   // total on-die eDRAM/SRAM chiplet capacity
        const float get_memory_bandwidth()const;
        const float get_communication_bandwidth()const;
        const int get_rows()const;
        const int get_columns()const;
        const Die get_die()const;

        void set_die(Die& newdie); // change the die in the wafer and update 
    
    private:
        float sizes[2];
        float TFLOPS;
        float SRAM_capacity;        // total on-core SRAM across all dies
        float on_die_mem_capacity;  // total on-die eDRAM/SRAM chiplet capacity across all dies
        float memory_bandwidth;
        float communication_bandwidth;
        Die die;
        int rows; // rows of dies
        int columns; // columns of dies
        void update(); // calculate the performance metrics

};

#endif
