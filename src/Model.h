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
    int nCores;
    std::vector<double> slowFactor;
    std::vector<std::shared_ptr<Task>> tasks;
    std::unordered_map<int, std::unordered_map<int, IloNumVar>> runningOnMachineVars;
    IloCplex solver;
    IloObjective fObj;
public:
    explicit Model(int aNCores, std::vector<std::shared_ptr<Task>>& aTask, const std::vector<double>& aSlowFactor);

    virtual ~Model();

    bool solve();

    std::vector<std::vector<Task>> getSchedule();

    std::string getStatus();

    void importIncumbentSolution(const std::unordered_map<int, std::shared_ptr<Task>>& tasks);
};


#endif //NETWORKFLOWSTF_MODEL_H
