#ifndef C_BATTERYPACKOPTIMIZER_BATTERY_H
#define C_BATTERYPACKOPTIMIZER_BATTERY_H

#include <string>
#include <iostream>
#include <format>

// Represents a single battery cell.
class Battery {
    public:
        // Empty constructor with default parameters
        Battery() {
            id = 0;
            capacity = 0;
            manufacturer = "N/A";
            condition = "N/A";
        }

        // Constructor with parameters
        Battery(int id, int cap, const std::string & man, const std::string & cond = "N/A") {
            this->id = id;
            this->capacity = cap;
            this->manufacturer = man;
            this->condition = cond;
        }

        std::string PrintStatus() const { return Status(); }
        int GetId() const{ return id; }
        int GetCapacity() const{ return capacity; }
        std::string GetManufacturer() const{ return manufacturer; }
        std::string GetCondition() const{ return condition; }

        // Compares 2 cells by capacity.
        friend bool operator<(const Battery& lhs, const Battery& rhs) {
                return lhs.capacity < rhs.capacity;
            }
        // Compares 2 cells by capacity.
        friend bool operator>(const Battery& lhs, const Battery& rhs) {
                return lhs.capacity > rhs.capacity;
            }

    private:
        int id;                     // Battery individual identifier
        int capacity;               // Capacity in mAh
        std::string manufacturer;   // Manufacturer name
        std::string condition;      // Battery condition

        // Returns formatted string with battery info.
        std::string Status() const{
            return std::format(
                "{:<17} | {:<17} | {:<26} | {} ",
                id, capacity, manufacturer, condition
            );
        }
};

#endif