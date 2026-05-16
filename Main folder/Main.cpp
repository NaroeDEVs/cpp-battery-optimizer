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

    BatteryPack AllBateries;

    dataHandler.ReadData(AllBateries);

    int series = dataHandler.GetUserSeries();
    int parallel = dataHandler.GetUserParallel();
    cout<<"series: "<<series<<" parallel: "<<parallel<<endl;;
    AllBateries.Sort();
    AllBateries.OutputAll();

    return 0;
}