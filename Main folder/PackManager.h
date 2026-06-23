#ifndef C_BATTERYPACKOPTIMIZER_PACKMANAGER_H
#define C_BATTERYPACKOPTIMIZER_PACKMANAGER_H

#include <string>
#include <vector>
#include "BatteryInventory.h"
#include "Battery.h"
#include "ParallelGroup.h"
#include "limits.h"

// Represent whole pack of parallel groups
class PackManager {
public:
    PackManager() = default;

    // sets the size of the pack in series and parallel
    void SetSize(int  series, int parallel) {
        seriesCount = series;
        parallelCount = parallel;
        seriesGroups.resize(seriesCount);
    }

    // sets single cell (Parallel group) nominal and maximum voltages
    void SetVoltages(double NominalCellVoltage, double MaxSingleCellVoltage) {
        nominalCellVoltage = NominalCellVoltage;
        maxSingleCellVoltage = MaxSingleCellVoltage;
    }

    // ---- Exemption stage ----
    // Packs the cells into the pack without any optimization, just filling groups in order.
    // Used in case of all cells being same capacity to reduce complexity and time of packing.
    void PackWithoutOptimization(BatteryInventory& batteryInventory) {
        int k = 0;
        for (int i=0; i<seriesCount; i++) {
            for (int j=0; j<parallelCount; j++) {
                seriesGroups[i].AddCell(batteryInventory.GetCell(k));
                k++;
            }
        }
    }

    // ---- 1st packing stage: Greedy packing algorithm. ----
    // Packs by taking top series*parallel sorted cells and assigns them to whichever next parallel group has lowest capacity.
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

    int GetSeries() const { return seriesCount; }
    int GetParallel() const { return parallelCount; }

    Battery GetCell(int seriesIndex, int parallelIndex) const { return seriesGroups[seriesIndex].GetCell(parallelIndex); }

    // Single parallel group info getters.
    int GetIndexParallelCapacity(int seriesIndex) const { return seriesGroups[seriesIndex].GetTotalCapacity();}
    int GetIndexParallelMaxCapacity(int seriesIndex) const { return seriesGroups[seriesIndex].GetMaxCapacity(); }
    int GetIndexParallelMinCapacity(int seriesIndex) const { return seriesGroups[seriesIndex].GetMinCapacity(); }
    double GetIndexParallelAverageCapacity(int seriesIndex) const { return seriesGroups[seriesIndex].GetAverageCapacity(); }
    double GetIndexParallelVariancePercentage(int seriesIndex) const { return seriesGroups[seriesIndex].GetVariancePercentage(); }
    double GetIndexParallelTotalPackVoltage(int seriesIndex) const { return seriesGroups[seriesIndex].GetTotalPackEnergy(nominalCellVoltage); }

    // Returns maximum parallel group total capacity.
    int MaxCapacity() const {
        if (seriesCount > 0) {
            return FindMinMaxIndexes().maxCapacity;
        }
        return 0;
    }

    // Returns minimum parallel group total capacity.
    int MinCapacity() const {
        if (seriesCount > 0) {
            return FindMinMaxIndexes().minCapacity;
        }
        return 0;
    }

    // Returns whole pack capacity (sum of all parallel groups capacities).
    int CalculateTotalCapacity() const {
        int totalCapacity = 0;
        for (int i = 0; i < seriesCount; i++) {
            totalCapacity += seriesGroups[i].GetTotalCapacity();
        }
        return totalCapacity;
    }

    // Returns average parallel group capacity.
    double CalculateAverageCapacity() const {
        int totalCapacity = CalculateTotalCapacity();
        return static_cast<double>(totalCapacity) / seriesCount;
    }

    // Returns percentage variance between the weakest and strongest parallel group, calculated as (max - min) / average * 100.
    double CalculateVariancePercentage() const {
        double variance = 0;
        double averageCap = CalculateAverageCapacity();
        auto extremes = FindMinMaxIndexes();
        int biggestDifference = extremes.maxCapacity - extremes.minCapacity;
        variance = static_cast<double>(biggestDifference) / averageCap;
        return variance * 100;
    }

    // Returns actual usable pack capacity, which is the capacity of the weakest parallel group, since it limits the whole pack.
    int GetUsablePackCapacity() const {
        return MinCapacity();
    }

