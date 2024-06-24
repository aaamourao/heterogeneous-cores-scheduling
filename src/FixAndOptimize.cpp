//
// Created by Adriano Mourão on 23/06/24.
//

#include "FixAndOptimize.h"

FixAndOptimize::FixAndOptimize(const std::shared_ptr<Model>& aModel, const int aWindowSize)
: windowSize(aWindowSize), model(aModel), makeSpan(std::numeric_limits<double>::max()) {

}

double FixAndOptimize::execute(const std::unordered_map<int, std::shared_ptr<Task>>& taskSchedule) {
    std::unordered_map<int, int> assignedCores;

    for (const auto& kv : taskSchedule) {
        assignedCores[kv.first] = kv.second->getAssignedCoreIndex();
    }

    const int varsSize = static_cast<int>(model->tasks.size());
    int iterations = varsSize / windowSize;

    if (varsSize % windowSize != 0) {
        ++iterations;
    }
    int start = 0;
    for (int i = 0; i < iterations; ++i) {
        fix(start, std::min(varsSize, start + windowSize), assignedCores);
        optimize(assignedCores);
        if (i != iterations - 1) {
            removeFixed();
        }

        start += windowSize;
    }
    return model->getMakeSpan();
}

void FixAndOptimize::fix(const int start, const int end, std::unordered_map<int, int>& assignedCores) {
    for (int core = 0; core < model->nCores; ++core) {
        int taskNum = 0;
        for (const std::shared_ptr<Task>& task : model->tasks) {
            if (taskNum < start || taskNum > end) {
                const double value = assignedCores[task->getId()] == core ? 1.0 : 0.0;

                // TODO: should use emplace here, instead of push_back
                fixedVarConstraints.push_back(
                        IloRange(*(model->env), value, model->runningOnMachineVars[core][task->getId()], value));
                model->cplexModel.add(fixedVarConstraints[fixedVarConstraints.size() - 1]);
            }
            ++taskNum;
        }
    }
}

void FixAndOptimize::optimize(std::unordered_map<int, int>& assignedCores) {
    model->solve();
    for (int core = 0; core < model->nCores; ++core) {
        for (const std::shared_ptr<Task> &task: model->tasks) {
            if (model->solver.getValue(model->runningOnMachineVars[core][task->getId()]) == 1.0) {
                assignedCores[task->getId()] = core;
            }
        }
    }
}

void FixAndOptimize::removeFixed() {
    for (const IloRange& constraint : fixedVarConstraints) {
        model->cplexModel.remove(constraint);
    }
    fixedVarConstraints.clear();
}

double FixAndOptimize::getMakeSpan() const {
    return model->getMakeSpan();
}