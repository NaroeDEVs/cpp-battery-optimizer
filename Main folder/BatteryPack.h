#ifndef C_BATTERYPACKOPTIMIZER_BATTERY_PACK_H
#define C_BATTERYPACKOPTIMIZER_BATTERY_PACK_H

#include <algorithm>
#include <string>
#include <iostream>
#include <vector>
#include <memory>
#include "Battery.h"
#include "Battery.h"

class BatteryPack {
    private:
        std::vector<Battery> packs;

    public:
        BatteryPack(){
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

        void Sort() {
            std::sort(packs.begin(), packs.end(), std::greater<Battery>());
        }

        Battery TakeBattery(int index) {
            return packs[index];
        }

        void OutputAll() const {
            for (Battery battery : packs) {
                std::cout << battery.PrintStatus() << std::endl;
            }
        }

        void ResizeTo(int newSize) {
            if (newSize < packs.size()) {
                packs.resize(newSize);
            }
        }
};


#endif