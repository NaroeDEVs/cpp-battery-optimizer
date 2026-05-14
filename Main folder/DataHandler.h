#ifndef C_BATTERYPACKOPTIMIZER_DATAHANDLER_H
#define C_BATTERYPACKOPTIMIZER_DATAHANDLER_H

#include <string>
#include "Battery.h"
#include "BatteryPack.h"
#include <fstream>
#include <sstream>
#include <vector>
#include <iostream>
#include <limits>

class DataHandler {



public:
    DataHandler(std::string sourceFilePath, std::string destinationFilePath) :
        sourceFilePath(sourceFilePath),
        destinationFilePath(destinationFilePath) {}

    void ReadData(BatteryPack& batteryPack) { Read(batteryPack); }
    int GetUserSeries() { return GetUserInt("series: ");}
    int GetUserParallel() { return GetUserInt("parallel: ");}


private:
    std::string sourceFilePath;
    std::string destinationFilePath;

    void Read(BatteryPack & batteryPack) {
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

            // These three are mandatory
            if (!std::getline(ss, idStr, ';')) continue;
            if (!std::getline(ss, capStr, ';')) continue;
            if (!std::getline(ss, manStr, ';')) continue;

            // For the last one, just try to read it.
            // If it's not there, condStr stays empty/default.
            std::getline(ss, condStr, ';');

            int id = std::stoi(idStr);
            int capacity = std::stoi(capStr);

            Battery battery(id, capacity, manStr, condStr.empty() ? "N/A" : condStr);
            batteryPack.addCell(battery);
        }
    }

    /**
     * @brief Gets user input for series or parallel connection and validates it to ensure it's a positive integer.
     * @param prompt Output of the command type
     * @return integer of the selected type
     */
    int GetUserInt(std::string prompt) {
        int value = 0;
        while (true) {
            std::cout << prompt;
            if (std::cin>>value && value >0 ) {
                return value;
            }
            else {
                std::cin.clear(); // Clear the error
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Clear the error state and ignore the rest of the input
                std::cout << "Wrong input! ";
            }
        }
        return value;
    }

};


#endif //C_BATTERYPACKOPTIMIZER_DATAHANDLER_H