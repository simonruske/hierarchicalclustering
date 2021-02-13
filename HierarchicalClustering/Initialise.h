#pragma once
#include <unordered_set>
#include <functional>

void InitialiseClusterLabels(std::unordered_set<int>* clusterLabels, int numberOfLabels);

void InitialiseSizes(int* sizes, int numberOfRows);

template<typename T>
void InitialiseNearestNeighbours(
	int numberOfRows,
	int numberOfColumns,
	int* nearestNeighbours,
	T* minimumDistances,
	T* data);