#include "Input and Output.h"
#include <algorithm>
#include "Capacity algorithms.h"
#include <vector>
#include <iomanip>
#include <sstream>
#include <iostream>
#include <fstream>
#include <numeric>

using namespace std;

void Read(string filename, vector<Battery>& batteries) {
    ifstream reader(filename);
    if (!reader.is_open()) {
        cout << "Error opening file" << endl;
        return;
    }

    string line;
    getline(reader, line);
    string idString, mahString, manufacturerString;

    while (getline(reader, idString, ';')) {
        getline(reader, mahString, ';');
        getline(reader, manufacturerString);

        Battery tempBattery;
        tempBattery.id = stoi(idString);
        tempBattery.capacity = stoi(mahString);
        tempBattery.manufacturer = manufacturerString;
        batteries.push_back(tempBattery);
    }
}

void ConsoleRead(vector<Battery>& batteries) {
    int capacity;
    string manufacturer;

    int currentCount = batteries.size();
    int id = currentCount;

    cout << "Enter battery details (capacity manufacturer), or -1 to stop:" << endl;
    while (true) {
        cout << "Battery " << id << ": ";

        while (!(cin >> capacity)) {
            cout << "[Invalid input. Try again] Battery "<<id<<": ";
            cin.clear();
            cin.ignore(10000, '\n');
        }

        if (capacity == -1) break;

        cin>>manufacturer;

        Battery tempBattery;
        tempBattery.id = id;
        tempBattery.capacity = capacity;
        tempBattery.manufacturer = manufacturer;
        batteries.push_back(tempBattery);
        id++;
    }
}

void InputHandling(vector<Battery>& batteries, string filename) {
    string choice;

    while (true) {
        cout<<"Do you want to read from a file or enter data manually or first from file then manually? (file/manual/both) : ";
        cin >> choice;
        if (choice =="file") {
            Read(filename, batteries);
            break;
        }
        else if (choice == "manual") {
            ConsoleRead(batteries);
            break;
        }
        else if (choice == "both") {
            Read(filename, batteries);
            cout << "File data loaded. Now you can enter additional battery details manually." << endl;
            ConsoleRead(batteries);
            break;
        }
        else {
            cout << "Invalid choice. Please enter 'file', 'manual', or 'both'." << endl;
        }
    }

}

void Output(vector<Battery> batteries, string topping) {

    string dottedLine(55, '=');
    cout << dottedLine << endl;
    int mid = (55-topping.length())/2-1;
    string spaces(mid, ' ');

    string lineTop ="="+ spaces + topping + spaces + "=";
    cout << lineTop << endl;
    cout << dottedLine << endl;

    for (int i = 0; i < batteries.size(); i++) {
        cout<<Formed1(batteries[i].id, batteries[i].capacity, batteries[i].manufacturer)<<endl;
    }
    cout << dottedLine << endl;
    cout<<endl;
    cout<<endl;
}

void TakeUserInput(int & series, int & parallel) {
    cout<< "Enter series: ";
    while (!(cin >> series) || series <= 0) {
        cout << "[Invalid input. Try again] Enter series: ";
        cin.clear();
        cin.ignore(10000, '\n');
    }

    cout<< "Enter parallel: ";
    while (!(cin >> parallel) || parallel <= 0) {
        cout << "[Invalid input. Try again] Enter parallel: ";
        cin.clear();
        cin.ignore(10000, '\n');
    }
}

string FormedPreString(string input, int lent) {
    string line = "| " + input;
    return "| " + input + string(lent - input.length()-2, ' ');
}

void NumbersOutput(vector<int> & numbers, string preString, string preString2, int colWith, bool withoutPrestring) {
    cout<<FormedPreString(preString, 30);
    for (int i=0; i<numbers.size(); i++) {
        if (withoutPrestring) {
            cout << "| " << left << setw(colWith) << numbers[i];
        }
        else {
            string headerTitle = preString2 + to_string(i+1);
            cout << "| " << left << setw(colWith) << headerTitle;
        }
    }
    cout<<"|" << endl;
}

