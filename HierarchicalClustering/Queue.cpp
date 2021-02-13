#include "Queue.h"
#include <stdexcept>

template <typename T>
PriorityQueue<T>::PriorityQueue(T minimumDistances[], int size)
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

template <typename T>
void PriorityQueue<T>::RemoveMinimum()
{
    if (this->currentSize == 0)
    {
        throw std::out_of_range("There are not any nodes in the queue to remove");
        std::exit(EXIT_FAILURE);
    }

    this->indices[0] = this->indices[this->currentSize - 1];
    this->currentSize -= 1;
    this->fixDownwards(0);
}

template <typename T>
void PriorityQueue<T>::GetMinimum(int* index, T* minimum_distance)
{
    if (this->currentSize == 0)
    {
        throw std::out_of_range("There are not any nodes in the queue");
        std::exit(EXIT_FAILURE);
    }
    
    *index = this->indices[0];
    *minimum_distance = this->minimumDistances[*index];
}

template <typename T>
void PriorityQueue<T>::UpdateMinimum(T distance)
{
    if (this->currentSize == 0)
    {
        throw std::out_of_range("There are not any nodes in the queue");
        std::exit(EXIT_FAILURE);
    }

    this->minimumDistances[this->indices[0]] = distance;
    this->fixDownwards(0);
}

template <typename T>
void PriorityQueue<T>::ReplaceElement(int originalLabel, int newLabel)
{
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
        std::exit(EXIT_FAILURE);
    }

    this->indices[originalIndex] = newLabel;

    int currentIndex = this->fixUpwards(originalIndex);
    if (currentIndex == originalIndex)
    {
        this->fixDownwards(originalIndex);
    }
}

template <typename T>
int* PriorityQueue<T>::GetIndices()
{
	return this->indices;
}

template <typename T>
T* PriorityQueue<T>::GetMinimumDistances()
{
	return this->minimumDistances;
}

template <typename T>
int PriorityQueue<T>::GetCurrentSize()
{
	return this->currentSize;
}

template <typename T>
int PriorityQueue<T>::GetCapacity()
{
    return this->capacity;
}

template <typename T>
void PriorityQueue<T>::swap(int first_index, int second_index)
{
	int temp = this->indices[first_index];
    this->indices[first_index] = this->indices[second_index];
    this->indices[second_index] = temp;
}

template <typename T>
bool PriorityQueue<T>::isStrictlyGreater(int first_index, int second_index)
{
    return this->minimumDistances[this->indices[first_index]] > this->minimumDistances[this->indices[second_index]];
}

template <typename T>
int PriorityQueue<T>::propagateUp(int startingIndex)
{
    if (startingIndex == 0)
    {
        return 0;
    }

    int parentIndex = (startingIndex + 1) / 2 - 1;

    if (this->isStrictlyGreater(parentIndex, startingIndex))
    {
        this->swap(parentIndex, startingIndex);
        return parentIndex;
    }
    return startingIndex;
}

template <typename T>
int PriorityQueue<T>::propagateDown(int startingIndex)
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

template <typename T>
int PriorityQueue<T>::fixDownwards(int index)
{
    int previous_index = index;
    int current_index = index;
    do
    {
        previous_index = current_index;
        current_index = this->propagateDown(previous_index);
    } while (previous_index != current_index);
    return current_index;
}

template <typename T>
int PriorityQueue<T>::fixUpwards(int index)
{
    int previous_index = index;
    int current_index = index;
    do
    {
        previous_index = current_index;
        current_index = this->propagateUp(previous_index);
    } while (previous_index != current_index);
    return current_index;
}

template <typename T>
void PriorityQueue<T>::heapify()
{
    for (int i = this->currentSize / 2; i >= 0; i--)
    {
        this->fixDownwards(i);
    }
}

template PriorityQueue<double>;
template PriorityQueue<float>;