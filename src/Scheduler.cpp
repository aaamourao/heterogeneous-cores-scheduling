//
// Created by Adriano Mourão on 11/06/24.
//

#include <iostream>
#include <queue>
#include "Scheduler.h"

Scheduler::Scheduler(const int aNCores, const std::vector<double>& aSlowFactor)
        : nCores(aNCores), slowFactor(aSlowFactor),
          makeSpan(std::numeric_limits<int>::max()) {
}

void Scheduler::addTask(std::shared_ptr<Task>& task) {
    tasks.insert({task->getId(), task});
}

void Scheduler::reset() {
    bestSchedule.clear();
    makeSpan = std::numeric_limits<int>::max();
}

int Scheduler::getNCores() const {
    return nCores;
}

int Scheduler::getMakeSpan() const {
    return makeSpan;
}

int Scheduler::backtrack(const int index, std::vector<std::vector<Task>>& schedule, const int current) {
    if (index == tasks.size() || current >= makeSpan) {
        if (current < makeSpan) {
            makeSpan = current;
            bestSchedule = schedule;
        }
        return current;
    }

    int minMakeSpan = std::numeric_limits<int>::max();
    double factor = 1.0;

    for (int coreIndex = 0; coreIndex < nCores; ++coreIndex) {
        factor = slowFactor[coreIndex];

        std::shared_ptr<Task> task = tasks[index];
        int start = 0;
        if (!schedule[coreIndex].empty()) {
            start = schedule[coreIndex][schedule[coreIndex].size() - 1].getEnd();
        }
        schedule[coreIndex].push_back(*task);
        schedule[coreIndex][schedule[coreIndex].size() - 1].scheduleTask(start, coreIndex, factor);

        minMakeSpan = std::min(minMakeSpan, backtrack(index + 1, schedule,
                                                      std::max(current, schedule[coreIndex][
                                                              schedule[coreIndex].size() - 1
                                                              ].getEnd())));
        schedule[coreIndex].pop_back();

    }

    return minMakeSpan;
}

void Scheduler::execute() {
    std::vector<std::vector<Task>> schedule(nCores, std::vector<Task>());
    std::string tasksScheduled(tasks.size(), 'x');
    std::unordered_set<std::string> dp;
    makeSpan = backtrack(0, schedule, 0);
}

int Scheduler::backtrack(const int index, std::vector<std::vector<Task>>& schedule, const int current,
                         const int beginning, const int end) {
    if (index == end - beginning || current >= makeSpan) {
        if (current < makeSpan) {
            makeSpan = current;
            bestSchedule = schedule;
        }
        return current;
    }

    int minMakeSpan = std::numeric_limits<int>::max();
    double factor = 1.0;
    const int realIndex = index + beginning;
    for (int coreIndex = 0; coreIndex < nCores; ++coreIndex) {
        factor = slowFactor[coreIndex];
        std::shared_ptr<Task> task = tasks[realIndex];
        int start = 0;
        if (!schedule[coreIndex].empty()) {
            start = schedule[coreIndex][schedule[coreIndex].size() - 1].getEnd();
        }
        schedule[coreIndex].push_back(*task);
        schedule[coreIndex][schedule[coreIndex].size() - 1].scheduleTask(start, coreIndex, factor);

        minMakeSpan = std::min(minMakeSpan, backtrack(index + 1, schedule,
                                                      std::max(current, schedule[coreIndex][
                                                              schedule[coreIndex].size() - 1
                                                      ].getEnd()),
                                                      beginning, end));
        schedule[coreIndex].pop_back();
    }

    return minMakeSpan;
}

void Scheduler::executeBatch(const int start, const int end, const int current) {
    std::vector<std::vector<Task>> schedule(nCores, std::vector<Task>());
    if (!bestSchedule.empty()) {
        schedule = bestSchedule;
    }
    // reinitialize makeSpan or it is going to stop immediately
    makeSpan = std::numeric_limits<int>::max();
    makeSpan = backtrack(0, schedule, current, start, end);
}

std::unordered_map<int, std::shared_ptr<Task>> Scheduler::getTasks() const {
    return tasks;
}

void Scheduler::saveSchedule() {
    if (!bestSchedule.empty()) {
        for (int core = 0; core < nCores; ++core) {
            std::vector<Task>& coreSchedule = bestSchedule[core];
            for (Task& task : coreSchedule) {
                tasks[task.getId()]->scheduleTask(task.getStart(), core, slowFactor[core]);
            }
        }
    }
}

void Scheduler::printResult() {
    std::cout << "Heuristic found makeSpan found for " << tasks.size() << " tasks on " << nCores << " cores: ";
    std::cout << getMakeSpan() << std::endl;

    const std::unordered_map<int, std::shared_ptr<Task>> scheduledTasks = getTasks();

    for (const auto & kv : scheduledTasks) {
        std::shared_ptr<Task> scheduledTask = kv.second;
        std::cout << "task " << scheduledTask->getId() << " scheduled to core " << scheduledTask->getAssignedCoreIndex();
        std::cout << ", starting at " << scheduledTask->getStart() << " and finishing at ";
        std::cout << scheduledTask->getEnd() << std::endl;
    }
}