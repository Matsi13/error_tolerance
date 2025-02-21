#ifndef PERMUTATION_H
#define PERMUTATION_H

#include <string>
#include <queue>
#include <list>

#include "Macro.h"
#include "Memory.h"
#include "Communication.h"
#include "Compute.h"
#include "Die.h"
#include "Wafer.h"

using namespace std;



bool is_better_permutation(string first, string second, Memory& Memory_unit, Communication& Communication_unit, float Compute_size);

void Search_permutation(list<string>& permutation_side, float Compute_size, Memory& Memory_unit, Communication& Communication_unit, float relaxation);

bool is_over_threshold(Wafer solution, Threshold threshold);

bool is_better_die(Die& first, Die& second, float wafer_length, float wafer_width);

bool is_better_wafer(Wafer& first, Wafer& second);

void Permutation(Compute& Compute_unit, Memory& Memory_unit, Communication& Communication_unit, float die_padding, list<Wafer>& result, float relaxation, float wafer_length, float wafer_width, Threshold threshold);


#endif