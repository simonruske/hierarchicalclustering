#include "pch.h"
#include "CppUnitTest.h"
#include "../HierarchicalClustering/GenericLinkage.h"
#include "../HierarchicalClustering/GenericLinkage.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TestHierarchicalClustering
{
	TEST_CLASS(TestGenericLinkage)
	{
	public:

		#pragma region GetNextClustersToMerge
		TEST_METHOD(TestGenericLinkage_GetNextClustersToMerge_NearestNeighbourStillExists)
		{
			// Arrange
			std::unordered_set<int> labels = { 0, 1, 2, 3 };

			int size = 3;
			float* minimumDistances = new float[size] {0.0609f, 0.1095f, 0.641f };
			int* nearestNeighbours = new int[size]    { 1, 3, 3 };
			auto queue = PriorityQueue(minimumDistances, size);

			float* data = new float[16]
			{
				0.25f, 0.74f, 0.36f, 0.45f,
				0.12f, 0.94f, 0.38f, 0.39f,
				0.93f, 0.54f, 0.69f, 0.49f,
				0.39f, 0.96f, 0.39f, 0.20f,
			};

			int firstCluster;
			int secondCluster;
			float distance;

			// Act
			GetNextClustersToMerge(labels, queue, 4, data, nearestNeighbours, &firstCluster, &secondCluster, &distance);

			// Assert
			Assert::AreEqual(0, firstCluster);
			Assert::AreEqual(1, secondCluster);
			Assert::IsTrue(std::abs(distance - 0.0609) < this->tolerance);
		}

		TEST_METHOD(TestGenericLinkage_GetNextClustersToMerge_NearestNeighbourMissing_PointStillMinimum)
		{
			// Arrange
			std::unordered_set<int> labels = { 0, 2, 3, 4 }; // 1 is no longer there

			int size = 4;
			float* minimumDistances = new float[size] {0.0f, 0.0609f, 0.1095f, 0.641f }; // 0 has nearest neighbour of 1 with distance 0
			int* nearestNeighbours = new int[size] { 1, 2, 3, 3 }; 
			auto queue = PriorityQueue(minimumDistances, size);

			float* data = new float[20]
			{
				0.25f, 0.74f, 0.36f, 0.45f,
				0.25f, 0.74f, 0.36f, 0.45f,
				0.12f, 0.94f, 0.38f, 0.39f,
				0.93f, 0.54f, 0.69f, 0.49f,
				0.39f, 0.96f, 0.39f, 0.20f,
			};

			int firstCluster;
			int secondCluster;
			float distance;

			// Act
			GetNextClustersToMerge(labels, queue, 4, data, nearestNeighbours, &firstCluster, &secondCluster, &distance);

			// Assert - should realise that 1 is no longer in the labels and that 2 is the next closest point
			Assert::AreEqual(0, firstCluster);
			Assert::AreEqual(2, secondCluster);
			Assert::IsTrue(std::abs(distance - 0.0609) < this->tolerance);
		}

		TEST_METHOD(TestGenericLinkage_GetNextClustersToMerge_NearestNeighbourMissing_AnotherPointNowMinimum)
		{
			// Arrange
			std::unordered_set<int> labels = { 0, 1, 3, 4 }; // 2 is no longer there

			int size = 4;
			float* minimumDistances = new float[size] {0.0609f, 0.0f, 0.1095f, 0.641f }; // 1 has nearest neighbour of 2 with distance 0
			int* nearestNeighbours = new int[size] { 1, 2, 3, 3 };
			auto queue = PriorityQueue(minimumDistances, size);

			float* data = new float[20]
			{
				0.25f, 0.74f, 0.36f, 0.45f,
				0.12f, 0.94f, 0.38f, 0.39f,
				0.12f, 0.94f, 0.38f, 0.39f,
				0.93f, 0.54f, 0.69f, 0.49f,
				0.39f, 0.96f, 0.39f, 0.20f,
			};

			int firstCluster;
			int secondCluster;
			float distance;

			// Act
			GetNextClustersToMerge(labels, queue, 4, data, nearestNeighbours, &firstCluster, &secondCluster, &distance);

			// Assert - should realise that 2 is no longer in the labels and that 0 compared with 1 is the next closest point
			Assert::AreEqual(0, firstCluster);
			Assert::AreEqual(1, secondCluster);
			Assert::IsTrue(std::abs(distance - 0.0609) < this->tolerance);
		}

		TEST_METHOD(TestGenericLinkage_GetNextClustersToMerge_NearestNeighbourMissing_MultipleMissing)
		{
			// Arrange
			std::unordered_set<int> labels = { 0, 2, 4, 5 }; // 1, 3 are no longer there

			int size = 5;
			float* minimumDistances = new float[size] {0.0f, 0.0609f, 0.0f, 0.1095f, 0.641f };
			int* nearestNeighbours = new int[size] { 1, 2, 3, 5, 5 };
			auto queue = PriorityQueue(minimumDistances, size);

			float* data = new float[24]
			{
				0.25f, 0.74f, 0.36f, 0.45f,
				0.25f, 0.74f, 0.36f, 0.45f,
				0.12f, 0.94f, 0.38f, 0.39f,
				0.12f, 0.94f, 0.38f, 0.39f,
				0.93f, 0.54f, 0.69f, 0.49f,
				0.39f, 0.96f, 0.39f, 0.20f,
			};

			int firstCluster;
			int secondCluster;
			float distance;

			// Act
			GetNextClustersToMerge(labels, queue, 4, data, nearestNeighbours, &firstCluster, &secondCluster, &distance);

			// Assert - should realise that 1 and 3 is no longer in the labels and that 0 compared with 2 is the next closest point
			Assert::AreEqual(0, firstCluster);
			Assert::AreEqual(2, secondCluster);
			Assert::IsTrue(std::abs(distance - 0.0609) < this->tolerance);
		}

		#pragma endregion

		#pragma region UpdateNearestNeighbours

		TEST_METHOD(TestGenericLinkage_UpdateNearestNeighbours_WorkedExample)
		{
			// Arrange
			std::unordered_set<int> labels = { 0, 1, 2, 3 };

			// Create queue with out of date minimum distances
			int size = 4;
			float* minimumDistances = new float[size] { 12, 13, 14, 15 };
			auto queue = PriorityQueue(minimumDistances, size);

			float* data = new float[16]
			{
				0.25f, 0.74f, 0.36f, 0.45f,
				0.12f, 0.94f, 0.38f, 0.39f,
				0.93f, 0.54f, 0.69f, 0.49f,
				0.39f, 0.96f, 0.39f, 0.20f,
			};

			int* nearestNeighbours = new int[3]{ 3, 2, 1 };

			//Act I
			UpdateNearestNeighbourOfMinimumPoint(labels, queue, 4, data, nearestNeighbours, 0);

			//Assert I
			int index;
			float minimumDistance;
			queue.GetMinimum(&index, &minimumDistance);

			Assert::AreEqual(0, index);

			// point 1 should be the point closest to point 0 which has a squared euclidean distance of 0.0609
			Assert::IsTrue(std::abs(minimumDistance - 0.0609) < this->tolerance);
			Assert::AreEqual(1, nearestNeighbours[0]);

			//Act II

			// remove point 0 from the queue so point 1 is now first in the queue.
			queue.RemoveMinimum();
			UpdateNearestNeighbourOfMinimumPoint(labels, queue, 4, data, nearestNeighbours, 1);

			//Assert II
			queue.GetMinimum(&index, &minimumDistance);

			Assert::AreEqual(1, index);

			// point 3 should be the point closest to point 1 which has a squared euclidean distance of 0.1095
			Assert::IsTrue(std::abs(minimumDistance - 0.1095) < this->tolerance);
			Assert::AreEqual(3, nearestNeighbours[1]);

			//Act III

			// remove point 1 from queue so point 2 is now first in queue
			queue.RemoveMinimum();
			UpdateNearestNeighbourOfMinimumPoint(labels, queue, 4, data, nearestNeighbours, 2);

			//Assert III
			queue.GetMinimum(&index, &minimumDistance);
			
			Assert::AreEqual(2, index);

			// point 2 should be closest to point 3 (only two points remaining) with squared distance 0.6421

			Assert::IsTrue(std::abs(minimumDistance - 0.6421) < this->tolerance);
			Assert::AreEqual(3, nearestNeighbours[2]);
		}

		#pragma endregion

	private:

		float tolerance = 1e-7f;
	};
}
