#ifndef C_BATTERYPACKOPTIMIZER_INPUT_AND_OUTPUT_H
#define C_BATTERYPACKOPTIMIZER_INPUT_AND_OUTPUT_H

#include <string>
#include <vector>
#include <sstream>
#include <iomanip>

#include "../Battery.h"

void Read(std::string filename, std::vector<Battery>& batteries);

template <typename T1, typename T2, typename T3>
std::string Formed1(T1 first, T2 second, T3 third) {
    std::ostringstream oss;
    oss << "| " << std::left << std::setw(10) << first
        << " | " << std::left << std::setw(15) << second
        << " | " << std::left << std::setw(20) << third << " |";
    return oss.str();
}


void Output(std::vector<Battery> batteries, std::string topping);

void TakeUserInput(int& series, int& parallel);

void PacksOutput(std::vector<std::vector<Battery>>& batteries,
                 int series,
                 int parallel,
                 std::vector<int>& packCapacities,
                 std::string topping,
                 BiggestCapDifference Difference);
void InputHandling(std::vector<Battery>& batteries, std::string filename);
void ConsoleRead(std::vector<Battery>& batteries);

#endif // C_BATTERYPACKOPTIMIZER_INPUT_AND_OUTPUT_H
