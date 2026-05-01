#ifndef C_BATTERYPACKOPTIMIZER_PACKING_ALGORITHMS_H
#define C_BATTERYPACKOPTIMIZER_PACKING_ALGORITHMS_H


#include "../Battery.h"
#include <vector>

void BasePackingAlgorithm(std::vector<Battery>& batteries, int series, int parallel, std::vector<std::vector<Battery>>& batteryPacks, std::vector<int>& packCapacities, std::vector<int>& numberOfBatteries);
void BiggestCapacityDifference(std::vector<int>& packCapacities, BiggestCapDifference & Difference);
void Zero(std::vector<int>& batteries, int number);
void BestBatteries(std::vector<Battery>& batteries, int series, int parallel, std::vector<Battery>& selectedBatteries);
void Sort(std::vector<Battery>& batteries);
bool CompareByCapacity(const Battery& a, const Battery& b);

#endif //C_BATTERYPACKOPTIMIZER_PACKING_ALGORITHMS_H
