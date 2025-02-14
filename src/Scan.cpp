#include <iostream>
#include <fstream>
#include <list>
#include <string>

#include "Compute.h"
#include "Memory.h"
#include "Communication.h"
#include "Die.h"
#include "Permutation.h"

using namespace std;


// read the configs file and search for solutions
void Scan(){
    ifstream input("D:\\VS projects\\error_tolerance\\configuration\\component_configuration.txt", ios::in);
    string title;

    float compute_sizes[2];
    float tflops;
    float compute_padding;
    list<Compute> compute_configs;

    float memory_sizes[2];
    float capacity;
    float memory_bandwidth;
    float memory_padding;
    list<Memory> memory_configs;

    float communication_sizes[2];
    float communication_bandwidth;
    float communication_padding;
    list<Communication> communication_configs;

    float die_padding;
    float relaxation;
    float wafer_length;
    float wafer_width;
    int num;
 
    if (!input.is_open()){
        cout << "No existing configuration file!" << endl;
    }

    input >> title;
    input >> wafer_length;
    input >> title;
    input >> wafer_width;
    input >> title;
    input >> die_padding;
    input >> title;
    input >> relaxation;

    input >> title;
    input >> num;
    for (int i = 0; i < num; i++){
        input >> title;
        input >> compute_sizes[0] >> compute_sizes[1];
        input >> title;
        input >> tflops;
        input >> title;
        input >> compute_padding;
        Compute Compute_unit(compute_sizes, tflops, compute_padding);
        compute_configs.push_back(Compute_unit);
    }

    input >> title;
    input >> num;
    for (int i = 0; i < num; i++){
        input >> title;
        input >> memory_sizes[0] >> memory_sizes[1];
        input >> title;
        input >> capacity;
        input >> title;
        input >> memory_bandwidth;
        input >> title;
        input >> memory_padding;
        Memory Memory_unit(memory_sizes, memory_bandwidth, capacity, memory_padding);
    }

    input >> title;
    input >> num;
    for (int i = 0; i < num; i++){
        input >> title;
        input >> communication_sizes[0] >> communication_sizes[1];
        input >> title;
        input >> communication_bandwidth;
        input >> title;
        input >> communication_padding;
        Communication Communication_unit(communication_sizes, communication_bandwidth, communication_padding);
    }

    input.close();
    list<list<Die>> results_by_configs;
    list<Die> result;

    for (auto compute_idx = compute_configs.begin(); compute_idx != compute_configs.end(); compute_idx++){
        for (auto memory_idx = memory_configs.begin(); memory_idx != memory_configs.end(); memory_idx++){
            for (auto communication_idx = communication_configs.begin(); communication_idx != communication_configs.end(); communication_idx++){
                
                Permutation(*compute_idx, *memory_idx, *communication_idx, die_padding, result, relaxation, wafer_length, wafer_width);
                results_by_configs.push_back(result);
            }
        }
    }

    // ofstream output("D:\\VS projects\\error_tolerance\\result\\results.txt");
    // output.close();


    return;
}