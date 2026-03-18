#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <iomanip>
#include <ctime>
#include <cmath>
#include <sstream>
#include <Battery.h>
#include "Functions/Capacity algorithms.h"
#include "Functions/Input and Output.h"
#include "Functions/Packing Algorithms.h"

using namespace std;

int main() {

    const string file = "Batteries.csv";
    vector<Battery> initialBatteryPack;

    InputHandling(initialBatteryPack, file);
    Sort(initialBatteryPack);

    int series;
    int parallel;
    TakeUserInput(series, parallel);

    if (series*parallel > initialBatteryPack.size()) {
        cout << "Not enough batteries in the pack to fill the requested configuration." << endl;
        return 0;
    }
    else if (series <= 0 || parallel <= 0) {
        cout << "Series and parallel values must be greater than 0." << endl;
        return 0;
    }

    vector<Battery> SelectedBatteries;

    BestBatteries(initialBatteryPack, series, parallel, SelectedBatteries);

    vector<vector<Battery>> BatteriesPack;
    vector<int> PackCapacity;
    vector<int> NumberOfBatteries;

    BasePackingAlgorithm(SelectedBatteries, series, parallel, BatteriesPack, PackCapacity, NumberOfBatteries);

    BiggestCapDifference Difference;
    BiggestCapacityDifference(PackCapacity, Difference);

    PacksOutput(BatteriesPack, series, parallel, PackCapacity, "Pakuotės", Difference);
    return 0;
}