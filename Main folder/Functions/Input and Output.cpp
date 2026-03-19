#include "Input and Output.h"
#include "Capacity algorithms.h"
#include <vector>
#include <iomanip>
#include <sstream>
#include <iostream>
#include <fstream>

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
    cout<<"Do you want to read from a file or enter data manually or first from file then manually? (file/manual/both) : ";
    string choice;
    cin >> choice;
    if (choice =="file") {
        Read(filename, batteries);
    }
    else if (choice == "manual") {
        ConsoleRead(batteries);
    }
    else if (choice == "both") {
        Read(filename, batteries);
        cout << "File data loaded. Now you can enter additional battery details manually." << endl;
        ConsoleRead(batteries);
    }
    else {
        cout << "Invalid choice. Please enter 'file', 'manual', or 'both'." << endl;
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
    cin >> series;
    cout<< "Enter parallel: ";
    cin >> parallel;
}

void PacksOutput(vector<vector<Battery>>& batteries, int series, int parallel, vector<int>& packCapacities, string topping, BiggestCapDifference difference) {
    int colWidth = 9;
    int length = series * (colWidth + 2) + 31;

    string dottedLine(length, '=');

    cout << dottedLine << endl;

    int mid = (length-topping.length())/2-1;
    string spaces(mid, ' ');
    string lineTop = "="+ spaces + topping + spaces + " =";

    cout << lineTop << endl;
    cout << dottedLine << endl;

    cout <<left<< setw(30) << "| Pack:";

    for (int i = 0; i < series; i++) {
        string headerTitle = "S" + to_string(i+1);

    cout<<"| " << left << setw(colWidth) << headerTitle;
    }
    cout << "|" << endl;
    cout << dottedLine << endl;

    for (int i = 0; i < parallel; i++) {

        string label = "| " + to_string(i + 1) + ". cell (mAh) :";
        cout << left << setw(30) << label;

        for (int j=0; j<series; j++) {
            string headerTitle = to_string(batteries[j][i].capacity);
            cout << "| " << left << setw(colWidth) << headerTitle;
        }
        cout << "|" << endl;
    }
    cout<< dottedLine << endl;

    string label = "| Total capacity (mAh):";
    cout << left << setw(30) << label;

    for (int c = 0; c<packCapacities.size(); c++) {
        string headerTitle = to_string(packCapacities[c]);
        cout << "| " << left << setw(colWidth) << headerTitle;
    }
    cout << "|" << endl;
    cout << dottedLine << endl;


    string label1 = "| Divergence from avg: ";
    cout << left << setw(30) << label1;
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
    cout<<dottedLine << endl;


    string label2 = "| Deviation within pack: ";
    cout << left << setw(30) << label2;
    for (int i=0; i<batteries.size(); i++) {
        int deviation = PackDeviation(batteries[i]);
        string headerTitle = to_string(deviation);
        cout << "| " << left << setw(colWidth) << headerTitle;
    }
    cout << "|" << endl;
    cout << dottedLine << endl;

    string CapacityDifferenceLine = "| Max diff: S" + to_string(difference.smallest+1)+" and S" + to_string(difference.biggest+1)+": "+to_string(difference.difference) + " mAh";
    int spaces2 = length - (int)CapacityDifferenceLine.length()-1;
    string spaces2s(spaces2, ' ');
    cout << CapacityDifferenceLine <<spaces2s<<"|"<< endl;
    cout<<dottedLine << endl;
}

