#ifndef C_BATTERYPACKOPTIMIZER_BATTERYOPTIMIZER_H
#define C_BATTERYPACKOPTIMIZER_BATTERYOPTIMIZER_H

#include <vector>
#include <string>

struct  Battery {
    int id;
    int capacity;
    std::string manufacturer;
    std::string condition = "N/A";
};

struct BiggestCapDifference {
    int smallest=0;
    int biggest=0;
    int difference=0;
};

namespace Optimizer {
    class Battery {
    };
} // Optimizer

#endif //C_BATTERYPACKOPTIMIZER_BATTERYOPTIMIZER_H