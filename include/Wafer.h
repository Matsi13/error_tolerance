#ifndef WAFER_H
#define WAFER_H

#include "Die.h"

using namespace std;

class Wafer{
    public:
        Wafer(){}
        Wafer(float* sizes, Die& die);
        Wafer(const Wafer& obj);
        void print(); // print the solution for later visulization
        const float get_size(int idx)const;
        const float get_tflops()const;
        const float get_capacity()const;
        const float get_memory_bandwidth()const;
        const float get_communication_bandwidth()const;
        const Die get_die()const;
        void set_die(Die& newdie);
    
    private:
        float sizes[2];
        float tflops;
        float capacity;
        float memory_bandwidth;
        float communication_bandwidth;
        Die die;
        int rows;
        int columns;
        void update();

};

#endif