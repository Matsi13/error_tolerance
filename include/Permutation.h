#ifndef PERMUTATION_H
#define PERMUTATION_H

#include <string>
#include <queue>
#include <list>
#include <stack>

#include "Macro.h"
#include "Memory.h"
#include "Communication.h"
#include "Compute.h"
#include "Die.h"
#include "Wafer.h"

using namespace std;


// compare two permutations along the side of the compute chip
bool is_better_permutation(string first, string second, Memory& Memory_unit, Communication& Communication_unit, float Compute_size);

// search the possible permutations of memory and communication along the side of the compute chip. Relaxation allows the total length of components to be slightly over the length of compute chip
void Search_permutation(list<string>& permutation_side, float Compute_size, Memory& Memory_unit, Communication& Communication_unit, float relaxation);

// check whether the wafer meets the minimal performance metric
bool is_over_threshold(Wafer solution, Threshold threshold);

// compare two dies based on the wafer setting
bool is_better_die(Die& first, Die& second, float wafer_length, float wafer_width);

// compare two wafers
bool is_better_wafer(Wafer& first, Wafer& second);

void Prune(stack<Wafer>& all_solutions, list<Wafer>& result);

// find the optimal wafers that meets the threshold
void Permutation(Compute& Compute_unit, Memory& Memory_unit, Communication& Communication_unit, float die_padding, float bandwidth_per_area, float memory_bandwidth_ratio, list<Wafer>& result, float relaxation, float wafer_length, float wafer_width, Threshold threshold);


#endif