#ifndef RECOMPUTE_H
#define RECOMPUTE_H

#include "Workload.h"

using namespace std;

Workload recompute_memory(Workload ori_workload, float input_size, float distance2end, bool is_checkpoint);

#endif