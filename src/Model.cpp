//
// Created by Adriano Mourão on 22/06/24.
//

#include "Model.h"

Model::Model(const int aNFastCores, const int aNLowPowerCores, const std::vector<Task>& aTasks, double aSlowFactor)
: nFastCores(aNFastCores), nLowPowerCores(aNLowPowerCores), slowFactor(aSlowFactor), tasks{aTasks} {
    env = std::make_unique<IloEnv>();
    cplexModel = IloModel(*env);
    IloExpr obj(*env);
    for (const Task& task : aTasks) {
        IloExpr constraintExpr(*env);
        for (int core = 0; core < nFastCores + nLowPowerCores; ++core) {
            runningOnMachineVars[core].insert({task.getId(), IloNumVar(*env, 0.0, 1.0, ILOBOOL)});
            cplexModel.add(runningOnMachineVars[core][task.getId()]);
            const int duration = core < nFastCores ? task.getDuration()
                    : static_cast<int>(static_cast<double>(task.getDuration()) * slowFactor);
            obj += runningOnMachineVars[core][task.getId()] * duration;
            constraintExpr += runningOnMachineVars[core][task.getId()];
        }
        cplexModel.add(IloRange(*env, 1.0, constraintExpr, 1.0));
    }
    cplexModel.add(IloMinimize(*env, obj));
    solver = IloCplex(*env);
    solver.extract(cplexModel);
}

Model::~Model() {
    env->end();
}

bool Model::solve() {
    return solver.solve() == IloTrue;
}

std::string Model::getStatus() {
    return std::to_string(solver.getStatus());
}

std::vector<std::vector<Task>> Model::getSchedule() {
    std::vector<std::vector<Task>> schedule;

    for (int core = 0; core < nFastCores + nLowPowerCores; ++core) {
        for (const Task& task : tasks) {
            if (solver.getValue(runningOnMachineVars[core][task.getId()]) == 1) {
                std::cout << "task " << task.getId() << " running on core " << core << std::endl;
            }
        }
    }
    return schedule;
}