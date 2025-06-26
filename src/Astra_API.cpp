#include <iostream>
#include <fstream>
#include <list>
#include <cmath>
#include <string>
#include <iomanip>
#include <sstream>

#include "Wafer.h"
#include "Die.h"

using namespace std;

int time2cycle(float time, float freq){

    int cycle = ceil(time * freq);
    return cycle;
    
}

void astra_API(float freq, float off_chip_bandwidth, float TFLOPs, float model_size, float kv_cache_size, float traffic, list<Wafer> solutions, string path_workload,  string path_logical_network, string path_physical_network, string config){

    int wafer_idx = 0;
    for (auto it = solutions.begin(); it != solutions.end(); it++){
        Wafer solution = *it;
        Die die = solution.get_die();
        int columns = solution.get_columns();
        int rows = solution.get_rows();
        float model_size_per_die = 2 * model_size / columns; // consider 1F1B schedule, store weight gradient also
        float kv_cache_size_per_die = kv_cache_size / columns;
        int model_parallel_num = rows;
        float TFLOPs_per_die = TFLOPs / columns;

        float die_TFLOPS = die.get_TFLOPS();
        float forward_compute_time = TFLOPs_per_die / die_TFLOPS;
        
        float die_SRAM_size = die.get_SRAM_capacity();
        float forward_die_DRAM_size = die.get_DRAM_capacity();
        float die_DRAM_bandwidth = die.get_memory_bandwidth();
        float forward_DRAM_access_size = 0, forward_off_chip_access_size = 0;

        if(die_SRAM_size < model_size_per_die){
            forward_DRAM_access_size += model_size_per_die - die_SRAM_size;
            forward_die_DRAM_size -= model_size_per_die - die_SRAM_size;
        }

        if(forward_die_DRAM_size >= kv_cache_size_per_die){
            
            forward_DRAM_access_size += kv_cache_size_per_die;
            
        } else {

            forward_DRAM_access_size += forward_die_DRAM_size;
            forward_off_chip_access_size += kv_cache_size_per_die - forward_die_DRAM_size;
        
        }

        float forward_DRAM_access_time = forward_DRAM_access_size / die_DRAM_bandwidth;
        float forward_off_chip_access_time = forward_off_chip_access_size / off_chip_bandwidth;
        float forward_access_time = max(forward_DRAM_access_time, forward_off_chip_access_time);


        int forward_cycle = time2cycle(max(forward_compute_time, forward_access_time));
        int forward_communication_size = int(traffic);
        int input_gradient_cycle = forward_cycle; // no kv_cache in backward process. backward process is like prefill
        int weight_gradient_cycle = forward_cycle;
        int weight_communication_size = ceil(model_size_per_die * (rows - 1));

        // Create filename with 6-digit wafer_idx
        stringstream ss;
        ss << config << "_" << setw(6) << setfill('0') << wafer_idx << "_workload.txt";
        string filename = ss.str();
        
        // Create full file path
        string filepath = path_workload + "/" + filename;
        
        // Create and write content to txt file
        ofstream outfile(filepath);
        if (outfile.is_open()) {
            // First line: DATA
            outfile << "DATA" << endl;
            
            // Second line: columns value
            outfile << columns << endl;
            
            // Write columns rows
            for (int i = 0; i < columns; i++) {
                outfile << "layer_" << i << " "
                        << "-1 "
                        << forward_cycle << " "
                        << "ALLREDUCE "
                        << forward_communication_size << " "
                        << input_gradient_cycle << " "
                        << "ALLREDUCE "
                        << "0 "
                        << weight_gradient_cycle << " "
                        << "ALLREDUCE "
                        << weight_communication_size << " "
                        << "10" << endl;
            }
            
            outfile.close();
        } else {
            cerr << "Error: Unable to create file " << filepath << endl;
        }
        
        wafer_idx++;
    }
        

        
        


  
}
