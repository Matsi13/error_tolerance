#include <iostream>
#include <string>
#include <queue>
#include <list>
#include <vector>
#include <algorithm>
#include <cassert>
#include <cmath>
#include <stack>

#include "Macro.h"
#include "Memory.h"
#include "Communication.h"
#include "Compute.h"
#include "Die.h"
#include "Wafer.h"
#include "Permutation.h"

using namespace std;

// return ture if first is better than second
bool is_better_permutation(string first, string second, Memory& Memory_unit, Communication& Communication_unit, float Compute_size){

    float first_height = 0;
    float first_length = 0;
    float first_capacity = 0;
    float first_memory_bandwidth = 0;
    float first_communication_bandwidth = 0;

    float second_height = 0;
    float second_length = 0;
    float second_capacity = 0;
    float second_memory_bandwidth = 0;
    float second_communication_bandwidth = 0;

    float component_padding = max(Memory_unit.get_padding(), Communication_unit.get_padding());

    if (!first.empty()){

        for(int i = 0; i < first.size(); i++){

            switch(first[i]){
                case MEMORY_UNIT : {

                    first_capacity += Memory_unit.get_capacity();
                    first_memory_bandwidth += Memory_unit.get_bandwidth();
                    first_height = max(first_height, Memory_unit.get_size(1));
                    first_length += Memory_unit.get_size(0);
                    break;

                }
                case COMMUNICATION_UNIT : {
    
                    first_communication_bandwidth += Communication_unit.get_bandwidth();
                    first_height = max(first_height, Communication_unit.get_size(1));
                    first_length += Communication_unit.get_size(0);  
                    break;

                }
                default : break;
                }
        }
    
        first_length += component_padding * (first.size() - 1);

    }

    first_length = max(first_length, Compute_size);

    if(!second.empty()){

        for(int i = 0; i < second.size(); i++){

            switch(second[i]){
                case MEMORY_UNIT : {
                    second_capacity += Memory_unit.get_capacity();
                    second_memory_bandwidth += Memory_unit.get_bandwidth();
                    second_height = max(first_height, Memory_unit.get_size(1));
                    second_length += Memory_unit.get_size(0);
                    break;
                }
                case COMMUNICATION_UNIT : {
    
                    second_communication_bandwidth += Communication_unit.get_bandwidth();
                    second_height = max(first_height, Communication_unit.get_size(1));
                    second_length += Communication_unit.get_size(0);
    
                    break;
                }
                default : break;
                }
        }
    
        second_length += component_padding * (second.size() - 1);

    }

    second_length = max(second_length, Compute_size);
    
    bool is_better = (first_height <= second_height) && (first_length <= second_length) && (first_capacity >= second_capacity) && (first_memory_bandwidth >= second_memory_bandwidth) && (first_communication_bandwidth >= second_communication_bandwidth);

    return is_better;


}

