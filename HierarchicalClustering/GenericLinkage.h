#pragma once
#include <unordered_set>
#include <functional>
#include "Queue.h"

template <typename T>
class GenericLinkageStatus
{
public:
	GenericLinkageStatus(
		int numberOfRows, 
		int numberOfColumns, 
		T* data,
		T* linkage);
	~GenericLinkageStatus();

	void CombineSizes(int firstLocation, int secondLocation, int newLocation);
	void SetLinkage(int depth, int firstCluster, int secondCluster, T distance);
	void InsertNewCluster(int depth, int firstCluster, int secondCluster, T distance);
	void UpdateNearestNeighbours(int newCluster);

	void GetNextClustersToMerge(int* firstCluster, int* secondCluster, T* distance);
	void UpdateNearestNeighbourOfMinimumPoint(int clusterIndex);

	std::unordered_set<int>* GetClusterLabels();
	PriorityQueue<T>* GetQueue();
	T* GetMinimumDistances();
	int* GetNearestNeighbours();
	T* GetData();
	T* GetLinkage();
	int GetSize(int clusterIndex);
	void PrintLinkage();

private:
	std::unordered_set<int> clusterLabels;
	PriorityQueue<T> queue;
	T* minimumDistances;
	int* nearestNeighbours;
	T* data;
	int* sizes;
	T* linkage;
	int numberOfRows;
	int numberOfColumns;

	std::function<void(T*, int*, int, int, int, int)> newClusterUpdate;
};

template <typename T> void GenericLinkage(T* data, T* linkage, int numberOfRows, int numberOfColumns);