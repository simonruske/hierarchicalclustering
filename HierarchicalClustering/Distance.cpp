#include "Distance.h"

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

/// <summary>
/// The update should only be called after the sizes have been updated
/// </summary>
/// <param name="data"></param>
/// <param name="sizes"></param>
/// <param name="numberOfColumns"></param>
/// <param name="firstIndex"></param>
/// <param name="secondIndex"></param>
/// <param name="newIndex"></param>

void CentroidUpdate(float* data, int* sizes, int numberOfColumns, int firstIndex, int secondIndex, int newIndex)
{
	for (int i = 0; i < numberOfColumns; i++)
	{
		data[newIndex * numberOfColumns + i] = (
			data[firstIndex * numberOfColumns + i] * sizes[firstIndex] +
			data[secondIndex * numberOfColumns + i] * sizes[secondIndex]) /
			sizes[newIndex];
	}
}