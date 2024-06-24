//
// Created by Adriano Mourão on 23/06/24.
//

#include "FixAndOptimize.h"

FixAndOptimize::FixAndOptimize(const std::shared_ptr<Model>& aModel, const int aWindowSize)
: windowSize(aWindowSize), model(aModel) {

}