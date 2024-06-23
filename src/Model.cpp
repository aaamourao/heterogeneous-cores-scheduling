//
// Created by Adriano Mourão on 22/06/24.
//

#include "Model.h"

Model::Model(const int aNCores, std::vector<std::shared_ptr<Task>>& aTasks, const std::vector<double>& aSlowFactor)
: nCores(aNCores), slowFactor(aSlowFactor), tasks{aTasks} {
    env = std::make_unique<IloEnv>();
    cplexModel = IloModel(*env);

    IloNumVar makeSpan = IloNumVar(*env, 0.0, IloInfinity, ILOFLOAT);
    cplexModel.add(makeSpan);
    for (const std::shared_ptr<Task>& task : tasks) {
        for (int core = 0; core < nCores; ++core) {
            runningOnMachineVars[core].insert({task->getId(), IloNumVar(*env, 0.0, 1.0, ILOBOOL)});
            cplexModel.add(runningOnMachineVars[core][task->getId()]);
        }
    }
    fObj = IloMinimize(*env, makeSpan);
    cplexModel.add(fObj);
    solver = IloCplex(*env);

    for (const std::shared_ptr<Task>& task : tasks) {
        IloExpr taskIsExecutedByOnlyOneCoreConstraintExpr(*env);
        for (int core = 0; core < nCores; ++core) {
            taskIsExecutedByOnlyOneCoreConstraintExpr += runningOnMachineVars[core][task->getId()];
        }
        cplexModel.add(taskIsExecutedByOnlyOneCoreConstraintExpr == 1.0);
    }

    for (int core = 0; core < nCores; ++core) {
        IloExpr coreMakeSpanConstraintExpr(*env);
        for (const std::shared_ptr<Task>& task : tasks) {
            const int duration = static_cast<int>(static_cast<double>(task->getDuration()) * slowFactor[core]);
            coreMakeSpanConstraintExpr += duration * runningOnMachineVars[core][task->getId()];
        }
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
/*
    for (int core = 0; core < nFastCores + nLowPowerCores; ++core) {
        for (const std::shared_ptr<Task>& task : tasks) {
            if (solver.getValue(runningOnMachineVars[core][task->getId()]) == 1.0) {
                std::cout << "task " << task->getId() << " running on core " << core << std::endl;
            }
        }
    }*/
    return schedule;
}

void Model::importIncumbentSolution(const std::unordered_map<int, std::shared_ptr<Task>>& taskSchedule) {
    IloNumVarArray startVar(*env);
    IloNumArray startVal(*env);
    for (const auto& kv : taskSchedule) {
        const std::shared_ptr<Task> task = kv.second;
        const int core = task->getAssignedCoreIndex();
        startVar.add(runningOnMachineVars[core][task->getId()]);
        startVal.add(1.0);
        for (int otherCore = 0; otherCore < nCores; ++otherCore) {
            if (otherCore != core) {
                startVar.add(runningOnMachineVars[otherCore][task->getId()]);
                startVal.add(0.0);
            }
        }
    }
    solver.addMIPStart(startVar, startVal);
    startVal.end();
    startVar.end();
}