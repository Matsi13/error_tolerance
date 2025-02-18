#include<iostream>
#include<vector>
#include<list>
#include<queue>
#include<algorithm>
#include<string>
#include<cmath>

#include "Macro.h"
#include "Communication.h"
#include "Compute.h"
#include "Memory.h"
#include "Die.h"
#include "Wafer.h"
#include "Permutation.h"
#include "Distance.h"

using namespace std;

int main(){

    float compute_sizes[2] = {16, 12};
    float tflops = 10;
    float compute_padding = 0;
    Compute compute_unit(compute_sizes, tflops, compute_padding);

    float memory_sizes[2] = {5, 5};
    float capacity = 4;
    float memory_bandwidth = 10;
    float memory_padding = 0.1;
    Memory memory_unit(memory_sizes, memory_bandwidth, capacity, memory_padding);

    float communication_sizes[2] = {1, 2};
    float communication_bandwidth = 10;
    float communication_padding = 0.1;
    Communication communication_unit(communication_sizes, communication_bandwidth, communication_padding);

    float die_padding = 0.5;
    float relaxation = 0;
    float wafer_length = 120;
    float wafer_width = 120;
    Threshold threshold = {300, 200, 250, 400};

    list<Wafer> result;
    list<string> permutation_side;

    // Search_permutation(permutation_side, compute_sizes[0], memory_sizes[0], communication_sizes[0], memory_padding, relaxation);
    // for (auto idx = permutation_side.begin(); idx != permutation_side.end(); idx++){

    //         string current_solution = *idx;
    //         cout << current_solution << endl;
            
    //     }



    Permutation(compute_unit, memory_unit, communication_unit, die_padding, result, relaxation, wafer_length, wafer_width, threshold);

    for (auto idx = result.begin(); idx != result.end(); idx++){

            Wafer current_solution = *idx;
            current_solution.print();
            
        }

    cout << "solutions : " <<  result.size() << endl;

    return 0;
}

