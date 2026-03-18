#ifndef C_BATTERYPACKOPTIMIZER_INPUT_AND_OUTPUT_H
#define C_BATTERYPACKOPTIMIZER_INPUT_AND_OUTPUT_H
#include <string>

#endif //C_BATTERYPACKOPTIMIZER_INPUT_AND_OUTPUT_H

using namespace std;

#include <vector>
#include "../Battery.h"
#include <sstream>
#include <iomanip>

void Read(string filename, vector<Battery>& batteries);

template <typename T1, typename T2, typename T3>
string Formed(T1 first, T2 second, T3 third) {
    ostringstream oss;
    oss << "| " << left << setw(10) << first
            << " | " << left << setw(15) << second
            << " | " << left << setw(20) << third << " |";
    return oss.str();
}

void Output(vector<Battery> batteries, string topping);

void TakeUserInput(int & series, int & parallel);

void PacksOutput(vector<vector<Battery>>& batteries, int series, int parallel,  vector<int>& packCapacities, string topping, BiggestCapDifference Difference);
void InputHandling(vector<Battery>& batteries, string filename);
void ConsoleRead(vector<Battery>& batteries);

