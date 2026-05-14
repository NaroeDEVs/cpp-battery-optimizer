#ifndef C_BATTERYPACKOPTIMIZER_DATAHANDLER_H
#define C_BATTERYPACKOPTIMIZER_DATAHANDLER_H

#include <string>
#include "Battery.h"
#include "BatteryPack.h"
#include <fstream>
#include <sstream>
#include <vector>

class DataHandler {

private:
    std::string sourceFilePath;
    std::string destinationFilePath;

public:
    DataHandler(std::string sourceFilePath, std::string destinationFilePath) :
        sourceFilePath(sourceFilePath),
        destinationFilePath(destinationFilePath) {}

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
};


#endif //C_BATTERYPACKOPTIMIZER_DATAHANDLER_H