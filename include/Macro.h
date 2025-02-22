#ifndef MACRO_H
#define MACRO_H

#define MEMORY_UNIT '0'
#define COMMUNICATION_UNIT '1'

// the minimal performance metrics that the wafer must satisfy
struct Threshold{

    float tflops;
    float capacity;
    float memory_bandwidth;
    float communication_bandwidth;

};


// the configurations of the wafer
struct Config{

    float die_padding;
    float relaxation;
    float wafer_length;
    float wafer_width;
    Threshold threshold;
    
};


// the maximum possible error that the simulator can generate
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


// the distance between two solutions
struct solution_distance{
    float tflops;
    float capacity;
    float memory_bandwidth;
    float communication_bandwidth;
};

#endif