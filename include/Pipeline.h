#ifndef PIPELINE_H
#define PIPELINE_H

#include <stack>

#include "Workload.h"
#include "Macro.h"

using namespace std;

Workload pipeline_merge(stack<Workload> workloads);



#endif