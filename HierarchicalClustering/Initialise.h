#pragma once
#include <unordered_set>
#include <functional>

void InitialiseClusterLabels(std::unordered_set<int>* clusterLabels, int numberOfLabels);

void InitialiseSizes(int* sizes, int numberOfRows);

void InitialiseNearestNeighbours(
	int numberOfRows,
	int numberOfColumns,
	int* nearestNeighbours,
	float* minimumDistances,
	float* data);
