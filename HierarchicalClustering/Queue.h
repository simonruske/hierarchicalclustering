#pragma once

class PriorityQueue
{
public:
    
    PriorityQueue(float minimumDistances[], int size);

    void GetMinimum(int* index, float* minimum_distance);
    void UpdateMinimum(float distance);
    void RemoveMinimum();
    
    int* GetIndices();
    float* GetMinimumDistances();
    int GetCurrentSize();
    int GetCapacity();


private:
    int currentSize;
    int capacity;

    int* indices;
    float* minimumDistances;

    void swap(int first_index, int second_index);
    bool isStrictlyGreater(int first_index, int second_index);
    int propagateUp(int start_index);
    int propagateDown(int starting_index);
    void fix(int);
    void heapify();
};
