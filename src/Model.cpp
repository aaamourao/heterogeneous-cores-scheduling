//
// Created by Adriano Mourão on 22/06/24.
//

#include "Model.h"

Model::Model(const int aNFastCores, const int aNLowPowerCores, const std::vector<Task>& aTasks)
: nFastCores(aNFastCores), nLowPowerCores(aNLowPowerCores) {
    cplexModel = IloModel(env);
    IloExpr c;
    for (int core = 0; core < nFastCores + nLowPowerCores; ++core) {
        for (const Task& task : aTasks) {
            runningOnMachineVars[core].insert({task.getId(), IloNumVar(env, 0.0, 1.0, ILOBOOL)});
            cplexModel.add(runningOnMachineVars[core][task.getId()]);
            tasksDuration[core].insert({task.getId(), IloNumVar(env, 10.0, 100.0, ILOFLOAT)});
            cplexModel.add(tasksDuration[core][task.getId()]);
            c += runningOnMachineVars[core][task.getId()] * tasksDuration[core][task.getId()];
        }
    }
    cplexModel.add(IloMinimize(env, c));
    solver = IloCplex(cplexModel);
}

Model::~Model() {
    env.end();
}

int Model::solve() {
    return solver.solve() == IloTrue ? 1 : 0;
}