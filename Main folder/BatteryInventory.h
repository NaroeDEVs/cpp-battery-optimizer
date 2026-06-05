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

        void AddCell(const Battery& battery) {addCellToInventory(battery);}
        int GetCellCount() const {return cells.size();}
        void Sort() {std::sort(cells.begin(), cells.end(), std::greater<Battery>());}
        Battery GetBattery(int index) {return cells[index];}
        std::vector<Battery> GetTopCells(int numBatteries){return GetTopCells(numBatteries);};
        bool CheckIfAnyBatteryIsUnique() {return uniqueCells;}

    private:
        std::vector<Battery> cells;
        bool uniqueCells = false;

        void addCellToInventory(const Battery & battery) {
            cells.push_back(battery);
            if (!uniqueCells && cells.size()>0 && cells[0].GetCapacity() != battery.GetCapacity()) {
                uniqueCells = true;
            }
        }

        std::vector<Battery> GetTopSortedCells(int numBatteries) {
            Sort();
            int minCount = std::min(numBatteries, GetCellCount());
            return std::vector<Battery>(cells.begin(), cells.begin() + minCount);
        }
};

#endif