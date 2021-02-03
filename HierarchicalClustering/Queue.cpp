#include "Queue.h"
#include <stdexcept>

PriorityQueue::PriorityQueue(float minimumDistances[], int size)
{
	this->currentSize = size;
    this->capacity = size;
	this->minimumDistances = minimumDistances;
	this->indices = new int[this->currentSize];
	for (int i = 0; i < this->currentSize; i++)
	{
		indices[i] = i;
	}
    this->heapify();
}

void PriorityQueue::RemoveMinimum()
{
    if (this->currentSize == 0)
    {
        throw std::out_of_range("There are not any nodes in the queue to remove");
    }

    this->indices[0] = this->indices[this->currentSize - 1];
    this->currentSize -= 1;
    this->fixDownwards(0);
    
}

void PriorityQueue::GetMinimum(int* index, float* minimum_distance)
{
    if (this->currentSize == 0)
    {
        throw std::out_of_range("There are not any nodes in the queue");
    }
    
    *index = this->indices[0];
    *minimum_distance = this->minimumDistances[*index];
}

void PriorityQueue::UpdateMinimum(float distance)
{
    if (this->currentSize == 0)
    {
        throw std::out_of_range("There are not any nodes in the queue");
    }

    this->minimumDistances[this->indices[0]] = distance;
    this->fixDownwards(0);
}

void PriorityQueue::ReplaceElement(int originalLabel, int newLabel, float newMinimumDistance)
{
    this->minimumDistances[newLabel] = newMinimumDistance;

    bool found = false;
    int originalIndex;
    for (originalIndex = 0; originalIndex < this->currentSize; originalIndex++)
    {
        if (this->indices[originalIndex] == originalLabel)
        {
            found = true;
            break;
        }
    }

    if (!found)
    {
        throw std::runtime_error("The original node was not found in the queue");
    }

    int currentIndex = this->fixUpwards(originalIndex);
    if (currentIndex == originalIndex)
    {
        this->fixDownwards(originalIndex);
    }
}

int* PriorityQueue::GetIndices()
{
	return this->indices;
}

float* PriorityQueue::GetMinimumDistances()
{
	return this->minimumDistances;
}

int PriorityQueue::GetCurrentSize()
{
	return this->currentSize;
}

int PriorityQueue::GetCapacity()
{
    return this->capacity;
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

int PriorityQueue::propagateUp(int startingIndex)
{
    if (startingIndex == 0)
    {
        return 0;
    }

    int parentIndex = startingIndex / 2;

    if (this->isStrictlyGreater(parentIndex, startingIndex))
    {
        this->swap(parentIndex, startingIndex);
        return parentIndex;
    }
    return startingIndex;
}

int PriorityQueue::propagateDown(int startingIndex)
{    
    int leftChildIndex = 2 * startingIndex + 1;
    int rightChildIndex = 2 * startingIndex + 2;

    if (leftChildIndex >= this->currentSize) 
        return startingIndex;

    int minimumIndex = startingIndex;

    if (this->isStrictlyGreater(minimumIndex, leftChildIndex))
    {
        minimumIndex = leftChildIndex;
    }

    if ((rightChildIndex < this->currentSize) && (this->isStrictlyGreater(minimumIndex, rightChildIndex)))
    {
        minimumIndex = rightChildIndex;
    }

    if (minimumIndex != startingIndex)
    {
        this->swap(minimumIndex, startingIndex);
    }

    return minimumIndex;
}

int PriorityQueue::fixDownwards(int index)
{
    int previous_index;
    int current_index = index;
    do
    {
        previous_index = current_index;
        current_index = this->propagateDown(previous_index);
    } while (previous_index != current_index);
    return current_index;
}

int PriorityQueue::fixUpwards(int index)
{
    int previous_index;
    int current_index = index;
    do
    {
        previous_index = current_index;
        current_index = this->propagateUp(previous_index);
    } while (previous_index != current_index);
    return current_index;
}

void PriorityQueue::heapify()
{
    for (int i = this->currentSize / 2; i >= 0; i--)
    {
        this->fixDownwards(i);
    }
}