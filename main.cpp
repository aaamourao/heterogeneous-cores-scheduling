#include <iostream>
#include <__random/random_device.h>
#include <random>
#include "src/Task.h"
#include "src/Scheduler.h"

const int numberOfTasks = 10;
const int numberOfFastCores = 4;
const int numberOfLowPowerCores = 4;
const double slowFactor = 1.8;

std::vector<Task> generateBenchmark() {
    std::vector<Task> tasks;

    std::random_device random;
    std::mt19937 generator(random());
    std::uniform_int_distribution<> distribution(1, 10000);
    for (int i = 0; i < numberOfTasks; ++i) {
        const int duration = distribution(generator);
        std::cout << "task " << i << " duration: " << duration << std::endl;
        Task task(duration);
        tasks.push_back(task);
    }
    return tasks;
}

void printResult(const Scheduler scheduler) {
    std::cout << "MakeSpan found for " << numberOfTasks << " tasks on ";
    std::cout << numberOfFastCores << " fast cores and " << numberOfLowPowerCores;
    std::cout << " low power cores which are " << slowFactor << " slower: ";
    std::cout << scheduler.getMakeSpan() << std::endl;

    const std::vector<std::shared_ptr<Task>> scheduledTasks = scheduler.getTasks();

    for (const auto & scheduledTask : scheduledTasks) {
        std::cout << "task " << scheduledTask->getId() << " scheduled to core " << scheduledTask->getAssignedCoreIndex();
        std::cout << ", starting at " << scheduledTask->getStart() << " and finishing at ";
        std::cout << scheduledTask->getEnd() << std::endl;
    }
}

int main() {
    std::vector<Task> tasks = generateBenchmark();
    Scheduler scheduler(numberOfFastCores, numberOfLowPowerCores, slowFactor);
    for (Task& task : tasks) {
        scheduler.addTask(task);
    }

    // Optimal
    scheduler.execute();
    scheduler.saveSchedule();

    printResult(scheduler);

    return 0;
}
