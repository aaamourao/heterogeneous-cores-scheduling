//
// Created by Adriano Mourão on 11/06/24.
//

#include <iostream>
#include "Scheduler.h"

Scheduler::Scheduler(const int aNFastCpus, const int aNLowPowerCpus, const double aSlowFactor)
        : nFastCores(aNFastCpus), nLowPowerCores(aNLowPowerCpus), slowFactor(aSlowFactor), makeSpan(-1) {
}

void Scheduler::addTask(const Task task) {
    tasks.push_back(std::make_shared<Task>(task));
}

int Scheduler::getNFastCores() const {
    return nFastCores;
}

int Scheduler::getNLowPowerCores() const {
    return nLowPowerCores;
}

int Scheduler::getMakeSpan() const {
    return makeSpan;
}

int Scheduler::backtrack(const int index, std::vector<std::vector<Task>>& schedule, const int current,
                         std::vector<bool>& tasksScheduled, const int totalScheduledTasks) {
    if (totalScheduledTasks == tasks.size()) {
        return current;
    }

    const int coreIndex = index == nFastCores + nLowPowerCores ? 0 : index;

    int minMakeSpan = std::numeric_limits<int>::max();
    double factor = 1.0;
    if (coreIndex >= nFastCores) {
        factor = slowFactor;
        // If the core is slow, we may not assign a task to this core at this time
        minMakeSpan = std::min(minMakeSpan, backtrack(coreIndex + 1, schedule, current,tasksScheduled,
                                                      totalScheduledTasks));
    }
    // Assign a task to the core
    for (int i = 0; i < tasks.size(); ++i) {
        if (!tasksScheduled[i]) {
            std::shared_ptr<Task> task = tasks[i];
            int start = 0;
            if (!schedule[coreIndex].empty()) {
                start = schedule[coreIndex][schedule[coreIndex].size() - 1].getEnd();
            }
            schedule[coreIndex].push_back(*task);
            schedule[coreIndex][schedule[coreIndex].size() - 1].scheduleTask(start, coreIndex, factor);
            tasksScheduled[i] = true;
            int currMakeSpan = backtrack(coreIndex + 1, schedule,
                                         std::max(current,schedule[coreIndex][schedule[coreIndex].size() - 1].getEnd()),
                                         tasksScheduled, totalScheduledTasks + 1);
            if (minMakeSpan >= currMakeSpan) {
                minMakeSpan = currMakeSpan;
                task->scheduleTask(start, coreIndex, factor);
            }
            schedule[coreIndex].pop_back();
            tasksScheduled[i] = false;
        }
    }

    return minMakeSpan;
}

void Scheduler::execute() {
    const int totalCpus = nFastCores + nLowPowerCores;
    std::vector<std::vector<Task>> schedule(totalCpus, std::vector<Task>());
    std::vector<bool> tasksScheduled(tasks.size(), false);
    makeSpan = backtrack(0, schedule, 0, tasksScheduled, 0);
}

std::vector<std::shared_ptr<Task>> Scheduler::getTasks() const {
    return tasks;
}