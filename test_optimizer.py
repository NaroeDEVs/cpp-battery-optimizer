import os
import random
import subprocess
import re
import time
import sys

EXECUTABLE = "./cmake-build-debug/C_BatteryPackOptimizer"
if sys.platform.startswith("win"):
    EXECUTABLE += ".exe"

if not os.path.exists(EXECUTABLE):
    EXECUTABLE = "./C_BatteryPackOptimizer"
    if sys.platform.startswith("win") and not EXECUTABLE.endswith(".exe"):
        EXECUTABLE += ".exe"

CSV_FILE = "Batteries.csv"
TEST_RUNS_PER_CONFIG = 1000

def generate_random_csv(filename, num_cells=80):
    with open(filename, 'w') as f:
        f.write("ID;Capacity;Resistance;Manufacturer;Condition\n")
        for i in range(1, num_cells + 1):
            cap = random.randint(1800, 3200)
            res = round(random.uniform(15.0, 50.0), 2)
            f.write(f"{i};{cap};{res};TestCorp;Used\n")

def run_single_test(res_w, cap_w):
    input_data = f"{res_w}\n{cap_w}\nd\n"
    
    process = subprocess.Popen(
        [EXECUTABLE],
        stdin=subprocess.PIPE,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        text=True
    )
    stdout, _ = process.communicate(input=input_data)
    
    cap_variances = re.findall(r"Capacity variance:\s+([\d.]+)%", stdout)
    res_variances = re.findall(r"Resistance variance:\s+([\d.]+)%", stdout)
    
    if len(cap_variances) >= 2 and len(res_variances) >= 2:
        return (float(cap_variances[0]), float(res_variances[0]),
                float(cap_variances[1]), float(res_variances[1]))
    return None

def run_test_suite(res_w, cap_w, num_tests):
    print(f"\n[🚀] Starting block: {res_w}% Resistance, {cap_w}% Capacity ({num_tests} tests)")
    
    total_base_cap = 0.0
    total_base_res = 0.0
    total_final_cap = 0.0
    total_final_res = 0.0
    valid_runs = 0
    
    start_time = time.time()
    
    for i in range(num_tests):
        generate_random_csv(CSV_FILE)
        result = run_single_test(res_w, cap_w)
        
        if result:
            b_c, b_r, f_c, f_r = result
            total_base_cap += b_c
            total_base_res += b_r
            total_final_cap += f_c
            total_final_res += f_r
            valid_runs += 1
            
        if (i + 1) % 200 == 0:
            print(f"   [+] Completed: {i + 1}/{num_tests}...")

    if valid_runs == 0:
        print("   [!] Failed to read C++ output.")
        return

    duration = time.time() - start_time

    avg_base_cap = total_base_cap / valid_runs
    avg_base_res = total_base_res / valid_runs
    avg_final_cap = total_final_cap / valid_runs
    avg_final_res = total_final_res / valid_runs

    print("-" * 55)
    print(f"[📊] RESULTS ({valid_runs} successful iterations in {duration:.1f} s.):")
    print(f"   [CAPACITY] Initial variance: {avg_base_cap:.3f}%  ->  After Hill Climb: {avg_final_cap:.3f}%")
    print(f"   [RESISTANCE] Initial variance: {avg_base_res:.3f}%  ->  After Hill Climb: {avg_final_res:.3f}%")
    
    cap_imp = ((avg_base_cap - avg_final_cap) / avg_base_cap) * 100 if avg_base_cap > 0 else 0
    res_imp = ((avg_base_res - avg_final_res) / avg_base_res) * 100 if avg_base_res > 0 else 0
    
    print(f"   [🔥] IMPROVEMENT (Capacity): {cap_imp:.1f}% better")
    print(f"   [🔥] IMPROVEMENT (Resistance): {res_imp:.1f}% better")
    print("-" * 55)

def main():
    if not os.path.exists(EXECUTABLE):
        print(f"Executable not found: {EXECUTABLE}")
        return

    configs = [
        (50, 50),
        (10, 90),
        (90, 10),
    ]

    print("=" * 60)
    print(f"   [⚙️] MASSIVE BENCHMARKING (Total {len(configs) * TEST_RUNS_PER_CONFIG} tests) [⚙️]")
    print("=" * 60)

    for res_w, cap_w in configs:
        run_test_suite(res_w, cap_w, TEST_RUNS_PER_CONFIG)
        
    print("\n[✅] All tests completed!")

if __name__ == "__main__":
    main()
