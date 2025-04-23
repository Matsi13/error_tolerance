#ifndef MACRO_H
#define MACRO_H

#define MEMORY_UNIT '0'
#define COMMUNICATION_UNIT '1'

#define LAYERNORM 0
#define RMSNORM 1

#define INFERENCE 0
#define TRAINING 1

#define SOFTMAX 0
#define RELU 1
#define TANH 2

// the minimal performance metrics that the wafer must satisfy
struct Threshold{

    float TFLOPS;
    float SRAM_capacity;
    float DRAM_capacity;
    float memory_bandwidth;
    float communication_bandwidth;

};


// the configurations of the wafer
struct Config{

    float die_padding;
    float bandwidth_per_area;
    float memory_bandwidth_ratio;
    float relaxation;
    float wafer_length;
    float wafer_width;
    Threshold threshold;
    
};


// the maximum possible error that the simulator can generate
struct simulation_error{
    float TFLOPS_positive;
    float TFLOPS_negative;
    float SRAM_capacity_positive;
    float SRAM_capacity_negative;
    float DRAM_capacity_positive;
    float DRAM_capacity_negative;
    float memory_bandwidth_positive;
    float memory_bandwidth_negative;
    float communication_bandwidth_positive;
    float communication_bandwidth_negative;
};


// the distance between two solutions
struct solution_distance{
    float TFLOPS;
    float SRAM_capacity;
    float DRAM_capacity;
    float memory_bandwidth;
    float communication_bandwidth;
};

#endif