cmake_minimum_required(VERSION 3.27)
project(networkFlowsTF)

set(CMAKE_CXX_STANDARD 17)

add_executable(networkFlowsTF main.cpp
        src/Scheduler.cpp
        src/Scheduler.h
        src/Task.cpp
        src/Task.h
        src/MetaHeuristic.cpp
        src/MetaHeuristic.h
        src/Model.cpp
        src/Model.h)

include_directories(/Applications/CPLEX_Studio2211/cplex/include/)
include_directories(/Applications/CPLEX_Studio2211/cplex/include/ilcplex)
include_directories(/Applications/CPLEX_Studio2211/concert/include)
include_directories(/Applications/CPLEX_Studio2211/concert/include/ilconcert)
target_link_libraries(networkFlowsTF PUBLIC /Applications/CPLEX_Studio2211/cplex/lib/arm64_osx/static_pic/libcplex.a)
target_link_libraries(networkFlowsTF PUBLIC /Applications/CPLEX_Studio2211/cplex/lib/arm64_osx/static_pic/libilocplex.a)
target_link_libraries(networkFlowsTF PUBLIC /Applications/CPLEX_Studio2211/concert/lib/arm64_osx/static_pic/libconcert.a)
set (target_link_options "-lconcert -lilocplex -lcplex -lm -lpthread -ldl -framework CoreFoundation -framework IOKit -stdlib=libc++")
