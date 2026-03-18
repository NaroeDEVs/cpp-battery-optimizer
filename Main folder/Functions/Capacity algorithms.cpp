#include "Capacity algorithms.h"
#include <vector>
#include <algorithm>
#include <cmath>

using namespace std;

int TotalCapacity(vector<Battery>& batteries) {
    int totalCapacity = 0;
    for (int i=0; i <batteries.size(); i++) {
        totalCapacity += batteries[i].capacity;
    }
    return totalCapacity;
}

int HighestCapacity(vector<Battery>& batteries) {
    int highestCapacity = 0;
    for (int i=0; i <batteries.size(); i++) {
        if (batteries[i].capacity > highestCapacity) {
            highestCapacity = batteries[i].capacity;
        }
    }
    return highestCapacity;
}

int CapacityDifference(vector<Battery>& batteries1, vector<Battery>& batteries2) {
    int totalCapacity1 = TotalCapacity(batteries1);
    int totalCapacity2 = TotalCapacity(batteries2);
    int difference = abs(totalCapacity1 - totalCapacity2);
    if (totalCapacity1 > totalCapacity2) {
        return -1*difference;
    }
    else return difference;
}

int LowestCapacityIndex(vector<vector<Battery>>& batteries, int parallel) {
    int lowestCapacity = 9999999;
    int index = -1;

    for (int i=0; i<batteries.size(); i++) {
        int capacity = TotalCapacity(batteries[i]);
        if (capacity < lowestCapacity && batteries[i].size() < parallel) {
            lowestCapacity = capacity;
            index = i;
        }
    }
    return index;
}

int AveragePacksCapacity(vector<int>& batteryPackCapacities) {
    int sum = 0;
    for (int i=0; i<batteryPackCapacities.size(); i++) {
        sum += batteryPackCapacities[i];
    }
    if (batteryPackCapacities.size() == 0) return 0;
    return sum / batteryPackCapacities.size();
}

int PackDivergence(vector<int>& batteryPackCapacities, int average) {
    int av = AveragePacksCapacity(batteryPackCapacities);
    return av - average;
}

int PackDeviation(vector<Battery>& batteries) {
    int smallestBattery = batteries[0].capacity;
    int biggestBattery = batteries[0].capacity;
    for (int i=1; i<batteries.size(); i++) {
        if (batteries[i].capacity < smallestBattery) {
            smallestBattery = batteries[i].capacity;
        }
        if (batteries[i].capacity > biggestBattery) {
            biggestBattery = batteries[i].capacity;
        }
    }
    return biggestBattery-smallestBattery;
}
