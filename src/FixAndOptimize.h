//
// Created by Adriano Mourão on 23/06/24.
//

#ifndef NETWORKFLOWSTF_FIXANDOPTIMIZE_H
#define NETWORKFLOWSTF_FIXANDOPTIMIZE_H


#include "Model.h"

class FixAndOptimize {
    std::shared_ptr<Model> model;
    int windowSize;
    std::vector<IloRange> fixedVarConstraints;
public:
    explicit FixAndOptimize(const std::shared_ptr<Model>& aModel, int aWindowSize = 10);

    double execute(const std::unordered_map<int, std::shared_ptr<Task>>& taskSchedule);

private:
    FixAndOptimize() {
        windowSize = 0;
        model = nullptr;
    };

    void fix(int start, int end, const std::unordered_map<int, std::shared_ptr<Task>>& taskSchedule);

    void optimize();

    void removeFixed();
};


#endif //NETWORKFLOWSTF_FIXANDOPTIMIZE_H
