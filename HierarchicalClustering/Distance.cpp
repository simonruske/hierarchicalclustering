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