    // Calculates true usable energy in Watt-hours (Wh)
    double CalculateTotalPackEnergy() const {
        double usableAh = static_cast<double>(GetUsablePackCapacity()) / 1000.0;
        double totalPackVoltage = nominalCellVoltage * seriesCount;
        return usableAh * totalPackVoltage;
    }

    // ---- 2nd packing stage: Hill climbing optimization algorithm. ----
    // Iteratively tries to optimize the pack by swapping cells between the strongest and weakest parallel groups, until no improvement can be made.
    void HillClimbOptimization() {
        while (true) {
            auto extremes = FindMinMaxIndexes();

            if (extremes.minCapacityIndex == -1 || extremes.maxCapacityIndex == -1 || extremes.minCapacityIndex == extremes.maxCapacityIndex) {
                break;
            }
            bool madeImprovement = OptimizeParallelGroups(seriesGroups[extremes.minCapacityIndex], seriesGroups[extremes.maxCapacityIndex]);
            if (!madeImprovement) break;
        }
    }

    double TotalMaxVoltage() const {
        return maxSingleCellVoltage * seriesCount;
    }


private:
    std::vector<ParallelGroup> seriesGroups; // Vector holding parallel groups, which in turn hold cells.
    int seriesCount;                         // Number of parallel groups in series (number of cells in series).
    int parallelCount;                       // Number of cells in each parallel group.
    double nominalCellVoltage;               // Nominal voltage of a single cell.
    double maxSingleCellVoltage;             // Maximum voltage of a single cell.

    // Holds indexes of the parallel groups with minimum and maximum total capacities.
    struct PackExtremes {
        int minCapacityIndex = -1;
        int maxCapacityIndex = -1;
        int minCapacity = INT_MAX;
        int maxCapacity = INT_MIN;

        int minInternalResistanceIndex = -1;
        int maxInternalResistanceIndex = -1;
        double minInternalResistance = std::numeric_limits<double>::max();
        double maxInternalResistance = std::numeric_limits<double>::min();
    };


    // Returns index of the parallel group with the lowest total capacity, that isn't already full (doesn't have parallelCount cells).
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

    // Finds the indexes of the parallel groups with minimum and maximum total capacities and internal resistances.
    PackExtremes FindMinMaxIndexes() const {
        if (seriesCount == 0) return {};
        PackExtremes extremes;
        for (int i = 0; i < seriesCount; i++) {

            int totalCap = seriesGroups[i].GetTotalCapacity();
            if (totalCap < extremes.minCapacity) {
                extremes.minCapacityIndex = i;
                extremes.minCapacity = totalCap;
            }
            if (totalCap > extremes.maxCapacity) {
                extremes.maxCapacityIndex = i;
                extremes.maxCapacity = totalCap;
            }

            double internalResistance = seriesGroups[i].GetTotalInternalResistance();
            if (internalResistance < extremes.minInternalResistance) {
                extremes.minInternalResistanceIndex = i;
                extremes.minInternalResistance = internalResistance;
            }
            if (internalResistance > extremes.maxInternalResistance) {
                extremes.maxInternalResistanceIndex = i;
                extremes.maxInternalResistance = internalResistance;
            }
        }
        return extremes;
    }

    // Returns the absolute difference between two capacities.
    int CapacityDifference(int cap1, int cap2) const {
        return abs(cap1 - cap2);
    }

    // Part of the hill climbing optimization, tries to swap cells between two parallel groups to reduce the capacity difference.
    // Returns true if an improvement was made.
    bool OptimizeParallelGroups(ParallelGroup& parallelGroup1, ParallelGroup& parallelGroup2) {
        int bestDelta = CapacityDifference(parallelGroup1.GetTotalCapacity(), parallelGroup2.GetTotalCapacity());
        int bestI = -1;
        int bestJ = -1;

        for (int i = 0; i < parallelGroup1.GetCellCount(); i++) {
            int cap1 = parallelGroup1.GetCell(i).GetCapacity();
            for (int j = 0; j < parallelGroup2.GetCellCount(); j++) {
                int cap2 = parallelGroup2.GetCell(j).GetCapacity();
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
            Battery cell1 = parallelGroup1.GetCell(bestI);
            Battery cell2 = parallelGroup2.GetCell(bestJ);

            parallelGroup1.SetCell(bestI, cell2);
            parallelGroup2.SetCell(bestJ, cell1);
            return true;
        }

        return false;
    }
};


#endif