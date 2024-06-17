//
// Created by Adriano Mourão on 11/06/24.
//

#ifndef NETWORKFLOWSTF_SCHEDULER_H
#define NETWORKFLOWSTF_SCHEDULER_H


#include <vector>
#include "Task.h"

class Scheduler {
    int nFastCores;
    int nLowPowerCores;
    double slowFactor;
    std::vector<std::shared_ptr<Task>> tasks;
    int makeSpan;
public:
    explicit Scheduler(int aNFastCPUs, int aNLowPowerCPUs, double aSlowFactor);

    void addTask(Task task);

    int getNFastCores() const;

    int getNLowPowerCores() const;

    int getMakeSpan() const;

    [[nodiscard]] std::vector<std::shared_ptr<Task>> getTasks() const;

    void execute();

private:
    Scheduler() {
        nFastCores = 0;
        nLowPowerCores = 0;
        slowFactor = 0;
        makeSpan = -1;
    }

    int backtrack(int index, std::vector<std::vector<Task>>& schedule, int current,
                  std::vector<bool>& tasksScheduled, int totalScheduledTasks);
};


#endif //NETWORKFLOWSTF_SCHEDULER_H
