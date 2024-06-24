#include <iostream>
#include <__random/random_device.h>
#include <random>
#include "src/Task.h"
#include "src/Scheduler.h"
#include "src/MetaHeuristic.h"
#include "src/Model.h"
#include "src/FixAndOptimize.h"

const int numberOfTasks = 100;
const int numberOfCores = 10;


std::pair<std::vector<std::shared_ptr<Task>>, std::vector<double>> generateBenchmark() {
    std::vector<std::shared_ptr<Task>> tasks;

    std::random_device random;
    std::mt19937 generator(random());
    std::uniform_int_distribution<> distribution(10, 100);
    for (int i = 0; i < numberOfTasks; ++i) {
        const int duration = distribution(generator);
        std::cout << "task " << i << " duration: " << duration << std::endl;
        tasks.push_back(std::make_shared<Task>(duration));
    }
    std::uniform_real_distribution slowFactorDistribution(1.0, 2.0);
    std::vector<double> factor;
    for (int core = 0; core < numberOfCores; ++core) {
        const double slowFactor = slowFactorDistribution(generator);
        factor.push_back(slowFactor);
        std::cout << "Core " << core << " speed is " << slowFactor << std::endl;
    }
    return {tasks, factor};
}

int main() {
    auto benchmark = generateBenchmark();
    std::vector<std::shared_ptr<Task>> tasks = benchmark.first;
    std::vector<double> slowFactor = benchmark.second;
    std::shared_ptr<Scheduler> scheduler = std::make_shared<Scheduler>(numberOfCores, slowFactor);
    for (std::shared_ptr<Task>& task : tasks) {
        scheduler->addTask(task);
    }

    // Optimal
    //scheduler->execute();
    //scheduler->saveSchedule();
    //printResult(scheduler);

    //scheduler->reset();

    MetaHeuristic meta = MetaHeuristic(scheduler);
    meta.execute(10);
    meta.saveSchedule();



    std::shared_ptr<Model> model = std::make_shared<Model>(numberOfCores, tasks, slowFactor);
    model->importIncumbentSolution(scheduler->getTasks());
    // model->solve();
    // std::cout << "solved model " << model->getStatus() << std::endl;
    // model->getSchedule();

    FixAndOptimize fixAndOptimize(model, 10);
    fixAndOptimize.execute(scheduler->getTasks());

    std::cout << "result found my heuristic: " << scheduler->getMakeSpan() << std::endl;
    std::cout << "result found by FixAndOptimize: " << fixAndOptimize.getMakeSpan() << std::endl;

    return 0;
}
