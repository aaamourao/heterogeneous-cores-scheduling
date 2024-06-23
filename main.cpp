#include <iostream>
#include <__random/random_device.h>
#include <random>
#include "src/Task.h"
#include "src/Scheduler.h"
#include "src/MetaHeuristic.h"
#include "src/Model.h"

const int numberOfTasks = 20;
const int numberOfFastCores = 1;
const int numberOfLowPowerCores = 1;
const double slowFactor = 1.8;

std::vector<std::shared_ptr<Task>> generateBenchmark() {
    std::vector<std::shared_ptr<Task>> tasks;

    std::random_device random;
    std::mt19937 generator(random());
    std::uniform_int_distribution<> distribution(10, 100);
    for (int i = 0; i < numberOfTasks; ++i) {
        const int duration = distribution(generator);
        std::cout << "task " << i << " duration: " << duration << std::endl;
        tasks.push_back(std::make_shared<Task>(duration));
    }
    return tasks;
}

void printResult(const std::shared_ptr<Scheduler>& scheduler) {
    std::cout << "MakeSpan found for " << numberOfTasks << " tasks on ";
    std::cout << numberOfFastCores << " fast cores and " << numberOfLowPowerCores;
    std::cout << " low power cores which are " << slowFactor << " slower: ";
    std::cout << scheduler->getMakeSpan() << std::endl;

    const std::unordered_map<int, std::shared_ptr<Task>> scheduledTasks = scheduler->getTasks();

    for (const auto & kv : scheduledTasks) {
        std::shared_ptr<Task> scheduledTask = kv.second;
        std::cout << "task " << scheduledTask->getId() << " scheduled to core " << scheduledTask->getAssignedCoreIndex();
        std::cout << ", starting at " << scheduledTask->getStart() << " and finishing at ";
        std::cout << scheduledTask->getEnd() << std::endl;
    }
}

int main() {
    std::vector<std::shared_ptr<Task>> tasks = generateBenchmark();
    std::shared_ptr<Scheduler> scheduler = std::make_shared<Scheduler>(numberOfFastCores,
                                                                       numberOfLowPowerCores,slowFactor);
    for (std::shared_ptr<Task>& task : tasks) {
        scheduler->addTask(task);
    }

    // Optimal
    //scheduler->execute();
    //scheduler->saveSchedule();
    //printResult(scheduler);

    //scheduler->reset();

    //MetaHeuristic meta = MetaHeuristic(scheduler);
    //meta.execute(5);
    //meta.saveSchedule();

    //printResult(meta.getScheduler());
    Model model(numberOfFastCores, numberOfLowPowerCores, tasks, slowFactor);
    model.solve();
    std::cout << "solved model " << model.getStatus() << std::endl;
    model.getSchedule();

    return 0;
}
