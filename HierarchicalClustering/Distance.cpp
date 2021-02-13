#include "Distance.h"

template <typename T>
T SquaredEuclidean(T* data, int firstIndex, int secondIndex, int numberOfColumns)
{
	T distance = 0;
	for (int i = 0; i < numberOfColumns; i++)
	{
		T difference = data[firstIndex * numberOfColumns + i] - data[secondIndex * numberOfColumns + i];
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

template<typename T>
void CentroidUpdate(T* data, int* sizes, int numberOfColumns, int firstIndex, int secondIndex, int newIndex)
{
	for (int i = 0; i < numberOfColumns; i++)
	{
		data[newIndex * numberOfColumns + i] = (
			data[firstIndex * numberOfColumns + i] * sizes[firstIndex] +
			data[secondIndex * numberOfColumns + i] * sizes[secondIndex]) /
			sizes[newIndex];
	}
}

template double SquaredEuclidean<double>(double* data, int firstIndex, int secondIndex, int numberOfColumns);
template float SquaredEuclidean<float>(float* data, int firstIndex, int secondIndex, int numberOfColumns);
template void CentroidUpdate<double>(double* data, int* sizes, int numberOfColumns, int firstIndex, int secondIndex, int newIndex);
template void CentroidUpdate<float>(float* data, int* sizes, int numberOfColumns, int firstIndex, int secondIndex, int newIndex);