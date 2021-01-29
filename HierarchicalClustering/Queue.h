#pragma once

class PriorityQueue
{
public:
    
    PriorityQueue(float minimumDistances[], int size);

    int* GetIndices();
    float* GetMinimumDistances();
    int GetSize();

private:
    int size;
    int* indices;
    float* minimumDistances;

    void swap(int first_index, int second_index);
    bool isStrictlyGreater(int first_index, int second_index);
    int propagateDown(int starting_index);
    void heapify();
    void fix(int);
};
