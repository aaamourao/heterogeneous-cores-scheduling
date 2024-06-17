//
// Created by Adriano Mourão on 11/06/24.
//

#include "Task.h"

int Task::nextId = 0;

Task::Task(const int aDuration) : duration(aDuration) {
    start = -1;
    end = -1;
    assignedCoreIndex = -1;
    id = nextId;
    ++nextId;
}

Task::Task(const Task& other) {
    start = other.start;
    end = other.end;
    assignedCoreIndex = other.assignedCoreIndex;
    duration = other.duration;
    id = other.id;
}

int Task::getDuration() const {
    return duration;
}

int Task::getStart() const {
    return start;
}

int Task::getEnd() const {
    return end;
}

int Task::getAssignedCoreIndex() const {
    return assignedCoreIndex;
}

void Task::scheduleTask(const int aStart, const int aCoreIndex, const double aSlowFactor) {
    start = aStart;
    assignedCoreIndex = aCoreIndex;
    end = start + static_cast<int>(static_cast<double>(duration) * aSlowFactor);
}

int Task::getId() const {
    return id;
}