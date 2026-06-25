#ifndef C_BATTERYPACKOPTIMIZER_DATAHANDLER_H
#define C_BATTERYPACKOPTIMIZER_DATAHANDLER_H

#include <string>
#include "Battery.h"
#include "BatteryInventory.h"
#include <fstream>
#include <sstream>
#include <vector>
#include <iostream>
#include <limits>
#include "PackManager.h"

class DataHandler {
public:
    // Sets the source and destination file paths for reading and writing data.
    DataHandler(std::string sourceFilePath, std::string destinationFilePath) : sourceFilePath(sourceFilePath),
                                                                               destinationFilePath(
                                                                                   destinationFilePath) {
    }

    // Reads battery data from a CSV file and populates the provided BatteryInventory object.
    void ReadData(BatteryInventory &batteryPack) {
        std::ifstream reader(sourceFilePath);
        if (!reader.is_open()) {
            std::cout << "Error opening file: " << sourceFilePath << std::endl;
            return;
        }

        std::string line;
        std::getline(reader, line);
        while (std::getline(reader, line)) {
            std::istringstream ss(line);
            std::string idString, capacityString, manufacturerString, conditionString, internalResistanceString;

            if (!std::getline(ss, idString, ';')) continue;
            if (!std::getline(ss, capacityString, ';')) continue;
            if (!std::getline(ss, internalResistanceString, ';')) continue;
            if (!std::getline(ss, manufacturerString, ';')) continue;

            std::getline(ss, conditionString, ';');

            int id = std::stoi(idString);
            int capacity = std::stoi(capacityString);
            double internalResistance = std::stod(internalResistanceString);

            Battery battery(id, capacity, internalResistance, manufacturerString,
                            conditionString.empty() ? "N/A" : conditionString);
            batteryPack.AddCell(battery);
        }
    }

    int GetUserSeries() { return GetUserInt("series: "); }
    int GetUserParallel() { return GetUserInt("parallel: "); }
    int GetUserLevelOfOptimization() { return GetUserInt("Level of optimization: "); }

    // Compact output of all cells in the pack, showing only capacities and summary statistics.
    void CompactCellOutput(const PackManager &packManager, std::string topping) {
        std::string output = "";
        int series = packManager.GetSeries();
        int parallel = packManager.GetParallel();

        std::string dummyline = std::format("| {:<25}", "cell " + std::to_string(0)) + CellOutput(packManager, 0);
        int len = dummyline.length();
        std::string dashedLine(len, '-');
        std::string topLine = std::format("| {:^{}} |", topping, len - 4);

        std::cout << dashedLine << std::endl;
        std::cout << topLine << std::endl;
        std::cout << dashedLine << std::endl;
        for (int i = 0; i < parallel; i++) {
            std::string line = std::format("| {:<25}", "cell " + std::to_string(i)) + CellOutput(packManager, i);
            std::cout << line << std::endl;
        }
        std::cout << dashedLine << std::endl;
        std::string allCapacitiesLine = std::format("| {:<25}", "Total capacities:") + GetCapacitiesOutput(packManager);
        std::cout << allCapacitiesLine << std::endl;
        std::cout << dashedLine << std::endl;

        int maxCapacity = packManager.MaxCapacity();
        int minCapacity = packManager.MinCapacity();
        double averageCapacity = packManager.CalculateAverageCapacity();
        double capacityVariancePercentage = packManager.CalculateCapacityVariancePercentage();
        double totalPackEnergy = packManager.CalculateTotalPackEnergy();
        double totalResistance = packManager.GetTotalResistance();
        double maxResistance = packManager.GetMaxResistance();
        double minResistance = packManager.GetMinResistance();
        double averageResistance = packManager.GetAverageResistance();
        double resistanceVariancePercentage = packManager.GetResistanceVariancePercentage();

        SummaryOutput(dashedLine, maxCapacity, minCapacity, averageCapacity, capacityVariancePercentage, totalPackEnergy,
                      totalResistance, maxResistance, minResistance, averageResistance, resistanceVariancePercentage);
    }

