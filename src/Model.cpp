//
// Created by Adriano Mourão on 22/06/24.
//

#include "Model.h"

Model::Model(const int aNFastCores, const int aNLowPowerCores, const std::vector<Task>& aTasks)
: nFastCores(aNFastCores), nLowPowerCores(aNLowPowerCores) {
    cplexModel = IloModel(env);
    IloExpr c;
    for (const IloNumVar& x : runningOnMachineVars) {
        for (const IloNumVar& d : tasksDuration) {
            // TODO: Should use emplace here, should fix this later
            runningOnMachineVars.push_back(IloNumVar(env, 0.0, 1.0, ILOBOOL));
            cplexModel.add(runningOnMachineVars[runningOnMachineVars.size() - 1]);
            // TODO: Should use emplace here, should fix this later
            tasksDuration.push_back(IloNumVar(env, 10.0, 100.0, ILOFLOAT));
            cplexModel.add(tasksDuration[tasksDuration.size() - 1]);
            c += x * d;
        }
    }
    cplexModel.add(IloMinimize(env, c));
    solver = IloCplex(cplexModel);
}

Model::~Model() {
    env.end();
}