void Search_permutation(list<string>& permutation_side, float Compute_size, Memory& Memory_unit, Communication& Communication_unit, float relaxation){

    float Memory_size = Memory_unit.get_size(0);
    float Communication_size = Communication_unit.get_size(0);
    float component_padding = max(Memory_unit.get_padding(), Communication_unit.get_padding());
    
    stack<string> kdtree;  // search possible solutions in BFS pattern
    list<string> all_permutations;
    string origin = "";    // initial solution : put nothing on this edge
    kdtree.push(origin);   // save this in kdtree as the starting point
    all_permutations.push_back(origin); // also, this is a feasible solution, save it to permutation_side

        while(!kdtree.empty()){  // when kdtree is empty, there won't be any new solutions, the search can be terminated
            string solution = kdtree.top(); // get a solution, and try to find new solutions based on it
            kdtree.pop();        // discard the original solution from kdtree, since it is already used up
            float current_size = 0; // the curmulative length of the components on this edge

            if(!solution.empty()){ // if this solution is not an empty string, calculate the curmulative length of the components on this edge

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

                current_size += (solution.size() - 1) * component_padding; // add the padding length between each two components
            }

            float remain_size = Compute_size - current_size + relaxation; // the remaining available length for new components
            assert(remain_size >= 0);

            if (remain_size >= Memory_size + component_padding){  // whether a memory unit can be added
                string new_solution = solution;
                new_solution.push_back(MEMORY_UNIT);
                // order the string to avoid redundancy. For instance, 1010 and 0101 are the same after using sort function
                sort(new_solution.begin(), new_solution.end());                
                kdtree.push(new_solution); // add new solution to kdtree for future search
                all_permutations.push_back(new_solution); // add this solution as a feasible solution
            }

            if (remain_size >= Communication_size + component_padding){ // whether a communication unit can be added
                string new_solution = solution;
                new_solution.push_back(COMMUNICATION_UNIT);
                sort(new_solution.begin(), new_solution.end());
                kdtree.push(new_solution);
                all_permutations.push_back(new_solution);
            }

        }

    
    all_permutations.unique();  // clear the redundant solutions
    // put the first permutation into List premutation_side
    permutation_side.push_back(all_permutations.front());
    // clear this solution in List all_permutations
    all_permutations.pop_front();
    // if there is only one solution, it's optimal. return.
    if (all_permutations.empty()) return;
    // mark the position of the worse solutions that should be removed later
    stack<typename list<string>::iterator> indexes;

    while(!all_permutations.empty()){

        string new_permutation = all_permutations.front();
        all_permutations.pop_front();
        bool is_better = false;
        bool is_not_worse = true;

        for(typename list<string>::iterator idx = permutation_side.begin(); idx != permutation_side.end(); idx++){

            string current_permutation = *idx;
            if (is_better_permutation(new_permutation, current_permutation, Memory_unit, Communication_unit, Compute_size)){
                
                is_better = true;
                indexes.push(idx);

            }
        }

        if(is_better){

            typename list<string>::iterator idx;

            while(!indexes.empty()){

                idx = indexes.top();
                indexes.pop();
                permutation_side.erase(idx);

            }

            permutation_side.push_back(new_permutation);
 
        } else{

            for(auto idx = permutation_side.begin(); idx != permutation_side.end(); idx++){

                string current_permutation = *idx;
                if(is_better_permutation(current_permutation, new_permutation, Memory_unit, Communication_unit, Compute_size)){
                    is_not_worse = false; break;
                }

            }

            if(is_not_worse){
                permutation_side.push_back(new_permutation);
            }
        }
    }

    // cout << "num of permutations : " << permutation_side.size();
    // for(auto idx = permutation_side.begin(); idx != permutation_side.end(); idx++){

    //     cout << *idx << endl;

    // }

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

// return true is first is better than second, vice visa
bool is_better_wafer(Wafer& first, Wafer& second){

    
    float first_tflops = first.get_tflops();
    float first_capacity = first.get_capacity();
    float first_memory_bandwidth = first.get_memory_bandwidth();
    float first_communication_bandwidth = first.get_communication_bandwidth();

    float second_tflops = second.get_tflops();
    float second_capacity = second.get_capacity();
    float second_memory_bandwidth = second.get_memory_bandwidth();
    float second_communication_bandwidth = second.get_communication_bandwidth();

    bool is_better = (first_tflops >= second_tflops) && (first_capacity >= second_capacity) && (first_memory_bandwidth >= second_memory_bandwidth) && (first_communication_bandwidth >= second_communication_bandwidth);

    return is_better;

}

bool is_over_threshold(Wafer solution, Threshold threshold){

    bool flag = (solution.get_tflops() >= threshold.tflops) && (solution.get_capacity() >= threshold.capacity) && (solution.get_memory_bandwidth() >= threshold.memory_bandwidth) && (solution.get_communication_bandwidth() >= threshold.communication_bandwidth);
    return flag;

}


void Permutation(Compute& Compute_unit, Memory& Memory_unit, Communication& Communication_unit, float die_padding, list<Wafer>& result, float relaxation, float wafer_length, float wafer_width, Threshold threshold){

    // relaxation allows the length of memory + communication to slightly overflow the length of Compute chip
    float Compute_length = Compute_unit.get_size(0);
    float Compute_width = Compute_unit.get_size(1); 
    float Memory_length = Memory_unit.get_size(0);
    float Communication_length = Communication_unit.get_size(0);
    float component_padding = max(Memory_unit.get_padding(), Communication_unit.get_padding());
    float wafer_sizes[2] = {wafer_length, wafer_width};

    list<string> permutation_length;
    Search_permutation(permutation_length, Compute_length, Memory_unit, Communication_unit, relaxation);
    list<string> permutation_width;
    Search_permutation(permutation_width, Compute_width, Memory_unit, Communication_unit, relaxation);

    stack<Wafer> all_solutions;

    // list all solutions based on feasible permutations

    for(auto idx_up = permutation_length.begin(); idx_up != permutation_length.end(); idx_up++){
        for (auto idx_down = permutation_length.begin(); idx_down != permutation_length.end(); idx_down++){
            for (auto idx_left = permutation_width.begin(); idx_left != permutation_width.end(); idx_left++){
                for (auto idx_right = permutation_width.begin(); idx_right != permutation_width.end(); idx_right++){

                    Die Die_instance(die_padding, Compute_unit, Memory_unit, Communication_unit, *idx_up, *idx_down, *idx_left, *idx_right);
                    Wafer Wafer_instance(wafer_sizes, Die_instance);
                    if (is_over_threshold(Wafer_instance, threshold)){
                        all_solutions.push(Wafer_instance);
                    }
                    

                }
            } 
            
        }
    }

    cout << all_solutions.size() << endl;

    // put the first solution into List result
    result.push_back(all_solutions.top());
    // remove this solution from all_solutions
    all_solutions.pop();
    // if there is only one solution, it's optimal
    if(all_solutions.empty()) return;
    // mark the position of the worse solutions that should be removed later
    stack<typename list<Wafer>::iterator> indexes;

    while (!all_solutions.empty()){

        Wafer new_solution = all_solutions.top(); // get another solution from all_solutions
        all_solutions.pop(); // remove it from all_solutions
        bool is_better = false; // if this is true, new_solution should be added to the final result
        bool is_not_worse = true; // if this is true, new_solution should be added to the final result

        for (typename list<Wafer>::iterator idx = result.begin(); idx != result.end(); idx++){

            Wafer current_solution = *idx;
            if (is_better_wafer(new_solution, current_solution)){
                // if new_solution is better than one of the existing solutions in List result, remove the latter
                is_better = true; // new_solution will be added to List result later
                indexes.push(idx); // save the position of the worse solution for later removal
            } 
            
        }

        if(is_better){

            typename list<Wafer>::iterator idx;

            while(!indexes.empty()){

                idx = indexes.top();
                indexes.pop();
                result.erase(idx);

            }

            result.push_back(new_solution);

        } else{

            for (auto idx = result.begin(); idx != result.end(); idx++){

                Wafer current_solution = *idx;
                if (is_better_wafer(current_solution, new_solution)){
                    is_not_worse = false; break;
                } 
            }

            if (is_not_worse){
                result.push_back(new_solution);
            }

        }

    }

    return;
}