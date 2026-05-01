#include "Input and Output.h"
#include <algorithm>
#include "Capacity algorithms.h"
#include <vector>
#include <iomanip>
#include <sstream>
#include <iostream>
#include <fstream>
#include <numeric>

void Read(const std::string & filename, std::vector<Battery>& batteries) {
    std::ifstream reader(filename);
    if (!reader.is_open()) {
        std::cout << "Error opening file" << std::endl;
        return;
    }

    std::string line;
    std::getline(reader, line);
    std::string idString, mahString, manufacturerString;

    while (std::getline(reader, idString, ';')) {
        std::getline(reader, mahString, ';');
        std::getline(reader, manufacturerString);

        Battery tempBattery;
        tempBattery.id = std::stoi(idString);
        tempBattery.capacity = std::stoi(mahString);
        tempBattery.manufacturer = manufacturerString;
        batteries.push_back(tempBattery);
    }
}

void ConsoleRead(std::vector<Battery>& batteries) {
    int capacity;
    std::string manufacturer;

    int currentCount = batteries.size();
    int id = currentCount;

    std::cout << "Enter battery details (capacity manufacturer), or -1 to stop:" << std::endl;
    while (true) {
        std::cout << "Battery " << id << ": ";

        while (!(std::cin >> capacity)) {
            std::cout << "[Invalid input. Try again] Battery "<<id<<": ";
            std::cin.clear();
            std::cin.ignore(10000, '\n');
        }

        if (capacity == -1) break;

        std::cin>>manufacturer;

        Battery tempBattery;
        tempBattery.id = id;
        tempBattery.capacity = capacity;
        tempBattery.manufacturer = manufacturer;
        batteries.push_back(tempBattery);
        id++;
    }
}

