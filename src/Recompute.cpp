#include "Workload.h"
#include "Recompute.h"

using namespace std;

Workload recompute_memory(Workload ori_workload, float input_size, float distance2end, bool is_checkpoint){
    
    float TFLOPs = ori_workload.get_TFLOPs();
    float paramsize = ori_workload.get_paramsize();
    float access = ori_workload.get_access();
    float traffic = ori_workload.get_traffic();

    if (is_checkpoint){
        paramsize += input_size * distance2end;
    }

    return Workload(TFLOPs, paramsize, access, traffic);

}
