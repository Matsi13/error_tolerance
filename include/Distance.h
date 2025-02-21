#ifndef DISTANCE_H
#define DISTANCE_H

#include <queue>
#include <cmath>

#include "Macro.h"
#include "Die.h"
#include "Wafer.h"
#include "Distance.h"

solution_distance calculate_distance(Wafer& first, Wafer& second);

void Possible_optimal(queue<Wafer> solutions, Wafer& optimal, simulation_error& error, queue<Wafer>& possible_optimals);

#endif