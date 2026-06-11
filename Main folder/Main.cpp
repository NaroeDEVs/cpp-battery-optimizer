#include <iostream>
#include <vector>
#include <fstream>
#include <iomanip>
#include <cmath>
#include <ranges>
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


    double maxCellVoltage = dataHandler.GetUserDoubleInput("Max single cell voltage: ");
    double minCellVoltage = dataHandler.GetUserDoubleInput("Min single cell voltage: ");

    double nominalCellVoltage = (maxCellVoltage + minCellVoltage) / 2;
    double totalMaxVoltage = dataHandler.GetUserDoubleInput("Max pack voltage: ");

    int numberOfCells = std::round(totalMaxVoltage / maxCellVoltage);
    if (std::abs((numberOfCells * maxCellVoltage) - totalMaxVoltage) < 0.1) {
        std::cout << "Valid pack. Cells in series: " << numberOfCells << "\n";
        series = numberOfCells;
        parallel = dataHandler.GetUserParallel();
    }
    else {
        std::cout << "Invalid pack voltage for given cell max voltage. Reading manually.\n";
        series = dataHandler.GetUserSeries();
        parallel = dataHandler.GetUserParallel();

        if (series * parallel > AllBateries.GetCellCount()) {
            cout<<"Out of bounds!"<<endl;
            return 0;
        }
    }


    PackManager allPacks;

    if (areUnique) {
        allPacks.SetSize(series, parallel);
        allPacks.SetVoltages(maxCellVoltage, minCellVoltage, nominalCellVoltage, totalMaxVoltage);
        allPacks.PackWithOptimization(AllBateries);
        dataHandler.CompactCellOutput(allPacks, "Initial base optimization");

        allPacks.HillClimbOptimization();
        dataHandler.CompactCellOutput(allPacks, "After hill climbing optimization");
        dataHandler.DetailedCellOutput(allPacks, "Detailed cell output");
    }
    else {
        allPacks.SetSize(series, parallel);
        allPacks.SetVoltages(maxCellVoltage, minCellVoltage, nominalCellVoltage, totalMaxVoltage);
        allPacks.PackWithoutOptimization(AllBateries);
        dataHandler.CompactCellOutput(allPacks, "Packs output");
        dataHandler.DetailedCellOutput(allPacks, "Detailed cell output");

    }

    return 0;
}