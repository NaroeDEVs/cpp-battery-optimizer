# 🔋 Battery Pack Optimizer

An advanced C++ command-line tool designed to help DIY battery builders construct mathematically perfect, optimally balanced battery packs. 

Unlike basic calculators that only balance by capacity, this optimizer uses a dual-parameter approach: it calculates both **Capacity (mAh)** and **Internal Resistance (mΩ)** (using parallel conductance physics). By combining a Greedy sorting algorithm with a powerful **Hill-Climbing** optimization routine, it safely distributes cells into series and parallel configurations (e.g., 10S4P) to prevent thermal imbalances and BMS tripping.

## ✨ Features
* **Dual-Parameter Optimization:** Assign custom weight percentages (e.g., 90% Resistance / 10% Capacity) based on your cell quality (e.g., reclaimed laptop cells vs. brand new cells).
* **Advanced Hill-Climbing Algorithm:** The program performs thousands of hypothetical cell swaps across parallel groups to find the mathematically optimal layout.
* **Smart Cell Selection:** Automatically filters out cells with dangerous internal resistance and culls the weakest cells before building your requested configuration.
* **Physics-Based IR Calculation:** Calculates target parallel group resistance correctly using conductance ($1/R_{total} = 1/R_1 + 1/R_2...$).
* **Massive Benchmarking:** Proven to perform up to 97% better than standard greedy packing algorithms (see benchmarks below).
* **Dynamic Console Reporting:** Generates highly formatted ASCII tables showing the layout, capacities, resistances, and exact variance percentages.

## 📊 Benchmarks & Performance
A Python automated test suite was used to run **3,000 simulations** on randomized "salvaged" cell data to compare this optimizer's Hill-Climbing algorithm against standard greedy algorithms (like Repackr). 

**Hardware:** Tested on Fedora Linux (Execution time for 1,000 iterations: ~16 seconds).

| Optimization Focus | Parameter | Basic Greedy Algorithm (Initial) | After Hill-Climbing | Improvement |
| :--- | :--- | :--- | :--- | :--- |
| **50% IR / 50% Cap**<br>*(Balanced)* | **Capacity Variance**<br>**Resistance Variance** | 16.577%<br>40.142% | **1.583%**<br>**1.799%** | 🔥 **90.5%**<br>🔥 **95.5%** |
| **10% IR / 90% Cap**<br>*(Capacity Focus)* | **Capacity Variance**<br>**Resistance Variance** | 1.941%<br>56.544% | **0.539%**<br>**6.173%** | 🔥 **72.2%**<br>🔥 **89.1%** |
| **90% IR / 10% Cap**<br>*(Resistance Focus)* | **Capacity Variance**<br>**Resistance Variance** | 23.047%<br>20.619% | **4.569%**<br>**0.608%** | 🔥 **80.2%**<br>🔥 **97.0%** |

## 🚀 How It Works
1. **Load:** Reads your cell data (ID, Capacity, Internal Resistance, Manufacturer) from a CSV file.
2. **Configure:** Prompts for your desired Series (S) and Parallel (P) configuration, max voltages, and optimization weights.
3. **Validate & Filter:** Culls high-resistance cells and validates if enough inventory exists for the build.
4. **Greedy Sort:** Sorts the top $S \times P$ cells using a weighted score and performs an initial distribution.
5. **Hill-Climbing:** Aggressively swaps cells between parallel groups until no further mathematical improvement can be made towards the target average capacity and conductance.
6. **Output:** Prints detailed blocks showing exactly which cell goes into which pack and the summary statistics.

## 🛠️ Prerequisites & Compilation
To build and run this project, you will need a standard C++ compiler (like GCC, Clang, or MSVC) that supports **C++20** (due to `<format>` and `<ranges>` libraries). CMake is recommended for building.

## 🗺️ Roadmap (Upcoming Features)
* **WebAssembly (WASM) Port:** Compile the C++ backend into WebAssembly to run entirely in the browser.
* **Frontend UI:** Build a clean, modern website hosted on GitHub Pages for ease of use (replacing the CLI).
* **"Mini AI" Statistical Advisor:** An automated script that analyzes the standard deviation of your inputted CSV and recommends the optimal Capacity/Resistance weights for your specific batch of cells.

## 📄 Data Format
The application expects `Batteries.csv` to be formatted with semicolons as delimiters:
`ID;Capacity;Resistance;Manufacturer;Condition`

## ⚖️ License & Usage

This project is licensed under the **PolyForm Noncommercial 1.0.0** license.

**What this means for you:**
*   **Personal & Hobby Use:** You are free to download, use, and modify this code for your own personal, educational, or hobbyist battery projects.
*   **Commercial Use:** Using this software for any commercial purpose, for-profit business, or professional service is **strictly prohibited** without express permission. 

If you are interested in using this software for commercial applications, please contact mercrypt@proton.me to discuss a commercial licensing agreement. 

See the [LICENSE](LICENSE) file for full legal details.
