#pragma once
#include "Queue.h"
#include "Node.h"
#include <string>
#include <list>

PriorityQueue::PriorityQueue(float minimumDistances[], int size)
{
	this->size = size;
	this->minimumDistances = minimumDistances;
	this->indices = new int[this->size];
	for (int i = 0; i < this->size; i++)
	{
		indices[i] = i;
	}
    this->heapify();
}

int* PriorityQueue::GetIndices()
{
	return this->indices;
}

float* PriorityQueue::GetMinimumDistances()
{
	return this->minimumDistances;
}

int PriorityQueue::GetSize()
{
	return this->size;
}

void PriorityQueue::swap(int first_index, int second_index)
{
	int temp = this->indices[first_index];
    this->indices[first_index] = this->indices[second_index];
    this->indices[second_index] = temp;
}

bool PriorityQueue::isStrictlyGreater(int first_index, int second_index)
{
    return this->minimumDistances[this->indices[first_index]] > this->minimumDistances[this->indices[second_index]];
}

int PriorityQueue::propagateDown(int startingIndex)
{    
    int leftChildIndex = 2 * startingIndex + 1;
    int rightChildIndex = 2 * startingIndex + 2;

    if (leftChildIndex >= this->size) 
        return startingIndex;

    int minimumIndex = startingIndex;

    if (this->isStrictlyGreater(minimumIndex, leftChildIndex))
    {
        minimumIndex = leftChildIndex;
    }

    if ((rightChildIndex < this->size) && (this->isStrictlyGreater(minimumIndex, rightChildIndex)))
    {
        minimumIndex = rightChildIndex;
    }

    if (minimumIndex != startingIndex)
    {
        this->swap(minimumIndex, startingIndex);
    }

    return minimumIndex;
}

void PriorityQueue::fix(int index)
{
    int previous_index;
    int current_index = index;
    do
    {
        previous_index = current_index;
        current_index = this->propagateDown(previous_index);
    } while (previous_index != current_index);
}

void PriorityQueue::heapify()
{
    for (int i = this->size / 2; i >= 0; i--)
    {
        this->fix(i);
    }
}

//    /// <summary>
//    /// Swaps an element with its parent and returns its new location
//    /// </summary>
//    /// <param name="vec"></param>
//    /// <param name="queue"></param>
//    /// <param name="elem"></param>
//    /// <returns></returns>
//    int BubbleUp(float* vec, int* queue, int elem)
//    {
//        if (elem == 1)  // If already at the top then stop
//            return 1;
//
//        int parent = elem / 2;
//
//        if (vec[queue[parent]] > vec[queue[elem]]) // If Parent is bigger than child swap
//        {
//            Swap(parent, elem);
//            return parent;
//        }
//        return elem;
//
//    }
//};