void BatteryNumbersOutputFormedLine(vector<Battery> & batteries, string preString, int colWith) {
    cout<<FormedPreString(preString, 30);

    for (int i=0; i<batteries.size(); i++) {
        cout<<"| " << left << setw(colWith) << batteries[i].capacity;
    }

    cout<<"|" << endl;
}

void BatteriesOutput(vector<vector<Battery>> & batteries, string preString, int colWidth, int parallel, int series) {
    for (int i=0; i<parallel; i++) {
        vector<Battery> rowBatteries(series);
        for (int c=0; c<series; c++) {
            if (i < batteries[c].size()) {
                rowBatteries[c] = batteries[c][i];
            }
            else {
                rowBatteries[c] = {0, 0, "N/A"};
            }
        }
        BatteryNumbersOutputFormedLine(rowBatteries, preString+to_string(i+1)+":", colWidth);
    }
}

void DivergenceOutput(vector<int> & packCapacities, string preString, int colWidth) {

    cout << left << setw(30) << preString;
    int average = AveragePacksCapacity(packCapacities);

    for (int c = 0; c<packCapacities.size(); c++) {
        int divergence = packCapacities[c] - average;
        string headerTitle;

        if (divergence > 0) {
            headerTitle = "+" + to_string(divergence);
        }
        else headerTitle = to_string(divergence);

        cout << "| " << left << setw(colWidth) << headerTitle;
    }
    cout << "|" << endl;
}

void DeviationOutput(vector<vector<Battery>> & batteries, string preString, int colWidth) {
    cout << left << setw(30) << preString;
    for (int i=0; i<batteries.size(); i++) {
        int deviation = PackDeviation(batteries[i]);
        string headerTitle = to_string(deviation);
        cout << "| " << left << setw(colWidth) << headerTitle;
    }
    cout << "|" << endl;
}



void PacksOutput1(vector<vector<Battery>>& batteries, int series, int parallel, vector<int>& packCapacities, string topping, BiggestCapDifference difference) {
    int colWidth = 9;
    int length = series * (colWidth + 2) + 31;

    string dottedLine(length, '=');

    int mid = (length-topping.length())/2-1;
    string spaces(mid, ' ');
    string lineTop = "="+ spaces + topping + spaces + " =";

    vector<int> Ranging(series);
    iota(Ranging.begin(), Ranging.end(), 1);

    string CapacityDifferenceLine = "| Max diff: S" + to_string(difference.biggest+1)+" and S" + to_string(difference.smallest+1)+": "+to_string(difference.difference) + " mAh";
    int spaces2 = length - (int)CapacityDifferenceLine.length()-1;
    string spaces2s(spaces2, ' ');


    cout << dottedLine << endl;
    cout << lineTop << endl;

    cout << dottedLine << endl;
    NumbersOutput(Ranging, "Pack:", "S", colWidth, false);

    cout<<dottedLine << endl;
    BatteriesOutput(batteries, "Battery ", colWidth, parallel, series);

    cout << dottedLine << endl;
    NumbersOutput(packCapacities, "Total Capacity:", "S", colWidth, true);

    cout<< dottedLine << endl;
    DivergenceOutput(packCapacities, "Divergence from average:", colWidth);

    cout<<dottedLine << endl;
    DeviationOutput(batteries, "Deviation within pack:", colWidth);

    cout << dottedLine << endl;
    cout << CapacityDifferenceLine <<spaces2s<<"|"<< endl;
    cout<<dottedLine << endl;
}


