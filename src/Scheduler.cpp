//
// Created by Adriano Mourão on 11/06/24.
//

#include <iostream>
#include <queue>
#include "Scheduler.h"

int Scheduler::fastCores = 0;
int Scheduler::lowPowerCores = 0;

Scheduler::Scheduler(const int aNFastCores, const int aNLowPowerCores, const double aSlowFactor)
        : nFastCores(aNFastCores), nLowPowerCores(aNLowPowerCores), slowFactor(aSlowFactor),
          makeSpan(std::numeric_limits<int>::max()) {
    fastCores = aNFastCores;
    lowPowerCores = aNLowPowerCores;
}

void Scheduler::addTask(const Task task) {
    tasks.push_back(std::make_shared<Task>(task));
}

void Scheduler::reset() {
    bestSchedule.clear();
    makeSpan = std::numeric_limits<int>::max();
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
                         std::unordered_set<std::string>& dp) {
    if (index == tasks.size() || current >= makeSpan) {
        if (current < makeSpan) {
            makeSpan = current;
            bestSchedule = schedule;
        }
        return current;
    }
    SchedulerHasher hasher;
    dp.insert(hasher(schedule));

    int minMakeSpan = std::numeric_limits<int>::max();
    double factor = 1.0;

    for (int coreIndex = 0; coreIndex < nFastCores + nLowPowerCores; ++coreIndex) {
        if (coreIndex >= nFastCores) {
            factor = slowFactor;
        }

        std::shared_ptr<Task> task = tasks[index];
        int start = 0;
        if (!schedule[coreIndex].empty()) {
            start = schedule[coreIndex][schedule[coreIndex].size() - 1].getEnd();
        }
        schedule[coreIndex].push_back(*task);
        schedule[coreIndex][schedule[coreIndex].size() - 1].scheduleTask(start, coreIndex, factor);

        // We may actually be doing a DFS instead of Dynamic Programming here
        if (dp.find(hasher(schedule)) != dp.end()) {
            schedule[coreIndex].pop_back();
            continue;
        }
        minMakeSpan = std::min(minMakeSpan, backtrack(index + 1, schedule,
                                                      std::max(current, schedule[coreIndex][
                                                              schedule[coreIndex].size() - 1
                                                              ].getEnd()), dp));
        schedule[coreIndex].pop_back();

    }

    return minMakeSpan;
}

void Scheduler::execute() {
    const int totalCpus = nFastCores + nLowPowerCores;
    std::vector<std::vector<Task>> schedule(totalCpus, std::vector<Task>());
    std::string tasksScheduled(tasks.size(), 'x');
    std::unordered_set<std::string> dp;
    makeSpan = backtrack(0, schedule, 0, dp);
}

int Scheduler::backtrack(const int index, std::vector<std::vector<Task>>& schedule, const int current,
                         std::string& tasksScheduled, std::unordered_set<std::string>& dp, const int beginning,
                         const int end) {
    if (index == end - beginning || current >= makeSpan) {
        if (current < makeSpan) {
            makeSpan = current;
            bestSchedule = schedule;
        }
        return current;
    }

    dp.insert(tasksScheduled);

    int minMakeSpan = std::numeric_limits<int>::max();
    double factor = 1.0;

    for (int coreIndex = 0; coreIndex < nFastCores + nLowPowerCores; ++coreIndex) {
        const int realIndex = index + beginning;
        if (coreIndex >= nFastCores) {
            factor = slowFactor;
        }
        if (tasksScheduled[index] == 'x') {
            std::shared_ptr<Task> task = tasks[realIndex];
            int start = 0;
            if (!schedule[coreIndex].empty()) {
                start = schedule[coreIndex][schedule[coreIndex].size() - 1].getEnd();
            }
            schedule[coreIndex].push_back(*task);
            schedule[coreIndex][schedule[coreIndex].size() - 1].scheduleTask(start, coreIndex, factor);
            tasksScheduled[index] = static_cast<char>(coreIndex + '0');

            // We may actually be doing a DFS instead of Dynamic Programming here
            if (dp.find(tasksScheduled) != dp.end()) {
                continue;
            }
            minMakeSpan = std::min(minMakeSpan, backtrack(index + 1, schedule,
                                                          std::max(current, schedule[coreIndex][
                                                                  schedule[coreIndex].size() - 1
                                                          ].getEnd()),
                                                          tasksScheduled, dp, beginning, end));
            schedule[coreIndex].pop_back();
            tasksScheduled[index] = 'x';
        }
    }

    return minMakeSpan;
}

void Scheduler::executeBatch(const int start, const int end, const int current) {
    const int totalCpus = nFastCores + nLowPowerCores;
    std::vector<std::vector<Task>> schedule(totalCpus, std::vector<Task>());
    if (!bestSchedule.empty()) {
        schedule = bestSchedule;
    }
    std::string tasksScheduled(end - start, 'x');
    std::unordered_set<std::string> dp;
    // reinitialize makeSpan or it is going to stop immediately
    makeSpan = std::numeric_limits<int>::max();
    makeSpan = backtrack(0, schedule, current, tasksScheduled, dp, start, end);
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
