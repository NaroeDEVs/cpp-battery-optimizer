#ifndef C_BATTERYPACKOPTIMIZER_PARALLELGROUP_H
#define C_BATTERYPACKOPTIMIZER_PARALLELGROUP_H
#include <vector>

#include "Battery.h"

class ParallelGroup {

    public:
        ParallelGroup() = default;
        void AddCell(const Battery& battery) {AddBatCell(battery);}
        int GetTotalCapacity() const {return GetTotalCapacityOfWholeGroup();}
        void SortCells() {SortingCells();}
        Battery TakeCell(int index) const {return TakeCellByIndex(index);}
        void SetCell(int index, const Battery& battery) {SetCellByIndex(index, battery);}
        int GetCellCount() const {return GetCellCountSize();}

    private:
        std::vector<Battery> parallelCells;

        void AddBatCell(const Battery& battery) {
            parallelCells.push_back(battery);
        }

        int GetTotalCapacityOfWholeGroup() const {
            int totalCapacity = 0;
            for (const auto& cell : parallelCells) {
                totalCapacity += cell.GetCapacity();
            }
            return totalCapacity;
        }

        void SortingCells() {
            std::sort(parallelCells.begin(), parallelCells.end(), std::greater<Battery>());
        }

        Battery TakeCellByIndex(int index) const {
            if (index < 0 || index >= parallelCells.size()) {
                throw std::out_of_range("index out of range");
            }
            return parallelCells[index];
        }

        void SetCellByIndex(int index, const Battery& battery) {
            if (index < 0 || index >= parallelCells.size()) {
                throw std::out_of_range("Index out of range");
            }
            parallelCells[index] = battery;
        }

        int GetCellCountSize() const {
            return parallelCells.size();
        }
};


#endif //C_BATTERYPACKOPTIMIZER_PARALLELGROUP_H