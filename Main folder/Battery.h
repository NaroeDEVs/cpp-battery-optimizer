#ifndef C_BATTERYPACKOPTIMIZER_BATTERYOPTIMIZER_H
#define C_BATTERYPACKOPTIMIZER_BATTERYOPTIMIZER_H

#include <string>
#include <iostream>

class Battery {
    private:
        int id;
        int capacity;
        std::string manufacturer;
        std::string condition;

    public:
        Battery(int id, int cap, std::string man, std::string cond = "N/A") {
            this->id = id;
            this->capacity = cap;
            this->manufacturer = man;
            this->condition = cond;
        }

        std::string PrintStatus() {
            std::string line = "Battery ID: " + std::to_string(id) + ", Capacity: " + std::to_string(capacity) + "mAh, Manufacturer: " + manufacturer + ", Condition: " + condition;
            return line;
        }

        int GetId() { return id; }
        int GetCapacity() { return capacity; }
        std::string GetManufacturer() { return manufacturer; }
        std::string GetCondition() { return condition; }
};

    struct BiggestCapDifference {
        int smallest = 0;
        int biggest = 0;
        int difference = abs(biggest - smallest);
    };

#endif