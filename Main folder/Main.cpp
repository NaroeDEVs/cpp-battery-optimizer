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

    int series = 0;
    int parallel = 0;

    dataHandler.ReadData(AllBateries);
    bool areUnique = AllBateries.CheckIfAnyBatteryIsUnique();

    string selectedType = dataHandler.GetUserStringInput("Select to pack by voltage or manual "
                                                         "series/parallel input (v for voltage, m for manual): ", "vm");

    double nominalCellVoltage = dataHandler.GetUserDoubleInput("Nominal single cell voltage: ");
    double totalVoltage = dataHandler.GetUserDoubleInput("Total pack voltage: ");
    int numberOfCells = totalVoltage/nominalCellVoltage;

    if (numberOfCells * nominalCellVoltage == totalVoltage) {
        std::cout << "Valid pack. Cells in series: " << numberOfCells << "\n";
        series = numberOfCells;
        parallel = dataHandler.GetUserParallel();
    }
    else {
        std::cout << "Invalid pack voltage for given cell nominal voltage. Reading manually.\n";
        series = dataHandler.GetUserSeries();
        parallel = dataHandler.GetUserParallel();

        if (series * parallel > AllBateries.GetCellCount()) {
            cout<<"Out of bounds!"<<endl;
            return 0;
        }
    }


    PackManager allPacks;

    if (areUnique) {
        allPacks.SetSize(series, parallel, nominalCellVoltage, totalVoltage);
        allPacks.PackWithOptimization(AllBateries);
        dataHandler.CompactCellOutput(allPacks, "Initial base optimization");

        allPacks.HillClimbOptimization();
        dataHandler.CompactCellOutput(allPacks, "After hill climbing optimization");
        dataHandler.DetailedCellOutput(allPacks, "Detailed cell output");
    }
    else {
        allPacks.SetSize(series, parallel, nominalCellVoltage, totalVoltage);
        allPacks.PackWithoutOptimization(AllBateries);
        dataHandler.CompactCellOutput(allPacks, "Packs output");
        dataHandler.DetailedCellOutput(allPacks, "Detailed cell output");

    }

    return 0;
}