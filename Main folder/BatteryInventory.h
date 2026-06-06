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
        BatteryInventory() {}

        // Function to push Battery cell object to cells vector, updates uniqueCells bool variable.
        void AddCell(const Battery& battery) {
            cells.push_back(battery);
            if (!uniqueCells && cells.size()>0 && cells[0].GetCapacity() != battery.GetCapacity()) {
                uniqueCells = true;
            }
        }

        int GetCellCount() const {return cells.size();}

        // Sorts in descending order by capacity.
        void Sort() {std::sort(cells.begin(), cells.end(), std::greater<Battery>());}

        Battery GetBattery(int index) {return cells[index];}

        // Returns vector of top best by capacity cells (count numBatteries).
        std::vector<Battery> GetTopCells(int numBatteries) {
            Sort();
            int minCount = std::min(numBatteries, GetCellCount());
            return std::vector<Battery>(cells.begin(), cells.begin() + minCount);
        }

        // Returns false if atleast one battery is different from others
        bool CheckIfAnyBatteryIsUnique() {return uniqueCells;}

    private:
        std::vector<Battery> cells;
        bool uniqueCells = false;
};

#endif