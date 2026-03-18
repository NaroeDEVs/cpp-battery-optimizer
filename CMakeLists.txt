cmake_minimum_required(VERSION 4.1)
project(C_BatteryPackOptimizer)

set(CMAKE_CXX_STANDARD 14)

include_directories("Main folder")

add_executable(C_BatteryPackOptimizer
        "Main folder/Main.cpp"
        "Main folder/Battery.h"
        "Main folder/Functions/Capacity algorithms.cpp"
        "Main folder/Functions/Capacity algorithms.h"
        "Main folder/Functions/Input and Output.cpp"
        "Main folder/Functions/Input and Output.h"
        "Main folder/Functions/Packing Algorithms.cpp"
        "Main folder/Functions/Packing Algorithms.h"
)
