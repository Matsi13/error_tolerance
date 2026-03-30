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

// HBM_bandwidth: off-wafer HBM bandwidth (GB/s), used when on-die memory (eDRAM/SRAM chiplets)
// cannot hold the full model weights or KV-cache. Spilled data is fetched from off-wafer HBM.
void astra_API(float freq, float HBM_bandwidth, float TFLOPs, float model_size, float kv_cache_size, float traffic, list<Wafer> solutions, string path_workload, string path_physical_network, string config){

    int wafer_idx = 0;
    for (auto it = solutions.begin(); it != solutions.end(); it++){
        Wafer solution = *it;
        Die die = solution.get_die();
        int columns = solution.get_columns();
        int rows = solution.get_rows();
        if (columns <= 0) {
            cerr << "Error: Invalid wafer (columns <= 0), skipping wafer " << wafer_idx << endl;
            wafer_idx++;
            continue;
        }
        float model_size_per_die = 2 * model_size / columns; // consider 1F1B schedule, store weight gradient also
        float kv_cache_size_per_die = kv_cache_size / columns;
        int model_parallel_num = rows;
        float TFLOPs_per_die = TFLOPs / columns;

        float die_TFLOPS = die.get_TFLOPS();
        float forward_compute_time = TFLOPs_per_die / die_TFLOPS;
        
        float die_SRAM_size = die.get_SRAM_capacity();          // on-core SRAM
        float forward_on_die_mem_size = die.get_on_die_mem_capacity(); // on-die eDRAM/SRAM chiplets
        float die_mem_bandwidth = die.get_memory_bandwidth();     // bandwidth to on-die memory chiplets
        float die_communication_bandwidth = die.get_communication_bandwidth();
        float forward_on_die_access_size = 0, forward_HBM_access_size = 0; // HBM = off-wafer spill

        if (die_TFLOPS <= 0.0f || die_mem_bandwidth <= 0.0f || HBM_bandwidth <= 0.0f) {
            cerr << "Error: Invalid wafer parameters (die_TFLOPS, die_mem_bandwidth, HBM_bandwidth must be positive), skipping wafer " << wafer_idx << endl;
            wafer_idx++;
            continue;
        }

        // Memory access: model weights first fill SRAM, overflow goes to on-die eDRAM/SRAM chiplets
        if(die_SRAM_size < model_size_per_die){
            forward_on_die_access_size += model_size_per_die - die_SRAM_size;
            forward_on_die_mem_size -= model_size_per_die - die_SRAM_size;
        }

        // KV-cache: fill remaining on-die memory, overflow spills to off-wafer HBM
        if(forward_on_die_mem_size >= kv_cache_size_per_die){
            
            forward_on_die_access_size += kv_cache_size_per_die;
            
        } else {

            forward_on_die_access_size += forward_on_die_mem_size;
            forward_HBM_access_size += kv_cache_size_per_die - forward_on_die_mem_size;
        
        }

        float forward_on_die_access_time = forward_on_die_access_size / die_mem_bandwidth;
        float forward_HBM_access_time = forward_HBM_access_size / HBM_bandwidth; // off-wafer HBM spill
        float forward_access_time = max(forward_on_die_access_time, forward_HBM_access_time);


        int forward_cycle = time2cycle(max(forward_compute_time, forward_access_time) / columns, freq);
        int forward_communication_size = int(traffic / columns);
        int input_gradient_cycle = forward_cycle; // no kv_cache in backward process. backward process is like prefill
        int weight_gradient_cycle = forward_cycle;
        int weight_communication_size = ceil(model_size_per_die * (rows - 1) / columns);

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
                        << "NONE "
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

        stringstream ns3;
        ns3 << config << "_" << setw(6) << setfill('0') << wafer_idx << "_physical_network.txt";
        string filename_ns3 = ns3.str();
        
        // Create full file path
        string filepath_ns3 = path_physical_network + "/" + filename_ns3;
        
        // Create and write content to txt file
        ofstream outfile_ns3(filepath_ns3);
        int num_nodes = rows * columns;
        int num_links = 2 * rows * columns;
        float die_one_direction_bandwidth = die_communication_bandwidth / 4 * 8; // Gbps
        if (outfile_ns3.is_open()) {
            // First line: DATA
            outfile_ns3 << num_nodes << " "
                        << 0 << " " 
                        << num_links << " "
                        << endl << endl;
           
            // Write links
            // for (int i = 0; i < rows; i++) {

            //     for (int j = 0; j < columns; j++){

            //         if (i < rows - 1){

            //             int start = i * columns + j;
            //             int end = (i + 1) * columns + j;

            //             outfile_ns3 << start << " "
            //                         << end << " "
            //                         << die_one_direction_bandwidth << "Gbps " 
            //                         << 0.001 << "ms " 
            //                         << 0 << endl;

            //         }

            //         if (j < columns - 1){

            //             int start = i * columns + j;
            //             int end = i * columns + j + 1;

            //             outfile_ns3 << start << " "
            //                         << end << " "
            //                         << die_one_direction_bandwidth << "Gbps " 
            //                         << 0.001 << "ms " 
            //                         << 0 << endl;

            //         }

            //     }
                
            // }

            for (int i = 0; i < rows; i++) {

                for (int j = 0; j < columns; j++){

                    int start = i * columns + j;
                    int end1 = ((i + 1)%rows) * columns + j;

                    outfile_ns3 << start << " "
                                << end1 << " "
                                << die_one_direction_bandwidth << "Gbps " 
                                << 0.001 << "ms " 
                                << 0 << endl;
    
                    int end2 = i * columns + ((j + 1)%columns);

                    outfile_ns3 << start << " "
                                << end2 << " "
                                << die_one_direction_bandwidth << "Gbps " 
                                << 0.001 << "ms " 
                                << 0 << endl;

                }

            }
            outfile_ns3.close();
        } else {
            cerr << "Error: Unable to create file " << filepath_ns3 << endl;
        }
        
        wafer_idx++;
}
}
