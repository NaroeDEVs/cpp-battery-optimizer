#ifndef C_BATTERYPACKOPTIMIZER_PACKMANAGER_H
#define C_BATTERYPACKOPTIMIZER_PACKMANAGER_H

#include <algorithm>
#include <string>
#include <vector>
#include <iostream>
#include <BatteryInventory.h>
#include <Battery.h>
#include "ParallelGroup.h"
#include "limits.h"
#include <tuple>

class PackManager {
public:
    PackManager() = default;

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

    int GetSeries() const {
        return seriesCount;
    }
    int GetParallel() const {
        return parallelCount;
    }

    Battery TakeBattery(int seriesIndex, int parallelIndex) const {
        return seriesGroups[seriesIndex].TakeCell(parallelIndex);
    }

    int GetIndexParallelCapacity(int seriesIndex) const {
        return seriesGroups[seriesIndex].GetTotalCapacity();
    }

    int MaxCapacity() const {
        auto[min, max] = FindMaxAndMinCapacities();
        return max;
    }

    int MinCapacity() const {
        auto[min, max] = FindMaxAndMinCapacities();
        return min;
    }

    int CalculateTotalCapacity() const {
        int totalCapacity = 0;
        for (int i = 0; i < seriesCount; i++) {
            totalCapacity += seriesGroups[i].GetTotalCapacity();
        }
        return totalCapacity;
    }

    double CalculateAverageCapacity() const {
        int totalCapacity = CalculateTotalCapacity();
        return static_cast<double>(totalCapacity) / seriesCount;
    }

    double CalculateVariancePercentage() const {
        double variance = 0;

        double averageCap = CalculateAverageCapacity();

        auto[min, max] = FindMaxAndMinCapacities();
        int biggestDifference = max - min;

        variance = static_cast<double>(biggestDifference) / averageCap;
        return variance * 100;
    }





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

    std::tuple<int, int> FindMaxAndMinCapacities() const {
        int currentMin = INT_MAX;
        int currentMax = INT_MIN;
        for (int i = 0; i < seriesCount; i++) {
            int totalCap = seriesGroups[i].GetTotalCapacity();
            if (totalCap < currentMin) {
                currentMin = totalCap;
            }
            if (totalCap > currentMax) {
                currentMax = totalCap;
            }
        }
        return {currentMin, currentMax};
    }
};

#endif