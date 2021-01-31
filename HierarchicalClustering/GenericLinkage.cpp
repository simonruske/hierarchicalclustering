#include "GenericLinkage.h"
#include <unordered_set>
#include "Queue.h"

void GetNextClustersToMerge(
	std::unordered_set<int> clusterIndices,
	PriorityQueue queue,
	int numberOfColumns,
	float* data,
	int* nearestNeightbours,
	int* firstCluster,
	int* secondCluster,
	float* distance)
{
	queue.GetMinimum(firstCluster, distance);
	*secondCluster = nearestNeightbours[*firstCluster];

	while (clusterIndices.count(*secondCluster) == 0)
	{
		
		UpdateNearestNeighbourOfMinimumPoint(clusterIndices, queue, numberOfColumns, data, nearestNeightbours, *firstCluster);

		queue.GetMinimum(firstCluster, distance);
		*secondCluster = nearestNeightbours[*firstCluster];
	}

	queue.RemoveMinimum();
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

float SquaredEuclidean(float* data, int firstIndex, int secondIndex, int numberOfColumns)
{
	float distance = 0;
	for (int i = 0; i < numberOfColumns; i++)
	{
		float difference = data[firstIndex * numberOfColumns + i] - data[secondIndex * numberOfColumns + i];
		distance += difference * difference;
	}
	return distance;
}