#pragma once
#include <unordered_set>
#include "Queue.h"

class GenericLinkageStatus
{
public:
	GenericLinkageStatus(int numberOfRows, int numberOfColumns, float* data);

	void CombineSizes(int firstLocation, int secondLocation, int newLocation);
	void SetLinkage(int depth, int firstCluster, int secondCluster, float distance);

	std::unordered_set<int> GetClusterLabels();
	PriorityQueue GetQueue();
	float* GetMinimumDistances();
	int* GetNearestNeighbours();
	float* GetData();
	float* GetLinkage();
	int GetSize(int clusterIndex);

	void PrintLinkage();

private:
	std::unordered_set<int> clusterLabels;
	PriorityQueue queue;
	float* minimumDistances;
	int* nearestNeighbours;
	float* data;
	int* sizes;
	float* linkage;
	int numberOfRows;
	int numberOfColumns;
};

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