void InputHandling(std::vector<Battery>& batteries, const std::string & filename)
    {
        std::string choice;

        while (true) {
            std::cout<<"Do you want to read from a file or enter data manually or first from file then manually? (file/manual/both) : ";
            std::cin >> choice;
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
                std::cout << "File data loaded. Now you can enter additional battery details manually." << std::endl;
                ConsoleRead(batteries);
                break;
            }
            else {
                std::cout << "Invalid choice. Please enter 'file', 'manual', or 'both'." << std::endl;
            }
        }

    }

    void TakeUserInput(int & series, int & parallel) {
        std::cout<< "Enter series: ";
        while (!(std::cin >> series) || series <= 0) {
            std::cout << "[Invalid input. Try again] Enter series: ";
            std::cin.clear();
            std::cin.ignore(10000, '\n');
        }

        std::cout<< "Enter parallel: ";
        while (!(std::cin >> parallel) || parallel <= 0) {
            std::cout << "[Invalid input. Try again] Enter parallel: ";
            std::cin.clear();
            std::cin.ignore(10000, '\n');
        }
    }

    std::string FormedPreString(std::string input, int lent) {
        std::string line = "| " + input;
        return "| " + input + std::string(lent - input.length()-2, ' ');
    }

    void NumbersOutput(const std::vector<int> & numbers, std::string preString, std::string preString2, int colWith, bool withoutPrestring) {
        std::cout<<FormedPreString(preString, 30);
        for (int i=0; i<numbers.size(); i++) {
            if (withoutPrestring) {
                std::cout << "| " << std::left << std::setw(colWith) << numbers[i];
            }
            else {
                std::string headerTitle = preString2 + std::to_string(i+1);
                std::cout << "| " << std::left << std::setw(colWith) << headerTitle;
            }
        }
        std::cout<<"|" << std::endl;
    }

    void BatteryNumbersOutputFormedLine(std::vector<Battery> & batteries, std::string preString, int colWith) {
        std::cout<<FormedPreString(preString, 30);

        for (int i=0; i<batteries.size(); i++) {
            std::cout<<"| " << std::left << std::setw(colWith) << batteries[i].capacity;
        }

        std::cout<<"|" << std::endl;
    }

    void BatteriesOutput(std::vector<std::vector<Battery>> & batteries, std::string preString, int colWidth, int parallel, int series) {
        for (int i=0; i<parallel; i++) {
            std::vector<Battery> rowBatteries(series);
            for (int c=0; c<series; c++) {
                if (i < batteries[c].size()) {
                    rowBatteries[c] = batteries[c][i];
                }
                else {
                    rowBatteries[c] = {0, 0, "N/A"};
                }
            }
            BatteryNumbersOutputFormedLine(rowBatteries, preString+std::to_string(i+1)+":", colWidth);
        }
    }

    void DivergenceOutput(std::vector<int> & packCapacities, std::string preString, int colWidth) {

        std::cout << std::left << std::setw(30) << preString;
        int average = AveragePacksCapacity(packCapacities);

        for (int c = 0; c<packCapacities.size(); c++) {
            int divergence = packCapacities[c] - average;
            std::string headerTitle;

            if (divergence > 0) {
                headerTitle = "+" + std::to_string(divergence);
            }
            else headerTitle = std::to_string(divergence);

            std::cout << "| " << std::left << std::setw(colWidth) << headerTitle;
        }
        std::cout << "|" << std::endl;
    }

    void DeviationOutput(std::vector<std::vector<Battery>> & batteries, std::string preString, int colWidth) {
        std::cout << std::left << std::setw(30) << preString;
        for (int i=0; i<batteries.size(); i++) {
            int deviation = PackDeviation(batteries[i]);
            std::string headerTitle = std::to_string(deviation);
            std::cout << "| " << std::left << std::setw(colWidth) << headerTitle;
        }
        std::cout << "|" << std::endl;
    }



    void PacksOutput1(std::vector<std::vector<Battery>>& batteries, int series, int parallel, std::vector<int>& packCapacities, std::string topping, BiggestCapDifference difference) {
        int colWidth = 9;
        int length = series * (colWidth + 2) + 31;

        std::string dottedLine(length, '=');

        int mid = (length-topping.length())/2-1;
        std::string spaces(mid, ' ');
        std::string lineTop = "="+ spaces + topping;
        std::string lineTop2 = std::string(length - lineTop.length() - 1, ' ') + "=";
        lineTop += lineTop2;

        std::vector<int> Ranging(series);
        std::iota(Ranging.begin(), Ranging.end(), 1);

        std::string CapacityDifferenceLine = "| Max diff: S" + std::to_string(difference.biggest+1)+" and S" + std::to_string(difference.smallest+1)+": "+std::to_string(difference.difference) + " mAh";
        int spaces2 = length - (int)CapacityDifferenceLine.length()-1;
        std::string spaces2s(spaces2, ' ');


        std::cout << dottedLine << std::endl;
        std::cout << lineTop << std::endl;

        std::cout << dottedLine << std::endl;
        NumbersOutput(Ranging, "Pack:", "S", colWidth, false);

        std::cout<<dottedLine << std::endl;
        BatteriesOutput(batteries, "Battery ", colWidth, parallel, series);

        std::cout << dottedLine << std::endl;
        NumbersOutput(packCapacities, "Total Capacity:", "S", colWidth, true);

        std::cout<< dottedLine << std::endl;
        DivergenceOutput(packCapacities, "Divergence from average:", colWidth);

        std::cout<<dottedLine << std::endl;
        DeviationOutput(batteries, "Deviation within pack:", colWidth);

        std::cout << dottedLine << std::endl;
        std::cout << CapacityDifferenceLine <<spaces2s<<"|"<< std::endl;
        std::cout<<dottedLine << std::endl;
    }


    void Liner(std::vector<std::string>& words, std::vector<int>& spaces) {
        std::string outputLine = "";
        for (int i=0; i<words.size(); i++) {
            std::string word = words[i];
            int space = spaces[i];
            int emptySpace = space - 1 - word.length();
            if (i == words.size() - 1) {
                emptySpace -= 1;
            }
            int leftPad = emptySpace / 2;
            int rightPad = emptySpace - leftPad;

            std::string spacesBefore(leftPad, ' ');
            std::string spacesAfter(rightPad, ' ');

            outputLine += "|" + spacesBefore + word + spacesAfter;
        }
        outputLine += "|";
        std::cout << outputLine << std::endl;
    }


    void SinglePackBoard(std::vector<Battery>& batteries, int series, int parallel, int number, std::vector<int>& packCapacities, BiggestCapDifference difference) {
        std::string tinierDottedLine(120, '-');
        std::cout << tinierDottedLine << std::endl;

        // Header
        std::vector<std::string> words = {"Pack " + std::to_string(number), "Capacities"};
        std::vector<int> spaces = {45, 75};
        Liner(words, spaces);
        std::cout << tinierDottedLine << std::endl;

        // Columns
        words = {"Cells", "ID", "Capacity", "Manufacturer"};
        spaces = {45, 25, 25, 25};
        Liner(words, spaces);
        std::cout << tinierDottedLine << std::endl;

        // Print each cell in the pack
        for (int i=0; i<batteries.size(); i++) {
            Battery battery = batteries[i];
            words = {"Cell " + std::to_string(i+1), std::to_string(battery.id), std::to_string(battery.capacity), battery.manufacturer};
            spaces = {45, 25, 25, 25};
            Liner(words, spaces);
        }
        std::cout << tinierDottedLine << std::endl;

        words = {"Total Capacity:", std::to_string(packCapacities[number-1])};
        spaces = {45, 75}; // Resetting to 2 columns
        Liner(words, spaces);
        std::cout << tinierDottedLine << std::endl;

        int average = AveragePacksCapacity(packCapacities);
        int divergence = packCapacities[number-1] - average;
        std::string divString = (divergence > 0 ? "+" : "") + std::to_string(divergence);

        words = {"Divergence from average:", divString};
        Liner(words, spaces);
        std::cout << tinierDottedLine << std::endl;

        int deviation = PackDeviation(batteries);
        words = {"Deviation within pack:", std::to_string(deviation)};
        Liner(words, spaces);
        std::cout << tinierDottedLine << std::endl;

        std::cout<<std::endl;
        std::cout<<std::endl;
    }

    void PacksOutput2(std::vector<std::vector<Battery>>& batteries, int series, int parallel, std::vector<int>& packCapacities, std::string topping, BiggestCapDifference difference) {
        int colWidth = 25;
        int length = 120;

        std::string dottedLine(length, '=');
        std::string tinierDottedLine(length, '-');

        int mid = (length-topping.length())/2;
        std::string spaces(mid, ' ');
        std::string lineTop = "="+ spaces + topping + spaces + "=";

        std::vector<int> Ranging(series);
        std::iota(Ranging.begin(), Ranging.end(), 1);

        std::cout << dottedLine << std::endl;
        std::cout << lineTop << std::endl;
        std::cout << dottedLine << std::endl;
        std::cout << std::endl;



        for (int i=0; i<batteries.size(); i++) {
            SinglePackBoard(batteries[i], series, parallel, i+1, packCapacities, difference);
        }
    }

    std::string InputHandling2() {
        std::string result="";

        std::cout<<"Detailed or compact output? (detailed/compact): ";
        while (true) {
            std::string choice;
            std::cin >> choice;
            if (choice == "detailed") {
                result = "detailed";
                break;
            }
            else if (choice == "compact") {
                result = "compact";
                break;
            }
            else {
                std::cout << "Invalid choice. Please enter 'detailed' or 'compact': ";
            }
        }

        return result;
    }

    std::string GradingResult(int percentage) {
        std::vector<std::string> Results = {"Terrible", "Bad", "Average", "Good", "Great"};
        std::string result;
        switch (percentage) {
            case 0 ... 20:
                result = Results[0];
                break;
            case 21 ... 40:
                result = Results[1];
                break;
            case 41 ... 60:
                result = Results[2];
                break;
            case 61 ... 80:
                result = Results[3];
                break;
            case 81 ... 100:
                result = Results[4];
                break;
            default:
                result = "Invalid percentage";
                break;
        }
        return "Pack overall quality: " + result + " (" + std::to_string(percentage) + "%)";
    }
