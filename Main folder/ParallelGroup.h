#ifndef C_BATTERYPACKOPTIMIZER_PARALLELGROUP_H
#define C_BATTERYPACKOPTIMIZER_PARALLELGROUP_H
#include <vector>
#include <algorithm>
#include <functional>
#include <stdexcept>
#include "Battery.h"

// Represents a single parallel group of cells.
class ParallelGroup {

    public:
        ParallelGroup() : totalCapacity(0) {} // Empty constructor with initial total capacity set as 0.

        // Adds new cell to vector and updates total capacity by addition.
        void AddCell(const Battery& battery) {
            parallelCells.push_back(battery);
            totalCapacity += battery.GetCapacity();
        }

        int GetTotalCapacity() const { return totalCapacity; }
        int GetCellCount() const { return parallelCells.size();}

        // Sorts in descending order by capacity.
        void SortCells() { std::sort(parallelCells.begin(), parallelCells.end(), std::greater<Battery>());}

        // Returns specific index cell.
        Battery GetCell(int index) const {
            if (index < 0 || index >= parallelCells.size()) {
                throw std::out_of_range("index out of range");
            }
            return parallelCells[index];
        }

        // Replaces specified index cell with new one and adjusts total capacity.
        void SetCell(int index, const Battery& battery) {
            if (index < 0 || index >= parallelCells.size()) {
                throw std::out_of_range("Index out of range");
            }
            totalCapacity -= parallelCells[index].GetCapacity();
            totalCapacity += battery.GetCapacity();
            parallelCells[index] = battery;
        }
    private:
        std::vector<Battery> parallelCells;  // Battery vector for parallel group cells.
        int totalCapacity;       // Total capacity of the parallel group, updated whenever a cell is added or replaced.
};


#endif //C_BATTERYPACKOPTIMIZER_PARALLELGROUP_H