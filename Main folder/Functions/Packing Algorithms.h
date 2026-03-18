#ifndef C_BATTERYPACKOPTIMIZER_PACKING_ALGORITHMS_H
#define C_BATTERYPACKOPTIMIZER_PACKING_ALGORITHMS_H

#endif //C_BATTERYPACKOPTIMIZER_PACKING_ALGORITHMS_H

#include "../Battery.h"
#include <vector>
using namespace std;

void BasePackingAlgorithm(vector<Battery>& batteries, int series, int parallel, vector<vector<Battery>>& batteryPacks, vector<int>& packCapacities, vector<int>& numberOfBatteries);
void BiggestCapacityDifference(vector<int>& packCapacities, BiggestCapDifference & Difference);
void Zero(vector<int>& batteries, int number);
void BestBatteries(vector<Battery>& batteries, int series, int parallel, vector<Battery>& selectedBatteries);
void Sort(vector<Battery>& batteries);
bool CompareByCapacity(const Battery& a, const Battery& b);
