//
// Created by Adriano Mourão on 18/06/24.
//

#ifndef NETWORKFLOWSTF_METAHEURISTIC_H
#define NETWORKFLOWSTF_METAHEURISTIC_H


#include "Scheduler.h"

class MetaHeuristic {
    Scheduler scheduler;
public:
    MetaHeuristic(Scheduler aScheduler);

    void execute(int batchSize = 10);
};


#endif //NETWORKFLOWSTF_METAHEURISTIC_H
