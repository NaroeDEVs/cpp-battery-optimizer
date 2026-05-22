#ifndef C_BATTERYPACKOPTIMIZER_PARALLELGROUP_H
#define C_BATTERYPACKOPTIMIZER_PARALLELGROUP_H
#include <vector>

#include "Battery.h"

class ParallelGroup {
    private:
        std::vector<Battery> parallelCells;
    public:
        ParallelGroup();

        void AddCell(const Battery& battery) {
            parallelCells.push_back(battery);
        }

        int GetTotalCapacity() const {
            int totalCapacity = 0;
            for (const auto& cell : parallelCells) {
                totalCapacity += cell.GetCapacity();
            }
            return totalCapacity;
        }

        void SortCells() {
            std::sort(parallelCells.begin(), parallelCells.end(), std::greater<Battery>());
        }



};


#endif //C_BATTERYPACKOPTIMIZER_PARALLELGROUP_H