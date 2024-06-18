//
// Created by Adriano Mourão on 11/06/24.
//

#include <iostream>
#include "Scheduler.h"

Scheduler::Scheduler(const int aNFastCpus, const int aNLowPowerCpus, const double aSlowFactor)
        : nFastCores(aNFastCpus), nLowPowerCores(aNLowPowerCpus), slowFactor(aSlowFactor),
        makeSpan(std::numeric_limits<int>::max()) {
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
                         std::string& tasksScheduled, const int totalScheduledTasks,
                         std::unordered_set<std::string>& dp) {
    if (totalScheduledTasks == tasks.size() || current >= makeSpan) {
        if (current < makeSpan) {
            makeSpan = current;
            bestSchedule = schedule;
        }
        return current;
    }

    dp.insert(tasksScheduled);

    const int coreIndex = index == nFastCores + nLowPowerCores ? 0 : index;

    int minMakeSpan = std::numeric_limits<int>::max();
    double factor = 1.0;
    if (coreIndex >= nFastCores) {
        factor = slowFactor;
    }

    for (int i = 0; i < tasks.size(); ++i) {
        if (tasksScheduled[i] == 'x') {
            std::shared_ptr<Task> task = tasks[i];
            int start = 0;
            if (!schedule[coreIndex].empty()) {
                start = schedule[coreIndex][schedule[coreIndex].size() - 1].getEnd();
            }
            schedule[coreIndex].push_back(*task);
            schedule[coreIndex][schedule[coreIndex].size() - 1].scheduleTask(start, coreIndex, factor);
            tasksScheduled[i] = static_cast<char>(coreIndex + '0');

            // We may actually be doing a DFS instead of Dynamic Programming here
            if (dp.find(tasksScheduled) != dp.end()) {
                continue;
            }
            minMakeSpan = std::min(minMakeSpan, backtrack(coreIndex + 1, schedule,
                                                          std::max(current, schedule[coreIndex][
                                                                  schedule[coreIndex].size() - 1
                                                                  ].getEnd()),
                                                          tasksScheduled, totalScheduledTasks + 1, dp));
            schedule[coreIndex].pop_back();
            tasksScheduled[i] = 'x';
        }
    }

    return minMakeSpan;
}

void Scheduler::execute() {
    const int totalCpus = nFastCores + nLowPowerCores;
    std::vector<std::vector<Task>> schedule(totalCpus, std::vector<Task>());
    std::string tasksScheduled(tasks.size(), 'x');
    std::unordered_set<std::string> dp;
    makeSpan = backtrack(0, schedule, 0, tasksScheduled, 0, dp);
}

int Scheduler::backtrack(const int index, std::vector<std::vector<Task>>& schedule, const int current,
                         std::string& tasksScheduled, const int totalScheduledTasks,
                         std::unordered_set<std::string>& dp, const int beginning, const int end) {
    if (totalScheduledTasks == beginning - end || current >= makeSpan) {
        if (current < makeSpan) {
            makeSpan = current;
            bestSchedule = schedule;
        }
        return current;
    }

    dp.insert(tasksScheduled);

    const int coreIndex = index == nFastCores + nLowPowerCores ? 0 : index;

    int minMakeSpan = std::numeric_limits<int>::max();
    double factor = 1.0;
    if (coreIndex >= nFastCores) {
        factor = slowFactor;
    }

    for (int ii = beginning; ii < end; ++ii) {
        const int i = ii - beginning;
        if (tasksScheduled[i] == 'x') {
            std::shared_ptr<Task> task = tasks[ii];
            int start = 0;
            if (!schedule[coreIndex].empty()) {
                start = schedule[coreIndex][schedule[coreIndex].size() - 1].getEnd();
            }
            schedule[coreIndex].push_back(*task);
            schedule[coreIndex][schedule[coreIndex].size() - 1].scheduleTask(start, coreIndex, factor);
            tasksScheduled[i] = static_cast<char>(coreIndex + '0');

            // We may actually be doing a DFS instead of Dynamic Programming here
            if (dp.find(tasksScheduled) != dp.end()) {
                continue;
            }
            minMakeSpan = std::min(minMakeSpan, backtrack(coreIndex + 1, schedule,
                                                          std::max(current, schedule[coreIndex][
                                                                  schedule[coreIndex].size() - 1
                                                          ].getEnd()),
                                                          tasksScheduled, totalScheduledTasks + 1, dp,
                                                          beginning, end));
            schedule[coreIndex].pop_back();
            tasksScheduled[i] = 'x';
        }
    }

    return minMakeSpan;
}

void Scheduler::executeBatch(const int start, const int end, const int current) {
    const int totalCpus = nFastCores + nLowPowerCores;
    std::vector<std::vector<Task>> schedule(totalCpus, std::vector<Task>());
    std::string tasksScheduled(end - start, 'x');
    std::unordered_set<std::string> dp;
    // reinitialize makeSpan or it is going to stop immediately
    makeSpan = std::numeric_limits<int>::max();
    makeSpan = backtrack(0, schedule, current, tasksScheduled, 0, dp, start, end);
}

std::vector<std::shared_ptr<Task>> Scheduler::getTasks() const {
    return tasks;
}

void Scheduler::saveSchedule() {
    if (!bestSchedule.empty()) {
        tasks.clear();
        for (const std::vector<Task>& coreSchedule : bestSchedule) {
            for (const Task& task : coreSchedule) {
                addTask(task);
            }
        }
    }
    std::sort(tasks.begin(), tasks.end(), [](const std::shared_ptr<Task>& lhs, const std::shared_ptr<Task>& rhs) {
        return lhs->getId() < rhs->getId();
    });
}