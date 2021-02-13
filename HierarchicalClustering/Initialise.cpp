#include "Initialise.h"
#include "Distance.h"
#include <limits>
#include <functional>

void InitialiseClusterLabels(std::unordered_set<int> *clusterLabels, int numberOfLabels)
{
	for (int i = 0; i < numberOfLabels; i++)
	{
		(*clusterLabels).insert(i);
	}
}

void InitialiseSizes(int* sizes, int numberOfRows)
{
	for (int i = 0; i <= numberOfRows; i++)
	{
		sizes[i] = 1;
	}
}

void InitialiseNearestNeighbours(
	int numberOfRows,
	int numberOfColumns,
	int* nearestNeighbours,
	float* minimumDistances,
	float* data)
{
	float minimumDistance;
	float currentDistance;
	int nearestNeighbour;

	for (int i = 0; i < numberOfRows * 2 - 1; i++)
	{
		minimumDistances[i] = std::numeric_limits<float>().max();
	}

	for (int i = 0; i < numberOfRows - 1; i++)
	{
		minimumDistance = std::numeric_limits<float>().max();
		nearestNeighbour = 0;
		for (int j = i + 1; j < numberOfRows; j++)
		{
			currentDistance = SquaredEuclidean(data, i, j, numberOfColumns);

			if (currentDistance < minimumDistance)
			{
				minimumDistance = currentDistance;
				nearestNeighbour = j;
			}
		}
		minimumDistances[i] = std::sqrtf(minimumDistance);
		nearestNeighbours[i] = nearestNeighbour;
	}
}