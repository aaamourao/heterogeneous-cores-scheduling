//
// Created by Adriano Mourão on 23/06/24.
//

#include "FixAndOptimize.h"

FixAndOptimize::FixAndOptimize(const std::shared_ptr<Model>& aModel, const int aWindowSize)
: windowSize(aWindowSize), model(aModel) {

}

double FixAndOptimize::execute(const std::unordered_map<int, std::shared_ptr<Task>>& taskSchedule) {
    const int varsSize = static_cast<int>(model->tasks.size());
    int iterations = varsSize / windowSize;
    if (varsSize % windowSize != 0) {
        ++iterations;
    }
    int start = 0;
    double current = 0;
    for (int i = 0; i < iterations; ++i) {
        fix(start, std::min(varsSize, start + windowSize), taskSchedule);
        optimize();
        removeFixed();
        current = model->getMakeSpan();
        start += windowSize;
    }

    return current;
}

void FixAndOptimize::fix(const int start, const int end,
                         const std::unordered_map<int, std::shared_ptr<Task>>& taskSchedule) {
    for (int core = 0; core < model->nCores; ++core) {
        int taskNum = 0;
        for (const std::shared_ptr<Task>& task : model->tasks) {
            if (taskNum < start || taskNum > end) {
                double value = taskSchedule.at(task->getId())->getAssignedCoreIndex() == core ? 1.0 : 0.0;
                // TODO: should use emplace here, instead of push_back
                fixedVarConstraints.push_back(
                        IloRange(*(model->env), value, model->runningOnMachineVars[core][task->getId()], value));
                model->cplexModel.add(fixedVarConstraints[fixedVarConstraints.size() - 1]);
            }
            ++taskNum;
        }
    }
}

void FixAndOptimize::optimize() {
    model->solve();
}

void FixAndOptimize::removeFixed() {
    for (const IloRange& constraint : fixedVarConstraints) {
        model->cplexModel.remove(constraint);
    }
    fixedVarConstraints.clear();
}