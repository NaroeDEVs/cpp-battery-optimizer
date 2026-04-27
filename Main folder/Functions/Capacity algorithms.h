#ifndef C_BATTERYPACKOPTIMIZER_CAPACITY_ALGORITHMS_H
#define C_BATTERYPACKOPTIMIZER_CAPACITY_ALGORITHMS_H

#endif //C_BATTERYPACKOPTIMIZER_CAPACITY_ALGORITHMS_H

#include "../Battery.h"
#include <vector>


int TotalCapacity(std::vector<Battery>& batteries);
int HighestCapacity(std::vector<Battery>& batteries);
int CapacityDifference(std::vector<Battery>& batteries1, std::vector<Battery>& batteries2);
int LowestCapacityIndex(std::vector<std::vector<Battery>>& batteries, int parallel);
int AveragePacksCapacity(std::vector<int>& batteryPackCapacities);
int PackDivergence(std::vector<int>& batteryPackCapacities, int average);
int PackDeviation(std::vector<Battery>& batteries);
std::string GradingResult(int percentage);