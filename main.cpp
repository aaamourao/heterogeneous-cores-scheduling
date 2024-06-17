#include <iostream>
#include <__random/random_device.h>
#include <random>
#include "src/Task.h"
#include "src/Scheduler.h"

const int numberOfTasks = 3;
const int numberOfFastCores = 1;
const int numberOfLowPowerCores = 1;
const double slowFactor = 1.5;

int main() {
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
    Scheduler scheduler(numberOfFastCores, numberOfLowPowerCores, slowFactor);
    for (Task& task : tasks) {
        scheduler.addTask(task);
    }

    scheduler.execute();

    std::cout << "MakeSpan found for " << numberOfTasks << " tasks on ";
    std::cout << numberOfFastCores << " fast cores and " << numberOfLowPowerCores;
    std::cout << " low power cores which are " << slowFactor << " slower: ";
    std::cout << scheduler.getMakeSpan() << std::endl;

    std::vector<std::shared_ptr<Task>> scheduledTasks = scheduler.getTasks();

    for (int i = 0; i < tasks.size(); ++i) {
        std::cout << "task " << scheduledTasks[i]->getId() << " scheduled to core " << scheduledTasks[i]->getAssignedCoreIndex();
        std::cout << ", starting at " << scheduledTasks[i]->getStart() << " and finishing at ";
        std::cout << scheduledTasks[i]->getEnd() << std::endl;
    }

    return 0;
}
