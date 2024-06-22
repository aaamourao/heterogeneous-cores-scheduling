//
// Created by Adriano Mourão on 11/06/24.
//

#ifndef NETWORKFLOWSTF_TASK_H
#define NETWORKFLOWSTF_TASK_H


class Task {
    int duration;
    int start;
    int end;
    int assignedCoreIndex;
    static int nextId;
    int id;

public:
    explicit Task(int aDuration);

    Task(const Task& other);

    int getDuration() const;

    int getStart() const;

    int getEnd() const;

    int getId() const;

    int getAssignedCoreIndex() const;

    void scheduleTask(int aStart, int coreIndex, double aSlowFactor = 1.0);

    bool operator==(const Task& rhs) const;
private:
    Task() {}
};


#endif //NETWORKFLOWSTF_TASK_H
