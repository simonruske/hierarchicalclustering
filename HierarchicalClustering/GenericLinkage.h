#pragma once
#include <unordered_set>
#include <functional>
#include "Queue.h"

class GenericLinkageStatus
{
public:
	GenericLinkageStatus(
		int numberOfRows, 
		int numberOfColumns, 
		float* data);

	void CombineSizes(int firstLocation, int secondLocation, int newLocation);
	void SetLinkage(int depth, int firstCluster, int secondCluster, float distance);
	void InsertNewCluster(int depth, int firstCluster, int secondCluster, float distance);
	void UpdateNearestNeighbours(int newCluster);

	void GetNextClustersToMerge(int* firstCluster, int* secondCluster, float* distance);
	void UpdateNearestNeighbourOfMinimumPoint(int clusterIndex);

	std::unordered_set<int>* GetClusterLabels();
	PriorityQueue* GetQueue();
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

	std::function<void(float*, int*, int, int, int, int)> newClusterUpdate;
};

GenericLinkageStatus GenericLinkage(float* data, int numberOfRows, int numberOfColumns);