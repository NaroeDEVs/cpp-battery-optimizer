#ifndef C_BATTERYPACKOPTIMIZER_PACKMANAGER_H
#define C_BATTERYPACKOPTIMIZER_PACKMANAGER_H

#include <algorithm>
#include <string>
#include <vector>
#include <iostream>
#include "BatteryInventory.h"
#include "Battery.h"
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


    void PackWithoutOptimization(BatteryInventory& batteryInventory) {
        int k = 0;
        for (int i=0; i<seriesCount; i++) {
            for (int j=0; j<parallelCount; j++) {
                seriesGroups[i].AddCell(batteryInventory.GetBattery(k));
                k++;
            }
        }
    }

    void PackWithOptimization(BatteryInventory& batteryInventory) {
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

    void HillClimbOptimization() {
        while (true) {
            auto [min, max] = FindMaxAndMinCapacitiesIndexes();

            if (min == -1 || max == -1 || min == max) {
                break;
            }
            bool madeImprovement = OptimizeParallelGroups(seriesGroups[min], seriesGroups[max]);
            if (!madeImprovement) {
                break;
            }
        }
    }

private:
    std::vector<ParallelGroup> seriesGroups;
    int seriesCount;
    int parallelCount;

    int GetLowestCapacityGroupIndex() const {
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

    std::tuple<int, int> FindMaxAndMinCapacitiesIndexes() const {
        int currentMin = INT_MAX;
        int currentMax = INT_MIN;
        int minIndex = -1;
        int maxIndex = -1;

        for (int i = 0; i < seriesCount; i++) {
            int totalCap = seriesGroups[i].GetTotalCapacity();
            if (totalCap < currentMin) {
                currentMin = totalCap;
                minIndex = i;
            }
            if (totalCap > currentMax) {
                currentMax = totalCap;
                maxIndex = i;
            }
        }
        return {minIndex, maxIndex};
    }

    int CapacityDifference(int cap1, int cap2) const {
        return abs(cap1 - cap2);
    }
    bool OptimizeParallelGroups(ParallelGroup& parallelGroup1, ParallelGroup& parallelGroup2) {
        int bestDelta = CapacityDifference(parallelGroup1.GetTotalCapacity(), parallelGroup2.GetTotalCapacity());
        int bestI = -1;
        int bestJ = -1;

        for (int i = 0; i < parallelGroup1.GetCellCount(); i++) {
            int cap1 = parallelGroup1.TakeCell(i).GetCapacity();
            for (int j = 0; j < parallelGroup2.GetCellCount(); j++) {
                int cap2 = parallelGroup2.TakeCell(j).GetCapacity();
                int hypotheticalCap1 = parallelGroup1.GetTotalCapacity() - cap1 + cap2;
                int hypotheticalCap2 = parallelGroup2.GetTotalCapacity() - cap2 + cap1;
                int hypotheticalDelta = CapacityDifference(hypotheticalCap1, hypotheticalCap2);

                if (hypotheticalDelta < bestDelta) {
                    bestDelta = hypotheticalDelta;
                    bestI = i;
                    bestJ = j;
                }
            }
        }
        if (bestI != -1 && bestJ != -1) {
            Battery cell1 = parallelGroup1.TakeCell(bestI);
            Battery cell2 = parallelGroup2.TakeCell(bestJ);

            parallelGroup1.SetCell(bestI, cell2);
            parallelGroup2.SetCell(bestJ, cell1);
            return true;
        }

        return false;
    }
};


#endif