cmake_minimum_required(VERSION 3.27)
project(networkFlowsTF)

set(CMAKE_CXX_STANDARD 17)

add_executable(networkFlowsTF main.cpp
        src/Scheduler.cpp
        src/Scheduler.h
        src/Task.cpp
        src/Task.h
        src/MetaHeuristic.cpp
        src/MetaHeuristic.h)
