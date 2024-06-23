//
// Created by Adriano Mourão on 22/06/24.
//

#ifndef NETWORKFLOWSTF_MODEL_H
#define NETWORKFLOWSTF_MODEL_H


#include <ilomodel.h>
#include <vector>
#include <ilocplex.h>
#include "Task.h"

class Model {
    IloEnv env;
    IloModel cplexModel;
    int nFastCores;
    int nLowPowerCores;
    std::vector<IloNumVar> runningOnMachineVars;
    std::vector<IloNumVar> tasksDuration;
    IloCplex solver;
public:
    explicit Model(int aNFastCores, int aNLowPowerCores, const std::vector<Task>& aTask);
    virtual ~Model();
};


#endif //NETWORKFLOWSTF_MODEL_H
