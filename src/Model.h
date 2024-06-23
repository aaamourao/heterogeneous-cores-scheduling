//
// Created by Adriano Mourão on 22/06/24.
//

#ifndef NETWORKFLOWSTF_MODEL_H
#define NETWORKFLOWSTF_MODEL_H


#include <ilomodel.h>
#include <vector>
#include <unordered_map>
#include <ilocplex.h>
#include "Task.h"

class Model {
    std::unique_ptr<IloEnv> env;
    IloModel cplexModel;
    int nFastCores;
    int nLowPowerCores;
    double slowFactor;
    // TODO: optimzie it by saving shared_ptr
    std::vector<std::shared_ptr<Task>> tasks;
    std::unordered_map<int, std::unordered_map<int, IloNumVar>> runningOnMachineVars;
    IloCplex solver;
    IloObjective fObj;
public:
    explicit Model(int aNFastCores, int aNLowPowerCores, std::vector<std::shared_ptr<Task>>& aTask, double aSlowFactor);

    virtual ~Model();

    bool solve();

    std::vector<std::vector<Task>> getSchedule();

    std::string getStatus();
};


#endif //NETWORKFLOWSTF_MODEL_H
