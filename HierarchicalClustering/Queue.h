#pragma once

template <typename T>
class PriorityQueue
{
public:
    
    PriorityQueue(T minimumDistances[], int size);

    void GetMinimum(int* index, T* minimum_distance);
    void UpdateMinimum(T distance);
    void RemoveMinimum();
    void ReplaceElement(int originalLabel, int newLabel);
    
    int* GetIndices();
    T* GetMinimumDistances();
    int GetCurrentSize();
    int GetCapacity();


private:
    int currentSize;
    int capacity;

    int* indices;
    T* minimumDistances;

    void swap(int first_index, int second_index);
    bool isStrictlyGreater(int first_index, int second_index);
    int propagateUp(int start_index);
    int propagateDown(int starting_index);
    int fixUpwards(int index);
    int fixDownwards(int index);
    void heapify();
};
