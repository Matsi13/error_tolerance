#include <iostream>
#include <ifstream>
#include <list>
#include <cmath>
#include <string>

#include "Wafer.h"
#include "Die.h"

using namespace std;

void astra_API(float freq, float off_chip_bandwidth, float TFLOPs, float model_size, float kv_cache_size, float traffic, list<Wafer> solutions, string path_workload, string path_arch, string path_system, string path_network, string path_logical_network, string path_physical_network, string config){

    for (auto it = solutions.begin(); it != solutions.end(); it++){
        Wafer solution = *it;
        Die die = solution.get_die();
        int columns = solution.get_columns();
        int rows = solution.get_rows();
        float model_size_per_die = model_size / columns;
        float kv_cache_size_per_die = kv_cache_size / columns;
        int model_parallel_num = rows;
        float TFLOPs_per_die = TFLOPs / columns;

        float die_TFLOPS = die.get_TFLOPS();
        float forward_compute_time = TFLOPs_per_die / die_TFLOPs;
        
        float die_SRAM_size = die.get_SRAM_capacity();
        float forward_die_DRAM_size = die.get_DRAM_capacity();
        float die_DRAM_bandwidth = die.get_DRAM_bandwidth();
        float forward_DRAM_access_size = 0, forward_off_chip_access_size = 0;

        if(die_SRAM_size < model_size_per_die）{
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

        float forward_time = max(forward_compute_time, forward_access_time);
        


  
}

