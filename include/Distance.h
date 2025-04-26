#ifndef DISTANCE_H
#define DISTANCE_H

#include <queue>
#include <cmath>

#include "Macro.h"
#include "Die.h"
#include "Wafer.h"
#include "Distance.h"

// calculate the distance between two solutions
solution_distance calculate_distance(Wafer& first, Wafer& second); 

// return the possible optimals among the solutions, based on current optimal and error
// void Possible_optimal_hardward(queue<Wafer> solutions, Wafer& optimal, simulation_error& error, queue<Wafer>& possible_optimals); 

#endif