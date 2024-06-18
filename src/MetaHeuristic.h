//
// Created by Adriano Mourão on 18/06/24.
//

#ifndef NETWORKFLOWSTF_METAHEURISTIC_H
#define NETWORKFLOWSTF_METAHEURISTIC_H


#include "Scheduler.h"

class MetaHeuristic {
    std::shared_ptr<Scheduler> scheduler;
public:
    explicit MetaHeuristic(std::shared_ptr<Scheduler> aScheduler);

    void execute(int batchSize = 10);

    void saveSchedule();

    std::shared_ptr<Scheduler> getScheduler();

private:
    MetaHeuristic() {
        scheduler = nullptr;
    };
};


#endif //NETWORKFLOWSTF_METAHEURISTIC_H
