#include <iostream>
#include <vector>
#include <fstream>
#include <iomanip>
#include <cmath>
#include <sstream>
#include "Battery.h"
#include "BatteryInventory.h"
#include "DataHandler.h"
#include "ParallelGroup.h"
#include "PackManager.h"

using namespace std;

int main() {
    DataHandler dataHandler("Batteries.csv", "Output.csv");

    const string file = "Batteries.csv";

    BatteryInventory AllBateries;

    dataHandler.ReadData(AllBateries);
    bool areUnique = AllBateries.CheckIfAnyBatteryIsUnique();

    int series = dataHandler.GetUserSeries();
    int parallel = dataHandler.GetUserParallel();

    if (series * parallel > AllBateries.GetCellCount()) {
        cout<<"Out of bounds!"<<endl;
        return 0;
    }

    PackManager allPacks;

    if (areUnique) {
        allPacks.SetSize(series, parallel);
        allPacks.PackWithOptimization(AllBateries);
        dataHandler.CompactCellOutput(allPacks, "Initial base optimization");

        allPacks.HillClimbOptimization();
        dataHandler.CompactCellOutput(allPacks, "After hill climbing optimization");
    }
    else {
        allPacks.SetSize(series, parallel);
        allPacks.PackWithoutOptimization(AllBateries);
        dataHandler.CompactCellOutput(allPacks, "Packs output");
    }

    return 0;
}