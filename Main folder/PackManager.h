#ifndef C_BATTERYPACKOPTIMIZER_PACKMANAGER_H
#define C_BATTERYPACKOPTIMIZER_PACKMANAGER_H

#include <algorithm>
#include <string>
#include <vector>
#include <iostream>
#include <BatteryInventory.h>
#include <Battery.h>
#include "ParallelGroup.h"

class PackManager {
    private:
        std::vector<ParallelGroup> seriesGroups;
        int seriesCount;
        int parallelCount;
    public:
        PackManager();



};

#endif //C_BATTERYPACKOPTIMIZER_PACKMANAGER_H