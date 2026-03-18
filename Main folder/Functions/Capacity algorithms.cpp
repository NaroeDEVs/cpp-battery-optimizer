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

