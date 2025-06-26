#ifndef ASTRA_API_H
#define ASTRA_API_H

#include <string>
#include <list>

#include "Wafer.h"

using namespace std;

int time2cycle(float time, float freq);
void astra_API(float freq, float off_chip_bandwidth, float TFLOPs, float model_size, float kv_cache_size, float traffic, list<Wafer> solutions, string path_workload,  string path_physical_network, string config);

#endif

