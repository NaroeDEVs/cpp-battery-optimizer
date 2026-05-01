#ifndef C_BATTERYPACKOPTIMIZER_INPUT_AND_OUTPUT_H
#define C_BATTERYPACKOPTIMIZER_INPUT_AND_OUTPUT_H

#include <string>
#include <vector>
#include <sstream>
#include <iomanip>

#include "../Battery.h"

void Read(const std::string & filename,
                std::vector<Battery>& batteries);

void ConsoleRead(std::vector<Battery>& batteries);

void InputHandling(std::vector<Battery>& batteries,
                const std::string & filename);

void TakeUserInput(int& series, int& parallel);

void PacksOutput1(std::vector<std::vector<Battery>>& batteries,
                 int series,
                 int parallel,
                 std::vector<int>& packCapacities,
                 std::string topping,
                 BiggestCapDifference Difference);

void PacksOutput2(std::vector<std::vector<Battery>>& batteries,
                 int series,
                 int parallel,
                 std::vector<int>& packCapacities,
                 std::string topping,
                 BiggestCapDifference Difference);



std::string InputHandling2();



#endif // C_BATTERYPACKOPTIMIZER_INPUT_AND_OUTPUT_H
