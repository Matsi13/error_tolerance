#ifndef GENERATE_H
#define GENERATE_H

#include <string>
#include <list>

#include "Macro.h"
#include "Compute.h"
#include "Memory.h"
#include "Communication.h"

using namespace std;

// read configs from the input file, and return every compute, memory and communication config.  
void Generate(list<Compute>& compute_configs, list<Memory>& memory_configs, list<Communication>& communication_configs, string input_path, Config& input_config);

#endif