#ifndef MACRO_H
#define MACRO_H

#define MEMORY_UNIT '0'
#define COMMUNICATION_UNIT '1'

struct Threshold{

    float tflops;
    float capacity;
    float memory_bandwidth;
    float communication_bandwidth;

};

struct Config{

    float die_padding;
    float relaxation;
    float wafer_length;
    float wafer_width;
    Threshold threshold;
    
};

struct simulation_error{
    float tflops_positive;
    float tflops_negative;
    float capacity_positive;
    float capacity_negative;
    float memory_bandwidth_positive;
    float memory_bandwidth_negative;
    float communication_bandwidth_positive;
    float communication_bandwidth_negative;
};

struct solution_distance{
    float tflops;
    float capacity;
    float memory_bandwidth;
    float communication_bandwidth;
};

#endif