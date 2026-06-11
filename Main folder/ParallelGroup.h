#ifndef C_BATTERYPACKOPTIMIZER_PARALLELGROUP_H
#define C_BATTERYPACKOPTIMIZER_PARALLELGROUP_H
#include <vector>
#include <algorithm>
#include <functional>
#include <stdexcept>
#include "Battery.h"


class ParallelGroup {

    public:
        ParallelGroup() : totalCapacity(0) {}

        void AddCell(const Battery& battery) {
            parallelCells.push_back(battery);
            totalCapacity += battery.GetCapacity();
        }

        int GetTotalCapacity() const {
            return totalCapacity;
        }

        void SortCells() {
            std::sort(parallelCells.begin(), parallelCells.end(), std::greater<Battery>());
        }

        Battery GetCell(int index) const {
            if (index < 0 || index >= parallelCells.size()) {
                throw std::out_of_range("index out of range");
            }
            return parallelCells[index];
        }

        void SetCell(int index, const Battery& battery) {
            if (index < 0 || index >= parallelCells.size()) {
                throw std::out_of_range("Index out of range");
            }
            totalCapacity -= parallelCells[index].GetCapacity();
            totalCapacity += battery.GetCapacity();
            parallelCells[index] = battery;
        }
        int GetCellCount() const {
            return parallelCells.size();
        }

    private:
        std::vector<Battery> parallelCells;
        int totalCapacity;
};


#endif //C_BATTERYPACKOPTIMIZER_PARALLELGROUP_H