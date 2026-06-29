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
#include "ChoiceAssistant.h"

using namespace std;

int main() {
    DataHandler dataHandler("Batteries.csv", "Output.csv");

    const string file = "Batteries.csv";

    BatteryInventory AllBateries;

    int series = 0;
    int parallel = 0;

    dataHandler.ReadData(AllBateries);
    bool uniqueByCapacity = AllBateries.CheckUniqueByCapacity();
    bool uniqueByResistance = AllBateries.CheckUniqueByResistance();

    double selectedInternalResistanceLimit = 70.0;

    std::vector<Battery> unusedBatteries = AllBateries.GetAndRemoveBadResistanceCells(selectedInternalResistanceLimit);

    double resistanceWeight = dataHandler.GetUserDoubleInput("Resistance weight percentage (0 - 100%): ") / 100.0;
    double capacityWeight = dataHandler.GetUserDoubleInput("Capacity weight percentage (0 - 100%): ") / 100.0;

    string selectedType = dataHandler.GetUserStringInput("Select to pack by voltage or manual "
                                                         "series/parallel input (v for voltage, m for manual, d for defaults): ", "vmd");
    double nominalCellVoltage = 0;
    double maxCellVoltage = 4.2;

    if (selectedType == "v") {
        // Option A: Calculate configuration based on voltage targets
        maxCellVoltage = dataHandler.GetUserDoubleInput("Max single cell voltage (e.g., 4.2): ");
        double totalMaxVoltage = dataHandler.GetUserDoubleInput("Max target pack voltage (e.g., 58.8): ");
        nominalCellVoltage = dataHandler.GetUserDoubleInput("Nominal single cell voltage (e.g., 3.6 or 3.7): ");

        int numberOfCells = std::round(totalMaxVoltage / maxCellVoltage);
        if (std::abs((numberOfCells * maxCellVoltage) - totalMaxVoltage) < 0.1 && numberOfCells > 0 && totalMaxVoltage > 0) {
            std::cout << "Valid combination! Calculated cells in series: " << numberOfCells << "\n";
            series = numberOfCells;
            parallel = dataHandler.GetUserParallel();
        } else {
            std::cout << "Target pack voltage isn't a clean multiple of max cell voltage. Falling back to manual entry.\n";
            series = dataHandler.GetUserSeries();
            parallel = dataHandler.GetUserParallel();
        }
    }
    else if (selectedType == "m")
        {
        // Option B: Manual layout config
        series = dataHandler.GetUserSeries();
        parallel = dataHandler.GetUserParallel();
        nominalCellVoltage = dataHandler.GetUserDoubleInput("Nominal single cell voltage (e.g., 3.6): ");
        maxCellVoltage = dataHandler.GetUserDoubleInput("Max single cell voltage (e.g., 4.2): "); // <- Pridedame įvestį čia
    }
    else if (selectedType == "d") {
        // Option C: Defaults
        series = 16;
        parallel = 5;
        nominalCellVoltage = 3.7;
        maxCellVoltage = 4.2;
    }

    // Unified boundary check for safety
    if (series * parallel > AllBateries.GetCellCount()) {
        std::cout << "Out of bounds! You need " << (series * parallel)
                  << " cells, but only have " << AllBateries.GetCellCount() << " available.\n";
        return 0;
    }

    PackManager allPacks;

    if (uniqueByCapacity && uniqueByResistance) {
        allPacks.SetSize(series, parallel);
        allPacks.SetVoltages(nominalCellVoltage, maxCellVoltage);
        allPacks.SetOptimizationWeights(capacityWeight, resistanceWeight);
        allPacks.PackWithOptimization(AllBateries);
        dataHandler.CompactCellOutput(allPacks, "Initial base optimization");

        allPacks.HillClimbOptimization();
        dataHandler.CompactCellOutput(allPacks, "After hill climbing optimization");
        dataHandler.DetailedCellOutput(allPacks, "Detailed cell output");
    }
    else if (uniqueByCapacity && !uniqueByResistance) {
        allPacks.SetSize(series, parallel);
        allPacks.SetVoltages(nominalCellVoltage, maxCellVoltage);
        allPacks.SetOptimizationWeights(capacityWeight, 0);
        allPacks.PackWithoutOptimization(AllBateries);
        dataHandler.CompactCellOutput(allPacks, "Packs output");
        dataHandler.DetailedCellOutput(allPacks, "Detailed cell output");
    }

    else if (!uniqueByCapacity && uniqueByResistance) {
        allPacks.SetSize(series, parallel);
        allPacks.SetVoltages(nominalCellVoltage, maxCellVoltage);
        allPacks.SetOptimizationWeights(0, resistanceWeight);
        allPacks.PackWithoutOptimization(AllBateries);
        dataHandler.CompactCellOutput(allPacks, "Packs output");
        dataHandler.DetailedCellOutput(allPacks, "Detailed cell output");
    }
    else {
        allPacks.SetSize(series, parallel);
        allPacks.SetVoltages(nominalCellVoltage, maxCellVoltage);
        allPacks.PackWithoutOptimization(AllBateries);
        dataHandler.CompactCellOutput(allPacks, "Packs output");
        dataHandler.DetailedCellOutput(allPacks, "Detailed cell output");
    }

    return 0;
}