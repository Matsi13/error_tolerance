#ifndef ANALYSIS_H
#define ANALYSIS_H

#include <list>

#include "Macro.h"
#include "Workload.h"
#include "Wafer.h"

using namespace std;

// Finds wafers that could be optimal given workload error bounds.
// HBM_bandwidth: off-wafer HBM bandwidth (GB/s), used when on-die memory
//   capacity is insufficient to hold the full working set.
void Possible_optimal_wafers(Workload workload, list<Wafer> &wafers, list<Wafer> &optimals, Workload_error error, float HBM_bandwidth);

#endif
