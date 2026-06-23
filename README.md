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

## 🗺️ Roadmap (Upcoming Features)
* **Pack Quality Measurer:** Implement a grading system (Great, Good, Average, Bad, Terrible) based on the percentage deviation from the average pack capacity.
* **IR factor:** Include internal resistance as packing factor for more optimized results.
* **Website:** Build a website around it for ease of use. 

## 📄 Data Format
The application expects `Batteries.csv` to be formatted with semicolons as delimiters. 

## License & Usage

This project is licensed under the **PolyForm Noncommercial 1.0.0** license.

**What this means for you:**
*   **Personal & Hobby Use:** You are free to download, use, and modify this code for your own personal, educational, or hobbyist battery projects.
*   **Commercial Use:** Using this software for any commercial purpose, for-profit business, or professional service is **strictly prohibited** without express permission. 

If you are interested in using this software for commercial applications, please contact mercrypt@proton.me to discuss a commercial licensing agreement. 

See the [LICENSE](LICENSE) file for full legal details.

