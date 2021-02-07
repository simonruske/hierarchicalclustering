#include "GenericLinkage.h"
#include "Distance.h"
#include "Initialise.h"
#include <iostream>

GenericLinkageStatus::GenericLinkageStatus(
	int numberOfRows,
	int numberOfColumns,
	float* data) : queue(new float[0]{}, 0)
{
	this->numberOfRows = numberOfRows;
	this->numberOfColumns = numberOfColumns;
	this->data = data;

	InitialiseClusterLabels(&this->clusterLabels, numberOfRows);

	int minimumDistanceSizeLength = 2 * numberOfRows - 1;
	this->minimumDistances = new float[minimumDistanceSizeLength];
	this->nearestNeighbours = new int[minimumDistanceSizeLength];
	InitialiseNearestNeighbours(numberOfRows, numberOfColumns, this->nearestNeighbours, this->minimumDistances, this->data);

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
	float minimumDistance = this->minimumDistances[newCluster];
	int nearestNeighbour = 0;
	for (auto it = this->clusterLabels.begin(); it != this->clusterLabels.end(); ++it)
	{
		float currentDistance = SquaredEuclidean(data, newCluster, *it, this->numberOfColumns);
		if (currentDistance < minimumDistance)
		{
			minimumDistance = currentDistance;
			nearestNeighbour = *it;
		}
	}
	this->minimumDistances[newCluster] = std::sqrt(minimumDistance);
	this->nearestNeighbours[newCluster] = nearestNeighbour;
}

void GenericLinkageStatus::GetNextClustersToMerge(int* firstCluster, int* secondCluster, float* distance)
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

std::unordered_set<int>* GenericLinkageStatus::GetClusterLabels()
{
	return &this->clusterLabels;
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
			std::cout << this->linkage[i * 3 + j] << ", ";
		}
		std::cout << this->linkage[i * 3 + 2] << std::endl;
	}
}

void GenericLinkageStatus::UpdateNearestNeighbourOfMinimumPoint(int clusterIndex)
{
	float minimumDistance = std::numeric_limits<float>::max();
	std::unordered_set<int>::iterator setIterator;

	for (setIterator = this->clusterLabels.begin(); setIterator != this->clusterLabels.end(); setIterator++)
	{
		if (*setIterator == clusterIndex)
		{
			continue;
		}

		float current_distance = SquaredEuclidean(data, clusterIndex, *setIterator, numberOfColumns);

		if (current_distance < minimumDistance)
		{
			minimumDistance = current_distance;
			this->nearestNeighbours[clusterIndex] = *setIterator;
		}
	}

	this->queue.UpdateMinimum(std::sqrt(minimumDistance));
}

GenericLinkageStatus GenericLinkage(float* data, int numberOfRows, int numberOfColumns)
{
	int firstCluster, secondCluster;
	float distance;

	auto currentStatus = GenericLinkageStatus(numberOfRows, numberOfColumns, data);

	for (int depth = 0; depth < numberOfRows - 1; depth++)
	{
		currentStatus.GetNextClustersToMerge(&firstCluster, &secondCluster, &distance);
		currentStatus.InsertNewCluster(depth, firstCluster, secondCluster, distance);
	}

	return currentStatus;
}