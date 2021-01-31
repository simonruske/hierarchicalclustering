#pragma once
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
	float* distance);

void UpdateNearestNeighbourOfMinimumPoint(
	std::unordered_set<int> clusterIndices,
	PriorityQueue queue,
	int numberOfColumns,
	float* data,
	int* nearestNeightbours,
	int clusterIndex);

float SquaredEuclidean(float* data, int firstIndex, int secondIndex, int numberOfColumns);