#ifndef C_BATTERYPACKOPTIMIZER_PARALLELGROUP_H
#define C_BATTERYPACKOPTIMIZER_PARALLELGROUP_H
#include <vector>

#include "Battery.h"

class ParallelGroup {
    private:
        std::vector<Battery> parallelCells;
    public:
        ParallelGroup() = default;

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

        Battery TakeCell(int index) const {
            if (index < 0 || index >= parallelCells.size()) {
                throw std::out_of_range("index out of range");
            }
            return parallelCells[index];
        }

        void SetCell(int index, const Battery& battery) {
            if (index < 0 || index >= parallelCells.size()) {
                throw std::out_of_range("Index out of range");
            }
            parallelCells[index] = battery;
        }

        int GetCellCount() const {
            return parallelCells.size();
        }
};


#endif //C_BATTERYPACKOPTIMIZER_PARALLELGROUP_H