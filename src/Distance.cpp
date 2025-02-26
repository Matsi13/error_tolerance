#include <iostream>
#include <queue>
#include <cmath>

#include "Macro.h"
#include "Die.h"
#include "Distance.h"

using namespace std;

solution_distance calculate_distance(Wafer& first, Wafer& second){

    float first_TFLOPS = first.get_TFLOPS();
    float first_capacity = first.get_capacity();
    float first_memory_bandwidth = first.get_memory_bandwidth();
    float first_communication_bandwidth = first.get_communication_bandwidth();

    float second_TFLOPS = second.get_TFLOPS();
    float second_capacity = second.get_capacity();
    float second_memory_bandwidth = second.get_memory_bandwidth();
    float second_communication_bandwidth = second.get_communication_bandwidth();

    solution_distance result;
    result.TFLOPS = first_TFLOPS - second_TFLOPS;
    result.capacity = first_TFLOPS - second_TFLOPS;
    result.memory_bandwidth = first_memory_bandwidth - second_memory_bandwidth;
    result.communication_bandwidth = first_communication_bandwidth - second_communication_bandwidth;

    return result;

}


void Possible_optimal(queue<Wafer> solutions, Wafer& optimal, simulation_error& error, queue<Wafer>& possible_optimals){
    
    while (!solutions.empty()){

        Wafer possible_solution = solutions.front();
        solutions.pop();
        solution_distance distance2optimal = calculate_distance(possible_solution, optimal);

        float TFLOPS_max = (1 + error.TFLOPS_positive) * optimal.get_TFLOPS();
        float TFLOPS_min = (1 + error.TFLOPS_negative) * optimal.get_TFLOPS();
        float capacity_max = (1 + error.capacity_positive) * optimal.get_capacity();
        float capacity_min = (1 + error.capacity_negative) * optimal.get_capacity();
        float memory_bandwidth_max = (1 + error.memory_bandwidth_positive) * optimal.get_memory_bandwidth();
        float memory_bandwidth_min = (1 + error.memory_bandwidth_negative) * optimal.get_memory_bandwidth();
        float communication_bandwidth_max = (1 + error.communication_bandwidth_positive) * optimal.get_communication_bandwidth();
        float communication_bandwidth_min = (1 + error.communication_bandwidth_negative) * optimal.get_communication_bandwidth();

        float TFLOPS = possible_solution.get_TFLOPS();
        float capacity = possible_solution.get_capacity();
        float memory_bandwidth = possible_solution.get_memory_bandwidth();
        float communication_bandwidth = possible_solution.get_communication_bandwidth();

        bool is_within_TFLOPS = (TFLOPS >= TFLOPS_min) && (TFLOPS <= TFLOPS_max);
        bool is_within_capacity = (capacity >= capacity_min) && (capacity <= capacity_max);
        bool is_within_memory_bandwidth = (memory_bandwidth >= memory_bandwidth_min) && (memory_bandwidth <= memory_bandwidth_max);
        bool is_within_communication_bandwidth = (communication_bandwidth >= communication_bandwidth_min) && (communication_bandwidth <= communication_bandwidth_max);

        bool is_within_error = is_within_TFLOPS || is_within_capacity || is_within_memory_bandwidth || is_within_communication_bandwidth;
        
        if(is_within_error){
            possible_optimals.push(possible_solution);
        }

    }

}
