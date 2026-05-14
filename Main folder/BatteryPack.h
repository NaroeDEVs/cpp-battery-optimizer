#ifndef C_BATTERYPACKOPTIMIZER_BATTERY_PACK_H
#define C_BATTERYPACKOPTIMIZER_BATTERY_PACK_H

#include <string>
#include <iostream>
#include <vector>

#include "Battery.h"
#include "Battery.h"

class BatteryPack {
    private:
        std::vector<Battery> packs;

    public:
        BatteryPack() {

        }

        void addCell(const Battery & battery) {
            packs.push_back(battery);
        }

        int GetCellCount() const {
            return packs.size();
        }

        int GetTotalCapacity() const {
            int totalCapacity = 0;
            for (Battery battery : packs) {
                totalCapacity += battery.GetCapacity();
            }
            return totalCapacity;
        }
};


#endif