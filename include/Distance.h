#ifndef DISTANCE_H
#define DISTANCE_H

#include <queue>
#include <cmath>

#include "Die.h"
#include "Wafer.h"
#include "Distance.h"

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

solution_distance calculate_distance(Wafer& first, Wafer& second);

void Possible_optimal(queue<Wafer> solutions, Wafer& optimal, simulation_error& error, queue<Wafer>& possible_optimals);

#endif