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
    DataHandler(std::string sourceFilePath, std::string destinationFilePath) :
        sourceFilePath(sourceFilePath),
        destinationFilePath(destinationFilePath) {}

    void ReadData(BatteryInventory& batteryPack) {
            std::ifstream reader(sourceFilePath);
            if (!reader.is_open()) {
                std::cout << "Error opening file: " << sourceFilePath << std::endl;
                return;
            }

            std::string line;
            std::getline(reader, line);
            while (std::getline(reader, line)) {
                std::istringstream ss(line);
                std::string idStr, capStr, manStr, condStr;

                if (!std::getline(ss, idStr, ';')) continue;
                if (!std::getline(ss, capStr, ';')) continue;
                if (!std::getline(ss, manStr, ';')) continue;
                std::getline(ss, condStr, ';');

                int id = std::stoi(idStr);
                int capacity = std::stoi(capStr);

                Battery battery(id, capacity, manStr, condStr.empty() ? "N/A" : condStr);
                batteryPack.AddCell(battery);
            }

    }
    int GetUserSeries() { return GetUserInt("series: ");}
    int GetUserParallel() { return GetUserInt("parallel: ");}
    int GetUserLevelOfOptimization() { return GetUserInt("Level of optimization: ");}

    void CompactCellOutput(const PackManager& packManager, std::string topping) {
        std::string output = "";
        int series = packManager.GetSeries();
        int parallel = packManager.GetParallel();

        std::string dummyline = std::format("| {:<25}", "cell " + std::to_string(0)) + CellOutput(packManager, 0);
        int len = dummyline.length();
        std::string dashedLine(len, '-');
        std::string topLine = std::format("| {:^{}} |", topping, len - 4);

        std::cout<<dashedLine<<std::endl;
        std::cout<<topLine<<std::endl;
        std::cout<<dashedLine<<std::endl;
        for (int i=0; i<parallel; i++) {
            std::string line = std::format("| {:<25}", "cell " + std::to_string(i)) + CellOutput(packManager, i);
            std::cout<<line<<std::endl;
        }

        std::cout<<dashedLine<<std::endl;
        std::string allCapacitiesLine = std::format("| {:<25}", "Total capacities:") + GetCapacitiesOutput(packManager);
        std::cout<<allCapacitiesLine<<std::endl;
        std::cout<<dashedLine<<std::endl;


        int maxCapacity = packManager.MaxCapacity();
        int minCapacity = packManager.MinCapacity();
        std::string wordSummary = "Summary";
        std::string line1 = std::format("| {:^{}} |", wordSummary, len - 4);
        std::string line2 = std::format("| {0:<25}| {1:<{2}} |", "Max capacity:", maxCapacity, len - 25 - 6);
        std::string line3 = std::format("| {0:<25}| {1:<{2}} |", "Min capacity:", minCapacity, len - 25 - 6);
        std::string line4 = std::format("| {0:<25}| {1:<{2}} |", "Biggest Mah difference:", maxCapacity - minCapacity, len - 25 - 6);
        std::string line5 = std::format("| {0:<25}| {1:<{2}.2f} |", "Average capacity:", packManager.CalculateAverageCapacity(), len - 25 - 6);
        std::string line6 = std::format("| {0:<25}| {1:<{2}} |", "Variance: ",std::format("{0:.2f}%", packManager.CalculateVariancePercentage()), len - 25 - 6);;

        std::cout<<line1<<std::endl;
        std::cout<<dashedLine<<std::endl;
        std::cout<<line2<<std::endl;
        std::cout<<line3<<std::endl;
        std::cout<<line4<<std::endl;
        std::cout<<line5<<std::endl;
        std::cout<<line6<<std::endl;
        std::cout<<dashedLine<<std::endl;
        std::cout<<std::endl;
    }


private:
    std::string sourceFilePath;
    std::string destinationFilePath;

    int GetUserInt(const std::string prompt) {
        int value = 0;
        while (true) {
            std::cout << prompt;
            if (std::cin>>value && value >0 ) {
                return value;
            }
            else {
                std::cin.clear(); // Clear the error
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Wrong input! ";
            }
        }
        return value;
    }

    std::string CellOutput(const PackManager& packManager, int selectedCell) {
        std::string output= "|";
        for (int i=0; i<packManager.GetSeries(); i++) {
            Battery h = packManager.TakeBattery(i, selectedCell);
            output += std::format(" {:<6}|", h.GetCapacity());
        }
        return output;
    }

    std::string GetCapacitiesOutput(const PackManager& packManager) {
        std::string output = "|";
        for (int i=0; i<packManager.GetSeries(); i++) {
            output += std::format(" {:<6}|", packManager.GetIndexParallelCapacity(i));
        }
        return output;
    }
};


#endif //C_BATTERYPACKOPTIMIZER_DATAHANDLER_H