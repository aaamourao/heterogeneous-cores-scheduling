//
// Created by Adriano Mourão on 11/06/24.
//

#ifndef NETWORKFLOWSTF_SCHEDULER_H
#define NETWORKFLOWSTF_SCHEDULER_H

#include <unordered_map>
#include <unordered_set>
#include <vector>
#include "Task.h"

class Scheduler {
    int nFastCores;
    int nLowPowerCores;
    double slowFactor;
    std::vector<std::shared_ptr<Task>> tasks;
    std::vector<std::vector<Task>> bestSchedule;
    int makeSpan;
public:
    explicit Scheduler(int aNFastCPUs, int aNLowPowerCPUs, double aSlowFactor);

    void addTask(Task task);

    [[maybe_unused]] int getNFastCores() const;

    int getNLowPowerCores() const;

    int getMakeSpan() const;

    [[nodiscard]] std::vector<std::shared_ptr<Task>> getTasks() const;

    void execute();

    void saveSchedule();

    void executeBatch(int start, int end);

private:
    Scheduler() {
        nFastCores = 0;
        nLowPowerCores = 0;
        slowFactor = 0;
        makeSpan = -1;
    }

    int backtrack(int index, std::vector<std::vector<Task>> &schedule, int current,
                  std::string &tasksScheduled, int totalScheduledTasks, std::unordered_set<std::string> &dp);

    int backtrack(int index, std::vector<std::vector<Task>> &schedule, int current,
                  std::string &tasksScheduled, int totalScheduledTasks, std::unordered_set<std::string> &dp,
                  int beginning, int end);
};


#endif //NETWORKFLOWSTF_SCHEDULER_H
