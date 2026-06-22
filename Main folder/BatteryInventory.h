#ifndef C_BATTERYPACKOPTIMIZER_BATTERY_INVENTORY_H
#define C_BATTERYPACKOPTIMIZER_BATTERY_INVENTORY_H

#include <algorithm>
#include <string>
#include <iostream>
#include <vector>
#include <memory>
#include "Battery.h"
#include "Battery.h"

// Represents a battery inventory.
class BatteryInventory {

    public:
        BatteryInventory() {}        // Empty constructor

        // Function to push Battery cell object to cells vector, updates uniqueCells bool variable.
        void AddCell(const Battery& battery) {
            cells.push_back(battery);
            if (!uniqueCells && cells.size()>0 && cells[0].GetCapacity() != battery.GetCapacity()) {
                uniqueCells = true;
            }
        }

        // Returns current count of batteries in storage
        int GetCellCount() const {return cells.size();}

        // Sorts in descending order by capacity.
        void Sort() {std::sort(cells.begin(), cells.end(), std::greater<Battery>());}

        // Returns true if atleast one cell is different from the rest, false if all are the same.
        bool CheckIfAnyBatteryIsUnique() const {return uniqueCells;}

        // Returns specific index cell.
        Battery GetCell(const int & index) const {
            if (index < 0 || index >= cells.size()) {
                throw std::out_of_range("index out of range");
            }
            return cells[index];
        }

        // Returns vector of top best by capacity cells (count numBatteries).
        std::vector<Battery> GetTopCells(int numBatteries) {
            Sort();
            int minCount = std::min(numBatteries, GetCellCount());
            return std::vector<Battery>(cells.begin(), cells.begin() + minCount);
        }

    private:
        std::vector<Battery> cells;     // Battery vector for storing cells.
        bool uniqueCells = false;       // Keeps info if all cells are the same or not, false = all same, true = atleast one different.
};

#endif