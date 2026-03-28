# 🔋 Battery Pack Optimizer

A C++ command-line tool designed to help DIY battery builders construct optimally balanced battery packs. The program reads a database of individual battery cells (like 18650s or 21700s), selects the best available cells, and algorithmically distributes them into series and parallel configurations (e.g., 10S4P) to ensure every pack has a tightly matched total capacity.

## ✨ Features
* **Flexible Data Input:** Read your cell inventory from a CSV file (`Batteries.csv`) or enter cell capacities and details manually via the console.
* **Smart Cell Selection:** Automatically sorts your inventory and selects only the highest-capacity cells needed to build your requested configuration.
* **Algorithmic Balancing:** Uses a packing algorithm to distribute cells across parallel groups, minimizing the capacity difference between series packs.
* **Dynamic Console Reporting:** Choose between a compact summary view or a detailed view to see exactly which cell goes into which pack.
* **Divergence Tracking:** Calculates and displays the maximum capacity difference between your highest and lowest packs.

## 🚀 How It Works
1. **Load:** The program reads your cell data (ID, Capacity, Manufacturer) into the initial pool.
2. **Configure:** You input your desired Series (S) and Parallel (P) configuration.
3. **Validate:** It checks if your inventory has enough cells to safely build the pack.
4. **Sort & Select:** It culls the weak cells and selects the top `S * P` cells.
5. **Pack & Balance:** It distributes the selected cells into packs to keep the total mAh of each series connection as close to identical as possible.
6. **Output:** It prints a highly formatted ASCII table showing the layout, capacities, and standard deviations.

## 🛠️ Prerequisites & Compilation
To build and run this project, you will need a standard C++ compiler (like GCC, Clang, or MSVC) that supports C++11 or higher.

### Project Structure
Ensure your files are organized as follows:
* `main.cpp`
* `Batteries.csv` *(Your cell inventory)*
* `/Battery.h`
* `/Functions/Capacity algorithms.h`
* `/Functions/Input and Output.h`
* `/Functions/Packing Algorithms.h`

## 🗺️ Roadmap (Upcoming Features)
* **Pack Quality Measurer:** Implement a grading system (Great, Good, Average, Bad, Terrible) based on the percentage deviation from the average pack capacity.
* **Switcher** Add a secondary layer of pack improvement by manually switching batteries between packs, measure capacity difference and thru many iterations try to optimize the packs even more. 

## 📄 Data Format
The application expects `Batteries.csv` to be formatted with semicolons as delimiters. 

**Example `Batteries.csv`:**
```csv
ID;Capacity(mAh);Manufacturer
1;2500;Samsung
2;2480;Samsung
3;2510;Sony
