#include <iostream>
#include <vector>
#include <fstream>
#include <iomanip>
#include <cmath>
#include <sstream>
#include "Battery.h"
#include "BatteryPack.h"
#include "DataHandler.h"

using namespace std;

int main() {
    DataHandler dataHandler("Batteries.csv", "Output.csv");

    const string file = "Batteries.csv";

    BatteryPack batteryPack;

    dataHandler.Read(batteryPack);
    int capacity = batteryPack.GetTotalCapacity();
    cout << capacity << endl;

    return 0;
}