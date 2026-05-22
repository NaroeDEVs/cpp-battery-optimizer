#ifndef C_BATTERYPACKOPTIMIZER_PACKMANAGER_H
#define C_BATTERYPACKOPTIMIZER_PACKMANAGER_H

#include <algorithm>
#include <string>
#include <vector>
#include <iostream>
#include <BatteryInventory.h>
#include <Battery.h>
#include "ParallelGroup.h"

class PackManager {
private:
    std::vector<ParallelGroup> seriesGroups;
    int seriesCount;
    int parallelCount;

    int GetLowestCapacityGroupIndex() {
        int capacity = INT32_MAX;
        int index = -1;
        for (int i = 0; i < seriesCount; i++) {
            if (seriesGroups[i].GetTotalCapacity() < capacity && seriesGroups[i].GetCellCount() < parallelCount) {
                capacity = seriesGroups[i].GetTotalCapacity();
                index = i;
            }
        }
        return index;
    }

public:
    PackManager();

    void SetSize(int series, int parallel) {
        seriesCount = series;
        parallelCount = parallel;
        seriesGroups.resize(seriesCount);
    }

    /**
     * @brief Packs the battery cells into the series groups by selecting the best available cells from the inventory and adding them to the
     *            group with the lowest total capacity until all groups are filled or there are no more cells left.
     * @param batteryInventory All battery cells
     */
    void Pack(BatteryInventory& batteryInventory) {
        std::vector<Battery> bestCells = batteryInventory.GetTopCells(seriesCount * parallelCount);
        for (int i = 0; i < seriesCount*parallelCount; i++) {
            Battery battery = bestCells[i];
            int groupIndex = GetLowestCapacityGroupIndex();
            if (groupIndex != -1) {
                seriesGroups[groupIndex].AddCell(battery);
            }
        }
    }

};

#endif