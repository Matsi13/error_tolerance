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

#define TFLOPS_ERR 0
#define PARAM_ERR 1
#define ACCESS_ERR 2
#define TRAFF_ERR 3

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


struct Exec_time_range{

    float time_min;
    float time_standard;
    float time_max;


};

struct Workload_error{
    float TFLOPs_positive;
    float TFLOPs_negative;
    float paramsize_positive;
    float paramsize_negative;
    float access_positive;
    float access_negative;
    float traffic_positive;
    float traffic_negative;
};


struct Arch_error{

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

#endif