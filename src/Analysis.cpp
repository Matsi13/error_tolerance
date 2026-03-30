#include <list>
#include <stack>
#include <utility>
#include <cmath>

#include "Macro.h"
#include "Workload.h"
#include "Wafer.h"


using namespace std;

void Possible_optimal_wafers(Workload workload, list<Wafer> &wafers, list<Wafer> &optimals, Workload_error error, float HBM_bandwidth){
    // HBM_bandwidth: off-wafer HBM bandwidth (GB/s), used when on-die memory capacity
    // is insufficient to hold the full model/KV-cache working set.

    float TFLOPs = workload.get_TFLOPs();
    float paramsize = workload.get_paramsize();
    float access = workload.get_access();
    float traffic = workload.get_traffic();

    float paramsize_min = (1 + error.paramsize_negative) * paramsize;
    float paramsize_max = (1 + error.paramsize_positive) * paramsize;
    float access_min = (1 + error.access_negative) * access;  
    float access_max = (1 + error.access_positive) * access;
    float TFLOPs_min = (1 + error.TFLOPs_negative) * TFLOPs;
    float TFLOPs_max = (1 + error.TFLOPs_positive) * TFLOPs;

    stack<pair<Wafer, Exec_time_range>> solutions;
    float exec_time_opt = numeric_limits<float>::infinity();
    float exec_time_opt_max = numeric_limits<float>::infinity();
    Wafer opt_wafer;

    for(auto it = wafers.begin(); it != wafers.end(); it++){

        Exec_time_range exec_time_range;
        Wafer wafer = *it;
        float TFLOPS = wafer.get_TFLOPS();
        float on_die_mem_capacity = wafer.get_on_die_mem_capacity();
        float memory_bandwidth = wafer.get_memory_bandwidth();
        float communication_bandwidth = wafer.get_communication_bandwidth();

        float compute_time_standard = TFLOPs/TFLOPS;
        float access_time_standard = ( (on_die_mem_capacity >= paramsize) ? (access / memory_bandwidth) : (access * on_die_mem_capacity / paramsize / memory_bandwidth + access * (1 - on_die_mem_capacity / paramsize) / HBM_bandwidth) ); 
        float communication_time_standard = traffic / communication_bandwidth;
        exec_time_range.time_standard = max(max(compute_time_standard, access_time_standard), communication_time_standard);

        float compute_time_min = TFLOPs_min / TFLOPS;
        
        float access_time_min = ((on_die_mem_capacity >= paramsize_min) ? (access_min / memory_bandwidth) : (access_min * on_die_mem_capacity / paramsize_min / memory_bandwidth + access_min * (1 - on_die_mem_capacity / paramsize_min) / HBM_bandwidth) ); 
        float communication_time_min = traffic * (1 + error.traffic_negative) / communication_bandwidth;
        exec_time_range.time_min = max(max(compute_time_min, access_time_min), communication_time_min);

        float compute_time_max = TFLOPs_max / TFLOPS;
        float access_time_max = ((on_die_mem_capacity >= paramsize_max) ? (access_max / memory_bandwidth) : (access_max * on_die_mem_capacity / paramsize_max / memory_bandwidth + access_max * (1 - on_die_mem_capacity / paramsize_max) / HBM_bandwidth) ); 
        float communication_time_max = traffic * (1 + error.traffic_positive) / communication_bandwidth;
        exec_time_range.time_max = max(max(compute_time_max, access_time_max), communication_time_max);

        if(exec_time_range.time_standard < exec_time_opt) {

            exec_time_opt = exec_time_range.time_standard;
            exec_time_opt_max = exec_time_range.time_max;
            opt_wafer = wafer;

        } else if ((exec_time_range.time_standard == exec_time_opt) && (exec_time_range.time_max < exec_time_opt_max)){

            exec_time_opt_max = exec_time_range.time_max;
            opt_wafer = wafer;

        }

        solutions.push(make_pair(wafer, exec_time_range));
        
    }

    while (!solutions.empty()){

        pair<Wafer, Exec_time_range> solution = solutions.top();
        solutions.pop();

        Exec_time_range cur_time_range = solution.second;
        float cur_time_min = cur_time_range.time_min;
        if(cur_time_min <= exec_time_opt_max){

            Wafer possible = solution.first;
            optimals.push_back(possible);

        }

    }
    
    return;

}