    // Detailed output of all cells in the pack, showing full information for each cell and summary statistics for each parallel group.
    void DetailedCellOutput(const PackManager &packManager, const std::string &topping) {
        std::string output = "";
        int series = packManager.GetSeries();
        int parallel = packManager.GetParallel();

        int totalLen = 120;
        std::string dashedLine(totalLen, '-');
        std::string topLine = std::format("| {:^{}} |", topping, totalLen - 4);

        std::cout << dashedLine << std::endl;
        std::cout << topLine << std::endl;
        std::cout << dashedLine << std::endl << std::endl;

        for (int i = 0; i < series; i++) {
            OutputCellParallelBlock(packManager, totalLen, i);
        }
    }

    // Prompts the user for an integer input with a given prompt message and returns the valid integer.
    std::string GetUserStringInput(const std::string &prompt, const std::string &allowedCharacters) {
        return GetUserString(prompt, allowedCharacters);
    }

    // Prompts the user for a double input with a given prompt message and returns the valid double.
    double GetUserDoubleInput(const std::string &prompt) {
        return GetUserDouble(prompt);
    }

private:
    std::string sourceFilePath; // Path to the source CSV file for reading battery data.
    std::string destinationFilePath; // Path to the destination file for writing output data.

    // Prompts the user for an integer input with a given prompt message and returns the valid integer.
    int GetUserInt(const std::string prompt) {
        int value = 0;
        while (true) {
            std::cout << prompt;
            if (std::cin >> value && value > 0) return value;
            else {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Wrong input! ";
            }
        }
        return value;
    }

    // Prompts the user for a string input with a given prompt message and allowed characters, and returns the valid string.
    std::string GetUserString(const std::string prompt, const std::string allowedCharacters) {
        while (true) {
            std::cout << prompt;
            if (std::cin) {
                std::string input;
                std::cin >> input;
                bool isValid = true;
                for (char c: input) {
                    if (allowedCharacters.find(c) == std::string::npos) {
                        isValid = false;
                        break;
                    }
                }
                if (isValid) return input;
            }
        }
    }

    // Prompts the user for a double input with a given prompt message and returns the valid double.
    double GetUserDouble(const std::string prompt) {
        double value = 0;
        while (true) {
            std::cout << prompt;
            if (std::cin >> value) {
                return value;
            } else {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Wrong input! ";
            }
        }
    }

    // Returns a formatted string representing the capacities of all cells in a specific parallel group (selectedCell) of the pack.
    std::string CellOutput(const PackManager &packManager, int selectedCell) {
        std::string output = "|";
        for (int i = 0; i < packManager.GetSeries(); i++) {
            Battery h = packManager.GetCell(i, selectedCell);
            output += std::format(" {:<6}|", h.GetCapacity());
        }
        return output;
    }

    // Returns a formatted string representing the total capacities of all parallel groups in the pack.
    std::string GetCapacitiesOutput(const PackManager &packManager) {
        std::string output = "|";
        for (int i = 0; i < packManager.GetSeries(); i++) {
            output += std::format(" {:<6}|", packManager.GetIndexParallelCapacity(i));
        }
        return output;
    }

