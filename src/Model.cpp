//
// Created by Adriano Mourão on 22/06/24.
//

#include "Model.h"

Model::Model(const int aNFastCores, const int aNLowPowerCores, std::vector<std::shared_ptr<Task>>& aTasks,
             double aSlowFactor)
: nFastCores(aNFastCores), nLowPowerCores(aNLowPowerCores), slowFactor(aSlowFactor), tasks{aTasks} {
    env = std::make_unique<IloEnv>();
    cplexModel = IloModel(*env);

    IloNumVar makeSpan = IloNumVar(*env, 0.0, 500.0, ILOFLOAT);
    cplexModel.add(makeSpan);
    for (const std::shared_ptr<Task>& task : tasks) {
        for (int core = 0; core < nFastCores + nLowPowerCores; ++core) {
            runningOnMachineVars[core].insert({task->getId(), IloNumVar(*env, 0.0, 1.0, ILOBOOL)});
            cplexModel.add(runningOnMachineVars[core][task->getId()]);
        }
    }
    fObj = IloMinimize(*env, makeSpan);
    cplexModel.add(fObj);
    solver = IloCplex(*env);

    for (const std::shared_ptr<Task>& task : tasks) {
        IloExpr taskIsExecutedByOnlyOneCoreConstraintExpr(*env);
        for (int core = 0; core < nFastCores + nLowPowerCores; ++core) {
            taskIsExecutedByOnlyOneCoreConstraintExpr += runningOnMachineVars[core][task->getId()];
        }
        //cplexModel.add(IloRange(*env, taskIsExecutedByOnlyOneCoreConstraintExpr == 1.0));
        cplexModel.add(taskIsExecutedByOnlyOneCoreConstraintExpr == 1.0);
    }

    for (int core = 0; core < nFastCores + nLowPowerCores; ++core) {
        IloExpr coreMakeSpanConstraintExpr(*env);
        for (const std::shared_ptr<Task>& task : tasks) {
            const int duration = core < nFastCores ? task->getDuration()
                    : static_cast<int>(static_cast<double>(task->getDuration()) * slowFactor);
            coreMakeSpanConstraintExpr += duration * runningOnMachineVars[core][task->getId()];
        }
        // cplexModel.add(IloRange(*env, makeSpan >= coreMakeSpanConstraintExpr));
        cplexModel.add(makeSpan >= coreMakeSpanConstraintExpr);
    }

    solver.extract(cplexModel);
    solver.exportModel("adrianoadrianoModel.lp");
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

    std::cout << "function object result = " << solver.getValue(fObj) << std::endl;

    for (int core = 0; core < nFastCores + nLowPowerCores; ++core) {
        for (const std::shared_ptr<Task>& task : tasks) {
            std::cout << "task " << task->getId() << " running on core " << core << ": " << solver.getValue(runningOnMachineVars[core][task->getId()]) << std::endl;
        }
    }
    return schedule;
}