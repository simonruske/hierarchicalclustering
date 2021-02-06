#include "GenericLinkage.h"
#include "Distance.h"
#include "Initialise.h"
#include <iostream>

GenericLinkageStatus::GenericLinkageStatus(
	int numberOfRows,
	int numberOfColumns,
	float* data,
	std::function<float(float*, int, int, int)> metric) : queue(new float[0]{}, 0)
{
	this->numberOfRows = numberOfRows;
	this->numberOfColumns = numberOfColumns;
	this->data = data;
	this->metric = metric;

	InitialiseClusterLabels(&this->clusterLabels, numberOfRows);

	int minimumDistanceSizeLength = 2 * numberOfRows - 1;
	this->minimumDistances = new float[minimumDistanceSizeLength];
	this->nearestNeighbours = new int[minimumDistanceSizeLength];
	InitialiseNearestNeighbours(numberOfRows, numberOfColumns, this->nearestNeighbours, this->minimumDistances, this->data, this->metric);

	this->queue = PriorityQueue(this->minimumDistances, numberOfRows);

	int linkageSize = (numberOfRows - 1) * 3;
	this->linkage = new float[linkageSize];

	int sizesLength = 2 * numberOfRows;
	this->sizes = new int[sizesLength];
	InitialiseSizes(sizes, numberOfRows);

	this->newClusterUpdate = CentroidUpdate;
}

void GenericLinkageStatus::CombineSizes(int firstLocation, int secondLocation, int newLocation)
{
	this->sizes[newLocation] = this->sizes[firstLocation] + this->sizes[secondLocation];
}

void GenericLinkageStatus::SetLinkage(int depth, int firstCluster, int secondCluster, float distance)
{
	this->linkage[depth * 3 ] = std::min(firstCluster, secondCluster);
	this->linkage[depth * 3  + 1] = std::max(firstCluster, secondCluster);
	this->linkage[depth * 3  + 2] = distance;
}

void GenericLinkageStatus::InsertNewCluster(int depth, int firstCluster, int secondCluster, float distance)
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

void GenericLinkageStatus::UpdateNearestNeighbours(int newCluster)
{
	for (auto it = this->clusterLabels.begin(); it != this->clusterLabels.end(); ++it)
	{
		float currentDistance = this->metric(data, newCluster, *it, this->numberOfColumns);
		if (currentDistance < this->minimumDistances[newCluster])
		{
			this->minimumDistances[newCluster] = currentDistance;
			this->nearestNeighbours[newCluster] = *it;
		}
	}
}

std::unordered_set<int> GenericLinkageStatus::GetClusterLabels()
{
	return this->clusterLabels;
}

PriorityQueue* GenericLinkageStatus::GetQueue()
{
	return &this->queue;
}

float* GenericLinkageStatus::GetData()
{
	return this->data;
}

float* GenericLinkageStatus::GetMinimumDistances()
{
	return this->minimumDistances;
}

int* GenericLinkageStatus::GetNearestNeighbours()
{
	return this->nearestNeighbours;
}

float* GenericLinkageStatus::GetLinkage()
{
	return this->linkage;
}

int GenericLinkageStatus::GetSize(int clusterIndex)
{
	return this->sizes[clusterIndex];
}

void GenericLinkageStatus::PrintLinkage()
{
	for (int i = 0; i < this->numberOfRows - 1; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			std::cout << linkage[i * this->numberOfColumns + j] << ", ";
		}
		std::cout << linkage[i * this->numberOfColumns + 2] << std::endl;
	}
}

void GetNextClustersToMerge(
	std::unordered_set<int> clusterIndices,
	PriorityQueue *queue,
	int numberOfColumns,
	float* data,
	int* nearestNeightbours,
	int* firstCluster,
	int* secondCluster,
	float* distance,
	std::function<float(float*, int, int, int)> metric)
{
	(*queue).GetMinimum(firstCluster, distance);
	*secondCluster = nearestNeightbours[*firstCluster];

	while (clusterIndices.count(*secondCluster) == 0)
	{
		
		UpdateNearestNeighbourOfMinimumPoint(clusterIndices, *queue, numberOfColumns, data, nearestNeightbours, *firstCluster, metric);

		(*queue).GetMinimum(firstCluster, distance);
		*secondCluster = nearestNeightbours[*firstCluster];
	}

	(*queue).RemoveMinimum();
}

void UpdateNearestNeighbourOfMinimumPoint(
	std::unordered_set<int> clusterIndices,
	PriorityQueue queue,
	int numberOfColumns,
	float* data,
	int* nearestNeightbours,
	int clusterIndex,
	std::function<float(float*, int, int, int)> metric)
{
	float minimumDistance = std::numeric_limits<float>::max();
	std::unordered_set<int>::iterator setIterator;

	for (setIterator = clusterIndices.begin(); setIterator != clusterIndices.end(); setIterator++)
	{
		if (*setIterator <= clusterIndex)
		{
			continue;
		}

		float current_distance = metric(data, clusterIndex, *setIterator, numberOfColumns);

		if (current_distance < minimumDistance)
		{
			minimumDistance = current_distance;
			nearestNeightbours[clusterIndex] = *setIterator;
		}
	}

	queue.UpdateMinimum(minimumDistance);
}

GenericLinkageStatus GenericLinkage(float* data, int numberOfRows, int numberOfColumns, std::function<float(float*, int, int, int)> metric)
{
	int firstCluster, secondCluster;
	float distance;

	auto currentStatus = GenericLinkageStatus(numberOfRows, numberOfColumns, data, metric);

	for (int depth = 0; depth < numberOfRows - 1; depth++)
	{
		GetNextClustersToMerge(
			currentStatus.GetClusterLabels(),
			currentStatus.GetQueue(),
			numberOfColumns,
			currentStatus.GetData(),
			currentStatus.GetNearestNeighbours(),
			&firstCluster,
			&secondCluster,
			&distance,
			metric);

		currentStatus.InsertNewCluster(depth, firstCluster, secondCluster, distance);
	}
	return currentStatus;
}