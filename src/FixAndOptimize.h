//
// Created by Adriano Mourão on 23/06/24.
//

#ifndef NETWORKFLOWSTF_FIXANDOPTIMIZE_H
#define NETWORKFLOWSTF_FIXANDOPTIMIZE_H


#include "Model.h"

class FixAndOptimize {
    std::shared_ptr<Model> model;
    int windowSize;
public:
    explicit FixAndOptimize(const std::shared_ptr<Model>& aModel, int aWindowSize = 10);

private:
    FixAndOptimize() {
        windowSize = 0;
        model = nullptr;
    };
};


#endif //NETWORKFLOWSTF_FIXANDOPTIMIZE_H
