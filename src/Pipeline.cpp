
#include <stack>

#include "Workload.h"
#include "Macro.h"
#include "Pipeline.h"

using namespace std;

Workload pipeline_merge(stack<Workload> workloads){

    float TFLOPs = 0;
    float paramsize = 0;
    float access = 0;
    float traffic = 0;

    while(!workloads.empty()){

        Workload cur_workload = workloads.top();
        workloads.pop();
        TFLOPs += cur_workload.get_TFLOPs();
        paramsize += cur_workload.get_paramsize();
        access += cur_workload.get_access();
        traffic += cur_workload.get_traffic();

    }

    Workload result(TFLOPs, paramsize, access, traffic);
    return result;

}



