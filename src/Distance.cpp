#include <iostream>
#include <queue>
#include <cmath>

#include "Die.h"
#include "Distance.h"

using namespace std;

solution_distance calculate_distance(Die& first, Die& second, float wafer_length, float wafer_width){

    float first_length = first.get_size(0);
    float first_width = first.get_size(1);
    int first_count = max(floor(wafer_length / first_length) * floor(wafer_width / first_width), floor(wafer_width / first_length) * floor(wafer_length / first_width));
    float first_tflops = first.get_tflops() * first_count;
    float first_capacity = first.get_capacity() * first_count;
    float first_memory_bandwidth = first.get_memory_bandwidth() * first_count;
    float first_communication_bandwidth = first.get_communication_bandwidth() * first_count;

    float second_length = second.get_size(0);
    float second_width = second.get_size(1);
    int second_count = max(floor(wafer_length / second_length) * floor(wafer_width / second_width), floor(wafer_width / second_length) * floor(wafer_length / second_width));
    float second_tflops = second.get_tflops() * second_count;
    float second_capacity = second.get_capacity() * second_count;
    float second_memory_bandwidth = second.get_memory_bandwidth() * second_count;
    float second_communication_bandwidth = second.get_communication_bandwidth() * second_count;

    solution_distance result;
    result.tflops = first_tflops - second_tflops;
    result.capacity = first_tflops - second_tflops;
    result.memory_bandwidth = first_memory_bandwidth - second_memory_bandwidth;
    result.communication_bandwidth = first_communication_bandwidth - second_communication_bandwidth;

    return result;

}


void Possible_optimal(queue<Die> solutions, Die& optimal, float wafer_length, float wafer_width, simulation_error& error, queue<Die>& possible_optimals){
    
    while (!solutions.empty()){

        Die possible_solution = solutions.front();
        solutions.pop();
        solution_distance distance2optimal = calculate_distance(possible_solution, optimal, wafer_length, wafer_width);

        float tflops_max = (1 + error.tflops_positive) * optimal.get_tflops();
        float tflops_min = (1 + error.tflops_negative) * optimal.get_tflops();
        float capacity_max = (1 + error.capacity_positive) * optimal.get_capacity();
        float capacity_min = (1 + error.capacity_negative) * optimal.get_capacity();
        float memory_bandwidth_max = (1 + error.memory_bandwidth_positive) * optimal.get_memory_bandwidth();
        float memory_bandwidth_min = (1 + error.memory_bandwidth_negative) * optimal.get_memory_bandwidth();
        float communication_bandwidth_max = (1 + error.communication_bandwidth_positive) * optimal.get_communication_bandwidth();
        float communication_bandwidth_min = (1 + error.communication_bandwidth_negative) * optimal.get_communication_bandwidth();

        float tflops = possible_solution.get_tflops();
        float capacity = possible_solution.get_capacity();
        float memory_bandwidth = possible_solution.get_memory_bandwidth();
        float communication_bandwidth = possible_solution.get_communication_bandwidth();

        bool is_within_tflops = (tflops >= tflops_min) && (tflops <= tflops_max);
        bool is_within_capacity = (capacity >= capacity_min) && (capacity <= capacity_max);
        bool is_within_memory_bandwidth = (memory_bandwidth >= memory_bandwidth_min) && (memory_bandwidth <= memory_bandwidth_max);
        bool is_within_communication_bandwidth = (communication_bandwidth >= communication_bandwidth_min) && (communication_bandwidth <= communication_bandwidth_max);

        bool is_within_error = is_within_tflops || is_within_capacity || is_within_memory_bandwidth || is_within_communication_bandwidth;
        
        if(is_within_error){
            possible_optimals.push(possible_solution);
        }

    }

}
