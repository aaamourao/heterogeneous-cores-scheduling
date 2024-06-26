//
// Created by Adriano Mourão on 11/06/24.
//

#ifndef NETWORKFLOWSTF_SCHEDULER_H
#define NETWORKFLOWSTF_SCHEDULER_H

#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include "Task.h"

class Scheduler {
    int nCores;
    std::vector<double> slowFactor;
    std::unordered_map<int, std::shared_ptr<Task>> tasks;
    std::vector<std::vector<Task>> bestSchedule;

    int makeSpan;
public:
    explicit Scheduler(int aNCores, const std::vector<double>& aSlowFactor);

    void addTask(std::shared_ptr<Task>& task);

    int getNCores() const;

    int getMakeSpan() const;

    [[nodiscard]] std::unordered_map<int, std::shared_ptr<Task>> getTasks() const;

    void execute();

    void saveSchedule();

    void executeBatch(int start, int end, int current);

    void reset();

    void printResult();

private:
    Scheduler() {
        nCores = 0;
        makeSpan = -1;
    }

    int backtrack(int index, std::vector<std::vector<Task>> &schedule, int current);

    int backtrack(int index, std::vector<std::vector<Task>> &schedule, int current, int beginning, int end);

};


#endif //NETWORKFLOWSTF_SCHEDULER_H
