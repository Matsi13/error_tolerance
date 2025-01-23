#ifndef DISTANCE_H
#define DISTANCE_H

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