    // Outputs detailed information for a specific parallel group (selectedBlock) of the pack, including each cell's status and summary statistics.
    void OutputCellParallelBlock(const PackManager &packManager, int toppingLen, int selectedBlock) {
        std::string dashedLine(toppingLen, '-');
        std::string topLine = std::format("| {:^{}} |", "Block " + std::to_string(selectedBlock), toppingLen - 4);

        std::cout << std::endl;
        std::cout << dashedLine << std::endl;
        std::cout << topLine << std::endl;
        std::cout << dashedLine << std::endl;

        std::string rawHeader = std::format("{:<17} | {:<17} | {:<17} | {:<17} | {}", "Battery ID", "Capacity (mAh)",
                                            "Internal Resistance (mΩ)", "Manufacturer", "Condition");
        std::string headerLine = std::format("| {:<{}} |", rawHeader, toppingLen - 4);
        std::cout << headerLine << std::endl;
        std::cout << dashedLine << std::endl;

        for (int i = 0; i < packManager.GetParallel(); i++) {
            Battery h = packManager.GetCell(selectedBlock, i);
            std::string rowLine = std::format("| {:<{}} |", h.PrintStatus(), toppingLen - 4);
            std::cout << rowLine << std::endl;
        }
        std::cout << dashedLine << std::endl;

        int maxCapacity = packManager.GetIndexParallelMaxCapacity(selectedBlock);
        int minCapacity = packManager.GetIndexParallelMinCapacity(selectedBlock);
        double averageCapacity = packManager.GetIndexParallelAverageCapacity(selectedBlock);
        double variancePercentage = packManager.GetIndexParallelVariancePercentage(selectedBlock);
        double totalPackEnergy = packManager.GetIndexParallelTotalPackVoltage(selectedBlock);

        double totalResistance = packManager.GetIndexParallelTotalResistance(selectedBlock);
        double maxResistance = packManager.GetIndexParallelMaxResistance(selectedBlock);
        double minResistance = packManager.GetIndexParallelMinResistance(selectedBlock);
        double averageResistance = packManager.GetIndexParallelAverageResistance(selectedBlock);
        double resistanceVariancePercentage = packManager.GetIndexParallelResistanceVariancePercentage(selectedBlock);

        SummaryOutput(dashedLine, maxCapacity, minCapacity, averageCapacity, variancePercentage, totalPackEnergy,
                      totalResistance, maxResistance, minResistance, averageResistance, resistanceVariancePercentage);
    }

    // Outputs a summary of the parallel group in a compact 2-column format.
    void SummaryOutput(const std::string &dashedLine, int maxCapacity, int minCapacity,
                       double averageCapacity, double variancePercentage, double totalPackEnergy,
                       double totalResistance,
                       double maxResistance, double minResistance, double averageResistance,
                       double resistanceVariancePercentage) {
        int len = dashedLine.length();
        int leftColWidth = (len - 7) / 2;
        int rightColWidth = len - 7 - leftColWidth;

        auto makeRow = [&](const std::string &col1, const std::string &col2) {
            return std::format("| {:<{}} | {:<{}} |", col1, leftColWidth, col2, rightColWidth);
        };

        auto formatCol = [](const std::string &label, const std::string &value) {
            return std::format("{:<22} {}", label, value);
        };

        std::string line1 = std::format("| {:^{}} |", "Summary", len - 4);

        std::string line2 = makeRow(
            formatCol("Max capacity:", std::to_string(maxCapacity) + " mAh"),
            formatCol("Min capacity:", std::to_string(minCapacity) + " mAh")
        );
        std::string line3 = makeRow(
            formatCol("Average capacity:", std::format("{:.2f} mAh", averageCapacity)),
            formatCol("Max difference:", std::to_string(maxCapacity - minCapacity) + " mAh")
        );

        std::string line4 = makeRow(
            formatCol("Max resistance:", std::format("{:.2f} mΩ", maxResistance)),
            formatCol("Min resistance:", std::format("{:.2f} mΩ", minResistance))
        );
        std::string line5 = makeRow(
            formatCol("Average resistance:", std::format("{:.2f} mΩ", averageResistance)),
            formatCol("Total resistance:", std::format("{:.2f} mΩ", totalResistance))
        );

        std::string line6 = makeRow(formatCol("Total energy:", std::format("{:.2f} Wh", totalPackEnergy)),"");

        std::string line7 = makeRow(
            formatCol("Capacity variance:", std::format("{:.2f}%", variancePercentage)),
            formatCol("Resistance variance:", std::format("{:.2f}%", resistanceVariancePercentage))
        );

        // Output block
        std::cout << line1 << '\n'
                << dashedLine << '\n'
                << line2 << '\n'
                << line3 << '\n'
                << line4 << '\n'
                << line5 << '\n'
                << line6 << '\n'
                << line7 << '\n'
                << dashedLine << '\n'
                << std::endl;
    }
};

#endif //C_BATTERYPACKOPTIMIZER_DATAHANDLER_H
