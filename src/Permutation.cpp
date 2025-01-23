#include <iostream>
#include <string>
#include <queue>
#include <list>
#include <vector>
#include <algorithm>
#include <cassert>
#include <cmath>

#include "Macro.h"
#include "Memory.h"
#include "Communication.h"
#include "Compute.h"
#include "Die.h"

using namespace std;

void Search_permutation(list<string>& permutation_side, float Compute_size, float Memory_size, float Communication_size, float component_padding, float relaxation){
    
    queue<string> kdtree;
    string origin = "";
    kdtree.push(origin);
    permutation_side.push_back(origin);
        while(!kdtree.empty()){
            string solution = kdtree.front();
            kdtree.pop();
            float current_size = 0;

            if(!solution.empty()){

                for (int i = 0; i < solution.size(); i++){

                    switch(solution[i]){
                    case MEMORY_UNIT : {
                        current_size += Memory_size;
                        break;
                    }
                    case COMMUNICATION_UNIT : {
                        current_size += Communication_size;
                        break;
                    }
                    default : break;
                    }

                }

                current_size += (solution.size() - 1) * component_padding;
            }

            float remain_size = Compute_size - current_size + relaxation;
            assert(remain_size >= 0);

            if (remain_size >= Memory_size + component_padding){
                string new_solution = solution;
                new_solution.push_back(MEMORY_UNIT);
                sort(new_solution.begin(), new_solution.end());
                kdtree.push(new_solution);
                permutation_side.push_back(new_solution);
            }

            if (remain_size >= Communication_size + component_padding){
                string new_solution = solution;
                new_solution.push_back(COMMUNICATION_UNIT);
                sort(new_solution.begin(), new_solution.end());
                kdtree.push(new_solution);
                permutation_side.push_back(new_solution);
            }

        }

    
    permutation_side.unique();

    return;
}


// return true is first is better than second, vice visa
bool is_better_die(Die& first, Die& second, float wafer_length, float wafer_width){

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

    bool is_better = (first_tflops >= second_tflops) && (first_capacity >= second_capacity) && (first_memory_bandwidth >= second_memory_bandwidth) && (first_communication_bandwidth >= second_communication_bandwidth);

    return is_better;

}


void Permutation(Compute& Compute_unit, Memory& Memory_unit, Communication& Communication_unit, float die_padding, list<Die>& result, float relaxation, float wafer_length, float wafer_width){

    // relaxation allows the length of memory + communication to slightly overflow the length of Compute chip
    float Compute_length = Compute_unit.get_size(0);
    float Compute_width = Compute_unit.get_size(1); 
    float Memory_length = Memory_unit.get_size(0);
    float Communication_length = Communication_unit.get_size(0);
    float component_padding = max(Memory_unit.get_padding(), Communication_unit.get_padding());

    list<string> permutation_length;
    Search_permutation(permutation_length, Compute_length, Memory_length, Communication_length, component_padding, relaxation);
    list<string> permutation_width;
    Search_permutation(permutation_width, Compute_width, Memory_length, Communication_length, component_padding, relaxation);

    queue<Die> all_solutions;

    for(auto idx_up = permutation_length.begin(); idx_up != permutation_length.end(); idx_up++){
        for (auto idx_down = permutation_length.begin(); idx_down != permutation_length.end(); idx_down++){
            for (auto idx_left = permutation_width.begin(); idx_left != permutation_width.end(); idx_left++){
                for (auto idx_right = permutation_width.begin(); idx_right != permutation_width.end(); idx_right++){

                    Die Die_instance(die_padding, Compute_unit, Memory_unit, Communication_unit, *idx_up, *idx_down, *idx_left, *idx_right);
                    all_solutions.push(Die_instance);

                }
            } 
            
        }
    }

    // clear the sub_optimal solutions
    result.push_back(all_solutions.front());
    all_solutions.pop();
    if(all_solutions.empty()) return;

    while (!all_solutions.empty()){

        Die new_solution = all_solutions.front();
        all_solutions.pop();
        bool is_better = false; // if this is true, new_solution should be added to the final result

        for (auto idx = result.begin(); idx != result.end(); idx++){

            Die current_solution = *idx;
            if (is_better_die(new_solution, current_solution, wafer_length, wafer_width)){
                // new_solution is better than the origin one
                is_better = true;
                result.erase(idx);
                idx--;
            } 
            
        }

        if(!is_better){

            for (auto idx = result.begin(); idx != result.end(); idx++){

            Die current_solution = *idx;
            if (is_better_die(current_solution, new_solution, wafer_length, wafer_width)){
                is_better = false; break;
            } 
            
        }

        }

        if(is_better){
            result.push_back(new_solution);
        }

    }

    return;
}