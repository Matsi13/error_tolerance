#ifndef ANALYSIS_H
#define ANALYSIS_H

#include <list>

#include "Macro.h"
#include "Workload.h"
#include "Wafer.h"

using namespace std;

void Possible_optimal_wafers(Workload workload, list<Wafer> &wafers, list<Wafer> &optimals, Workload_error error, float off_wafer_bandwidth);

#endif