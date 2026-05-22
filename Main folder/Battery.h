#ifndef C_BATTERYPACKOPTIMIZER_BATTERY_H
#define C_BATTERYPACKOPTIMIZER_BATTERY_H

#include <string>
#include <iostream>
#include <format>

class Battery {
    private:
        int id;
        int capacity;
        std::string manufacturer;
        std::string condition;


    public:
        Battery() {
            id = 0;
            capacity = 0;
            manufacturer = "N/A";
            condition = "N/A";
        }

        Battery(int id, int cap, std::string man, std::string cond = "N/A") {
            this->id = id;
            this->capacity = cap;
            this->manufacturer = man;
            this->condition = cond;
        }

        std::string PrintStatus() const{
            return std::format(
                "Battery ID: {:05} | Capacity: {:4}mAh | Manufacturer: {:<12} | Condition: {}",
                id, capacity, manufacturer, condition
            );
        }

        int GetId() const{ return id; }
        int GetCapacity() const{ return capacity; }
        std::string GetManufacturer() const{ return manufacturer; }
        std::string GetCondition() const{ return condition; }

        friend bool operator<(const Battery& lhs, const Battery& rhs) {
            return lhs.capacity < rhs.capacity;
        }

        friend bool operator>(const Battery& lhs, const Battery& rhs) {
            return lhs.capacity > rhs.capacity;
        }
};

#endif