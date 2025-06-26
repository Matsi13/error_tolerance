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

void astra_API(float freq, float off_chip_bandwidth, float TFLOPs, float model_size, float kv_cache_size, float traffic, list<Wafer> solutions, string path_workload, string path_physical_network, string config){

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
        float die_communication_bandwidth = die.get_communication_bandwidth();
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


        int forward_cycle = time2cycle(max(forward_compute_time, forward_access_time), freq);
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
        int num_links = 2 * rows * columns - rows - columns;
        float die_one_direction_bandwidth = die_communication_bandwidth / 4 * 8; // Gbps
        if (outfile_ns3.is_open()) {
            // First line: DATA
            outfile_ns3 << num_nodes << " "
                        << 0 << " " 
                        << num_links << " "
                        << endl << endl;
           
            // Write links
            for (int i = 0; i < rows; i++) {

                for (int j = 0; j < rows; j++){

                    if (i < rows - 1){

                        int start = i * rows + j;
                        int end = i * (rows + 1) + j;

                        outfile_ns3 << start << " "
                                    << end << " "
                                    << die_one_direction_bandwidth << "Gbps " 
                                    << 0.001 << "ms " 
                                    << 0 << endl;

                    }

                    if (j < columns - 1){

                        int start = i * rows + j;
                        int end = i * rows + j + 1;

                        outfile_ns3 << start << " "
                                    << end << " "
                                    << die_one_direction_bandwidth << "Gbps " 
                                    << 0.001 << "ms " 
                                    << 0 << endl;

                    }

                }
                
            }
            
            outfile_ns3.close();
        } else {
            cerr << "Error: Unable to create file " << filepath << endl;
        }
        
        wafer_idx++;
    }
  
}
