#ifndef C_BATTERYPACKOPTIMIZER_PARALLELGROUP_H
#define C_BATTERYPACKOPTIMIZER_PARALLELGROUP_H
#include <vector>
#include <algorithm>
#include <functional>
#include <stdexcept>
#include "Battery.h"
#include <limits.h>

// Represents a single parallel group of cells.
class ParallelGroup {

    public:
        ParallelGroup() : totalCapacity(0) {} // Empty constructor with initial total capacity set as 0.

        // Adds new cell to vector and updates total capacity by addition. Also updates min and max capacity indexes.
        void AddCell(const Battery& battery) {
            parallelCells.push_back(battery);
            totalCapacity += battery.GetCapacity();
            UpdateMinAndMaxCapacity();
            CalculateTotalInternalResistance();
        }

        int GetTotalCapacity() const { return totalCapacity; }
        int GetCellCount() const { return parallelCells.size();}

        // Sorts in descending order by capacity. Also updates min and max capacity indexes after sorting.
        void SortCells() {
            std::sort(parallelCells.begin(), parallelCells.end(), std::greater<Battery>());
            UpdateMinAndMaxCapacity();
        }

        // Returns specific index cell.
        Battery GetCell(int index) const {
            if (index < 0 || index >= parallelCells.size()) {
                throw std::out_of_range("index out of range");
            }
            return parallelCells[index];
        }

        // Replaces specified index cell with new one and adjusts total capacity. Also updates min and max capacity indexes after replacement.
        void SetCell(int index, const Battery& battery) {
            if (index < 0 || index >= parallelCells.size()) {
                throw std::out_of_range("Index out of range");
            }
            totalCapacity -= parallelCells[index].GetCapacity();
            totalCapacity += battery.GetCapacity();
            parallelCells[index] = battery;
            UpdateMinAndMaxCapacity();
            CalculateTotalInternalResistance();
        }

        int GetMinCapacity() const {return parallelCells[minCapacityIndex].GetCapacity();}
        int GetMaxCapacity() const {return parallelCells[maxCapacityIndex].GetCapacity();}
        double GetAverageCapacity() const {return totalCapacity/parallelCells.size();}

        // Calculates the percentage difference between the maximum and minimum capacities in the parallel group relative to the average capacity.
        double GetVariancePercentage() const {
            int maximumDifference  = parallelCells[maxCapacityIndex].GetCapacity() - parallelCells[minCapacityIndex].GetCapacity();
            if (maximumDifference == 0) return 0.0;
            return static_cast<double>(maximumDifference) / GetAverageCapacity() * 100.0;
        }

        // Calculates the total energy of the parallel group in Watt-hours (Wh) based on the nominal cell voltage.
        double GetTotalPackEnergy(double nominalCellVoltage) const { return totalCapacity / 1000.0 * nominalCellVoltage ; }
        double GetTotalInternalResistance() const { return totalInternalResistance; }



    private:
        std::vector<Battery> parallelCells;  // Battery vector for parallel group cells.
        int totalCapacity;       // Total capacity of the parallel group, updated whenever a cell is added or replaced.
        int minCapacityIndex = -1; // Index of the cell with minimum capacity in the parallel group.
        int maxCapacityIndex = -1; // Index of the cell with maximum capacity in the parallel group.
        double totalInternalResistance = 0.0;


        // Finds the indexes of the cells with minimum and maximum capacities in the parallel group and updates the corresponding member variables.
        void UpdateMinAndMaxCapacity() {
            if (parallelCells.empty()) {
                minCapacityIndex = -1;
                maxCapacityIndex = -1;
                return;
            }
            minCapacityIndex = 0;
            maxCapacityIndex = 0;
            for (int i = 1; i < parallelCells.size(); i++) {
                if (parallelCells[i].GetCapacity() < parallelCells[minCapacityIndex].GetCapacity()) {
                    minCapacityIndex = i;
                }
                if (parallelCells[i].GetCapacity() > parallelCells[maxCapacityIndex].GetCapacity()) {
                    maxCapacityIndex = i;
                }
            }
        }

        // Calculates total internal resistance of the parallel group using the formula: 1/R_total = 1/R1 + 1/R2 + ... + 1/Rn.
        void CalculateTotalInternalResistance() {
            double internalResitance = 0;
            for (int i=0; i<parallelCells.size(); i++) {
                if (parallelCells[i].GetInternalResistance() > 0) {
                    internalResitance += 1.0/parallelCells[i].GetInternalResistance();
                }
            }
            if (internalResitance > 0) {
                totalInternalResistance = 1.0/internalResitance;
            }
            else totalInternalResistance = 0.0;
        }

};


#endif //C_BATTERYPACKOPTIMIZER_PARALLELGROUP_H