#include <iostream>
#include <vector>
#include <fstream>
#include <iomanip>
#include <cmath>
#include <sstream>
#include "Battery.h"
#include "BatteryInventory.h"
#include "DataHandler.h"
#include "ParallelGroup.h"
#include "PackManager.h"

using namespace std;

int main() {
    DataHandler dataHandler("Batteries.csv", "Output.csv");

    const string file = "Batteries.csv";

    BatteryInventory AllBateries;

    dataHandler.ReadData(AllBateries);

    int series = dataHandler.GetUserSeries();
    int parallel = dataHandler.GetUserParallel();

    PackManager allPacks;
    allPacks.SetSize(series, parallel);
    allPacks.Pack(AllBateries);

    return 0;
}