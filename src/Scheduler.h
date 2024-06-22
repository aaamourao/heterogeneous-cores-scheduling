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
    int nFastCores;
    int nLowPowerCores;
    double slowFactor;
    std::vector<std::shared_ptr<Task>> tasks;
    std::vector<std::vector<Task>> bestSchedule;

    int makeSpan;
public:
    explicit Scheduler(int aNFastCores, int aNLowPowerCores, double aSlowFactor);

    void addTask(Task task);

    [[maybe_unused]] int getNFastCores() const;

    int getNLowPowerCores() const;

    int getMakeSpan() const;

    [[nodiscard]] std::vector<std::shared_ptr<Task>> getTasks() const;

    void execute();

    void saveSchedule();

    void executeBatch(int start, int end, int current);

    void reset();

private:
    Scheduler() {
        nFastCores = 0;
        nLowPowerCores = 0;
        slowFactor = 0;
        makeSpan = -1;
    }

    static int fastCores;
    static int lowPowerCores;

    struct SchedulerHasher {
        std::string operator()(const std::vector<std::vector<Task>>& schedule) const {

            std::vector<int> fastCoresSchedule;
            for (int i = 0; i < fastCores; ++i) {
                const int end = schedule[i].empty() ? 0 : schedule[i][schedule[i].size() - 1].getEnd();
                fastCoresSchedule.push_back(end);
            }
            std::vector<int> lowPowerCoresSchedule;
            for (int i = fastCores; i < fastCores + lowPowerCores; ++i) {
                const int end = schedule[i].empty() ? 0 : schedule[i][schedule[i].size() - 1].getEnd();
                lowPowerCoresSchedule.push_back(end);
            }
            std::sort(fastCoresSchedule.begin(), fastCoresSchedule.end());
            std::sort(lowPowerCoresSchedule.begin(), lowPowerCoresSchedule.end());
            std::string hashable;
            for (int end : fastCoresSchedule) {
                hashable += std::to_string(end) + "#";
            }
            for (int end : lowPowerCoresSchedule) {
                hashable += std::to_string(end) + "#";
            }
            return hashable;
        }
    };

    int backtrack(int index, std::vector<std::vector<Task>> &schedule, int current,
                  std::unordered_set<std::string>& dp);

    int backtrack(int index, std::vector<std::vector<Task>> &schedule, int current,
                  std::string &tasksScheduled, std::unordered_set<std::string> &dp, int beginning, int end);

};


#endif //NETWORKFLOWSTF_SCHEDULER_H
