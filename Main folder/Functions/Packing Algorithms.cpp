#include "Packing Algorithms.h"
#include "Capacity algorithms.h"
#include <vector>
#include <algorithm>

using namespace std;

bool CompareByCapacity(const Battery& a, const Battery& b) {
    return a.capacity > b.capacity;
}

void Sort(vector<Battery>& batteries) {
    sort(batteries.begin(), batteries.end(), CompareByCapacity);
}


void BestBatteries(vector<Battery>& batteries, int series, int parallel, vector<Battery>& selectedBatteries) {
    for (int i=0; i <series*parallel; i++) {
        selectedBatteries.push_back(batteries[i]);
    }
}

void Zero(vector<int>& batteries, int number) {
    for (int i=0; i<number; i++) {
        batteries[i] = 0;
    }
}


void BasePackingAlgorithm(vector<Battery>& batteries, int series, int parallel, vector<vector<Battery>>& batteryPacks, vector<int>& packCapacities, vector<int>& numberOfBatteries) {
    packCapacities.resize(series);
    numberOfBatteries.resize(series);
    batteryPacks.resize(series);

    Zero(numberOfBatteries, series);
    Zero(packCapacities, series);

    for (int i=0; i<batteries.size(); i++) {
        int k = LowestCapacityIndex(batteryPacks, parallel);

        if (k!=-1) {
            batteryPacks[k].push_back(batteries[i]);
            packCapacities[k] = TotalCapacity(batteryPacks[k]);
            numberOfBatteries[k] = batteryPacks[k].size();
        }
    }
}

void BiggestCapacityDifference(vector<int>& packCapacities, BiggestCapDifference & Difference) {
    int BiggestCapacity=0;
    int SmallestCapacity=9999999;

    int smallest, biggest, difference;
    for (int i=0; i<packCapacities.size(); i++) {
        int capacity = packCapacities[i];
        if (capacity < SmallestCapacity) {
            smallest = i;
            SmallestCapacity = capacity;
        }
        if (capacity > BiggestCapacity) {
            BiggestCapacity = capacity;
            biggest = i;
        }
    }
    difference = BiggestCapacity - SmallestCapacity;

    Difference.smallest= smallest;
    Difference.biggest= biggest;
    Difference.difference= difference;
}