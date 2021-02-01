#include "Initialise.h"
#include "Distance.h"
#include <limits>

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
	for (int i = 0; i < numberOfRows - 1; i++)
	{
		minimumDistances[i] = std::numeric_limits<float>().max();

		for (int j = i + 1; j < numberOfRows; j++)
		{
			float dist = SquaredEuclidean(data, i, j, numberOfColumns);

			if (dist < minimumDistances[i])
			{
				minimumDistances[i] = dist;
				nearestNeighbours[i] = j;
			}
		}
	}
}