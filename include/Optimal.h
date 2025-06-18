#ifndef OPTIMAL_H
#define OPTIMAL_H

#include <string>

#include "Wafer.h"

using namespace std;

// read configs from the input file, and return every compute, memory and communication config.  
Wafer Get_optimal(string input_path);

#endif