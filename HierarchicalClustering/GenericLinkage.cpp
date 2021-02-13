#include "GenericLinkage.h"
#include "Distance.h"
#include "Initialise.h"
#include <iostream>

template <typename T>
GenericLinkageStatus<T>::GenericLinkageStatus(
	int numberOfRows,
	int numberOfColumns,
	T* data,
	T* linkage) : queue(NULL, 0)
{
	this->numberOfRows = numberOfRows;
	this->numberOfColumns = numberOfColumns;
	this->data = data;

	InitialiseClusterLabels(&this->clusterLabels, numberOfRows);

	int minimumDistanceSizeLength = 2 * numberOfRows - 1;
	this->minimumDistances = new T[minimumDistanceSizeLength];
	this->nearestNeighbours = new int[minimumDistanceSizeLength];
	InitialiseNearestNeighbours(numberOfRows, numberOfColumns, this->nearestNeighbours, this->minimumDistances, this->data);

	this->queue = PriorityQueue<T>(this->minimumDistances, numberOfRows);

	int linkageSize = (numberOfRows - 1) * 3;
	this->linkage = linkage;

	int sizesLength = 2 * numberOfRows;
	this->sizes = new int[sizesLength];
	InitialiseSizes(sizes, numberOfRows);

	this->newClusterUpdate = CentroidUpdate<T>;
}

template <typename T>
GenericLinkageStatus<T>::~GenericLinkageStatus()
{
	delete[] minimumDistances;
	delete[] nearestNeighbours;
	delete[] sizes;
}

template <typename T>
void GenericLinkageStatus<T>::CombineSizes(int firstLocation, int secondLocation, int newLocation)
{
	this->sizes[newLocation] = this->sizes[firstLocation] + this->sizes[secondLocation];
}

template <typename T>
void GenericLinkageStatus<T>::SetLinkage(int depth, int firstCluster, int secondCluster, T distance)
{
	this->linkage[depth * 3 ] = std::min(firstCluster, secondCluster);
	this->linkage[depth * 3  + 1] = std::max(firstCluster, secondCluster);
	this->linkage[depth * 3  + 2] = distance;
}

template <typename T>
void GenericLinkageStatus<T>::InsertNewCluster(int depth, int firstCluster, int secondCluster, T distance)
{
	int newCluster = this->numberOfRows + depth;
	this->SetLinkage(depth, firstCluster, secondCluster, distance);
	this->CombineSizes(firstCluster, secondCluster, newCluster);

	this->newClusterUpdate(
		this->data,
		this->sizes,
		this->numberOfColumns,
		firstCluster,
		secondCluster,
		this->numberOfRows + depth);

	this->clusterLabels.erase(firstCluster);
	this->clusterLabels.erase(secondCluster);

	this->UpdateNearestNeighbours(newCluster);

	this->clusterLabels.insert(newCluster);

	this->queue.ReplaceElement(secondCluster, newCluster);
}

template <typename T>
void GenericLinkageStatus<T>::UpdateNearestNeighbours(int newCluster)
{
	T minimumDistance = this->minimumDistances[newCluster];
	int nearestNeighbour = 0;
	for (auto it = this->clusterLabels.begin(); it != this->clusterLabels.end(); ++it)
	{
		T currentDistance = SquaredEuclidean(data, newCluster, *it, this->numberOfColumns);
		if (currentDistance < minimumDistance)
		{
			minimumDistance = currentDistance;
			nearestNeighbour = *it;
		}
	}
	this->minimumDistances[newCluster] = std::sqrt(minimumDistance);
	this->nearestNeighbours[newCluster] = nearestNeighbour;
}

template <typename T>
void GenericLinkageStatus<T>::GetNextClustersToMerge(int* firstCluster, int* secondCluster, T* distance)
{
	this->queue.GetMinimum(firstCluster, distance);
	*secondCluster = this->nearestNeighbours[*firstCluster];

	while (this->clusterLabels.count(*secondCluster) == 0)
	{

		UpdateNearestNeighbourOfMinimumPoint(*firstCluster);

		this->queue.GetMinimum(firstCluster, distance);
		*secondCluster = nearestNeighbours[*firstCluster];
	}

	this->queue.RemoveMinimum();
}

template <typename T>
std::unordered_set<int>* GenericLinkageStatus<T>::GetClusterLabels()
{
	return &this->clusterLabels;
}

template <typename T>
PriorityQueue<T>* GenericLinkageStatus<T>::GetQueue()
{
	return &this->queue;
}

template <typename T>
T* GenericLinkageStatus<T>::GetData()
{
	return this->data;
}

template <typename T>
T* GenericLinkageStatus<T>::GetMinimumDistances()
{
	return this->minimumDistances;
}

template <typename T>
int* GenericLinkageStatus<T>::GetNearestNeighbours()
{
	return this->nearestNeighbours;
}

template <typename T>
T* GenericLinkageStatus<T>::GetLinkage()
{
	return this->linkage;
}

template <typename T>
int GenericLinkageStatus<T>::GetSize(int clusterIndex)
{
	return this->sizes[clusterIndex];
}

template <typename T>
void GenericLinkageStatus<T>::PrintLinkage()
{
	for (int i = 0; i < this->numberOfRows - 1; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			std::cout << this->linkage[i * 3 + j] << ", ";
		}
		std::cout << this->linkage[i * 3 + 2] << std::endl;
	}
}

template <typename T>
void GenericLinkageStatus<T>::UpdateNearestNeighbourOfMinimumPoint(int clusterIndex)
{
	T minimumDistance = std::numeric_limits<T>::max();
	std::unordered_set<int>::iterator setIterator;

	for (setIterator = this->clusterLabels.begin(); setIterator != this->clusterLabels.end(); setIterator++)
	{
		if (*setIterator == clusterIndex)
		{
			continue;
		}

		T current_distance = SquaredEuclidean(data, clusterIndex, *setIterator, numberOfColumns);

		if (current_distance < minimumDistance)
		{
			minimumDistance = current_distance;
			this->nearestNeighbours[clusterIndex] = *setIterator;
		}
	}

	this->queue.UpdateMinimum(std::sqrt(minimumDistance));
}

template <typename T>
void GenericLinkage(T* data, T* linkage, int numberOfRows, int numberOfColumns)
{
	int firstCluster, secondCluster;
	T distance{};

	auto currentStatus = GenericLinkageStatus<T>(numberOfRows, numberOfColumns, data, linkage);

	for (int depth = 0; depth < numberOfRows - 1; depth++)
	{
		currentStatus.GetNextClustersToMerge(&firstCluster, &secondCluster, &distance);
		currentStatus.InsertNewCluster(depth, firstCluster, secondCluster, distance);
	}
}

template void GenericLinkage<double>(double* data, double* linkage, int numberOfRows, int numberOfColumns);
template void GenericLinkage<float>(float* data, float* linkage, int numberOfRows, int numberOfColumns);