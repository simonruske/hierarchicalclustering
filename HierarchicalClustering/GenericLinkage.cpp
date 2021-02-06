#include "GenericLinkage.h"
#include "Distance.h"
#include "Initialise.h"
#include <iostream>

GenericLinkageStatus::GenericLinkageStatus(int numberOfRows, int numberOfColumns, float* data) : queue(new float[0]{}, 0)
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
	this->linkage[depth * 3 ] = firstCluster;
	this->linkage[depth * 3  + 1] = secondCluster;
	this->linkage[depth * 3  + 2] = distance;
}

void GenericLinkageStatus::InsertNewCluster(int depth, int firstCluster, int secondCluster, float distance)
{
	this->SetLinkage(depth, firstCluster, secondCluster, distance);
	this->CombineSizes(firstCluster, secondCluster, this->numberOfRows + depth);

	this->newClusterUpdate(
		this->data,
		this->sizes,
		this->numberOfColumns,
		firstCluster,
		secondCluster,
		this->numberOfRows + depth);

	this->clusterLabels.erase(firstCluster);
	this->clusterLabels.erase(secondCluster);
	this->clusterLabels.insert(this->numberOfRows + depth);
	this->queue.ReplaceElement(secondCluster, newCluster);
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
	float* distance)
{
	(*queue).GetMinimum(firstCluster, distance);
	*secondCluster = nearestNeightbours[*firstCluster];

	while (clusterIndices.count(*secondCluster) == 0)
	{
		
		UpdateNearestNeighbourOfMinimumPoint(clusterIndices, *queue, numberOfColumns, data, nearestNeightbours, *firstCluster);

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
	int clusterIndex)
{
	float minimumDistance = std::numeric_limits<float>::max();
	std::unordered_set<int>::iterator setIterator;

	for (setIterator = clusterIndices.begin(); setIterator != clusterIndices.end(); setIterator++)
	{
		if (*setIterator <= clusterIndex)
		{
			continue;
		}

		float current_distance = SquaredEuclidean(data, clusterIndex, *setIterator, numberOfColumns);

		if (current_distance < minimumDistance)
		{
			minimumDistance = current_distance;
			nearestNeightbours[clusterIndex] = *setIterator;
		}
	}

	queue.UpdateMinimum(minimumDistance);
}