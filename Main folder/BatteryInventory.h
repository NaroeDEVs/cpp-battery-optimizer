#ifndef C_BATTERYPACKOPTIMIZER_BATTERY_INVENTORY_H
#define C_BATTERYPACKOPTIMIZER_BATTERY_INVENTORY_H

#include <algorithm>
#include <string>
#include <iostream>
#include <vector>
#include <memory>
#include "Battery.h"
#include "Battery.h"

class BatteryInventory {
    private:
        std::vector<Battery> cells;

    public:
        BatteryInventory() {}

        void addCell(const Battery & battery) {
            cells.push_back(battery);
        }

        int GetCellCount() const {
            return cells.size();
        }

        void Sort() {
            std::sort(cells.begin(), cells.end(), std::greater<Battery>());
        }

        Battery TakeBattery(int index) {
            return cells[index];
        }

        std::vector<Battery> GetTopCells(int numBatteries) {
            Sort();
            int minCount = std::min(numBatteries, GetCellCount());
            return std::vector<Battery>(cells.begin(), cells.begin() + minCount);
        }
};

#endif