void Liner(vector<string>& words, vector<int>& spaces) {
    string outputLine = "";
    for (int i=0; i<words.size(); i++) {
        string word = words[i];
        int space = spaces[i];
        int emptySpace = space - 1 - word.length();
        if (i == words.size() - 1) {
            emptySpace -= 1;
        }
        int leftPad = emptySpace / 2;
        int rightPad = emptySpace - leftPad;

        string spacesBefore(leftPad, ' ');
        string spacesAfter(rightPad, ' ');

        outputLine += "|" + spacesBefore + word + spacesAfter;
    }
    outputLine += "|";
    cout << outputLine << endl;
}


void SinglePackBoard(vector<Battery>& batteries, int series, int parallel, int number, vector<int>& packCapacities, BiggestCapDifference difference) {
    string tinierDottedLine(120, '-');
    cout << tinierDottedLine << endl;

    // Header
    vector<string> words = {"Pack " + to_string(number), "Capacities"};
    vector<int> spaces = {45, 75};
    Liner(words, spaces);
    cout << tinierDottedLine << endl;

    // Columns
    words = {"Cells", "ID", "Capacity", "Manufacturer"};
    spaces = {45, 25, 25, 25};
    Liner(words, spaces);
    cout << tinierDottedLine << endl;

    // Print each cell in the pack
    for (int i=0; i<batteries.size(); i++) {
        Battery battery = batteries[i];
        words = {"Cell " + to_string(i+1), to_string(battery.id), to_string(battery.capacity), battery.manufacturer};
        spaces = {45, 25, 25, 25};
        Liner(words, spaces);
    }
    cout << tinierDottedLine << endl;

    words = {"Total Capacity:", to_string(packCapacities[number-1])};
    spaces = {45, 75}; // Resetting to 2 columns
    Liner(words, spaces);
    cout << tinierDottedLine << endl;

    int average = AveragePacksCapacity(packCapacities);
    int divergence = packCapacities[number-1] - average;
    string divString = (divergence > 0 ? "+" : "") + to_string(divergence);

    words = {"Divergence from average:", divString};
    Liner(words, spaces);
    cout << tinierDottedLine << endl;

    int deviation = PackDeviation(batteries);
    words = {"Deviation within pack:", to_string(deviation)};
    Liner(words, spaces);
    cout << tinierDottedLine << endl;

    cout<<endl;
    cout<<endl;
}

void PacksOutput2(vector<vector<Battery>>& batteries, int series, int parallel, vector<int>& packCapacities, string topping, BiggestCapDifference difference) {
    int colWidth = 25;
    int length = 120;

    string dottedLine(length, '=');
    string tinierDottedLine(length, '-');

    int mid = (length-topping.length())/2;
    string spaces(mid, ' ');
    string lineTop = "="+ spaces + topping + spaces + "=";

    vector<int> Ranging(series);
    iota(Ranging.begin(), Ranging.end(), 1);

    cout << dottedLine << endl;
    cout << lineTop << endl;
    cout << dottedLine << endl;
    cout << endl;



    for (int i=0; i<batteries.size(); i++) {
        SinglePackBoard(batteries[i], series, parallel, i+1, packCapacities, difference);
    }
}

string InputHandling2() {
    string result="";

    cout<<"Detailed or compact output? (detailed/compact): ";
    while (true) {
        string choice;
        cin >> choice;
        if (choice == "detailed") {
            result = "detailed";
            break;
        }
        else if (choice == "compact") {
            result = "compact";
            break;
        }
        else {
            cout << "Invalid choice. Please enter 'detailed' or 'compact': ";
        }
    }

    return result;
}

string GradingResult(int percentage) {
    vector<string> Results = {"Terrible", "Bad", "Average", "Good", "Great"};
    string result;
    switch (percentage) {
        case 0 ... 20:
            result = Results[0];
        case 21 ... 40:
            result = Results[1];
        case 41 ... 60:
            result = Results[2];
        case 61 ... 80:
            result = Results[3];
        case 81 ... 100:
            result = Results[4];
        default:
            result = "Invalidpercentage";
    }
    return "Pack overall quality: " + result + " (" + to_string(percentage) + "%)";
}

