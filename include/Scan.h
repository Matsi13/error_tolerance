#ifndef SCAN_H
#define SCAN_H

#include <list>
#include <string>

#include "Macro.h"
#include "Compute.h"
#include "Memory.h"
#include "Communication.h"

using namespace std;


void Scan(list<Compute>& compute_configs, list<Memory>& memory_configs, list<Communication>& communication_configs, string input_path, Config& input_config);

#endif