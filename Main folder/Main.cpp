#include <iostream>
#include <vector>
#include <fstream>
#include <iomanip>
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

    string outputChoice = InputHandling2();
    if (outputChoice == "compact") {
        PacksOutput1(BatteriesPack, series, parallel, PackCapacity, "Pakuotės", Difference);
    }
    else if (outputChoice == "detailed") {
        PacksOutput2(BatteriesPack, series, parallel, PackCapacity, "Pakuotės", Difference);
    }

    //Add pack quality measurer (percentage deviation from average), exampl: Great, Good, Average, Bad, Terrible
    //to do later

    cout<<GradingResult(20);
    return 0;
}