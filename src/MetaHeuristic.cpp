//
// Created by Adriano Mourão on 18/06/24.
//

#include "MetaHeuristic.h"

#include <utility>
#include <iostream>

MetaHeuristic::MetaHeuristic(std::shared_ptr<Scheduler> aScheduler) : scheduler(std::move(aScheduler)){

}

void MetaHeuristic::execute(const int batchSize) {
    const int tasksSize = static_cast<int>(scheduler->getTasks().size());
    int iterations = tasksSize / batchSize;
    if (tasksSize % batchSize != 0) {
        ++iterations;
    }
    int start = 0;
    int current = 0;
    for (int i = 0; i < iterations; ++i) {
        scheduler->executeBatch(start, std::min(tasksSize, start + batchSize), current);
        current = scheduler->getMakeSpan();
        std::cout << current << std::endl;
        start += batchSize;
    }
}

std::shared_ptr<Scheduler> MetaHeuristic::getScheduler() {
    return scheduler;
}

void MetaHeuristic::saveSchedule() {
    scheduler->saveSchedule();
}