#include "pch.h"
#include "CppUnitTest.h"
#include "../HierarchicalClustering/GenericLinkage.h"
#include "../HierarchicalClustering/GenericLinkage.cpp"
#include "../HierarchicalClustering/Read.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TestHierarchicalClustering
{
	TEST_CLASS(TestGenericLinkage)
	{
	public:

		#pragma region Status

		TEST_METHOD(TestGenericLinkage_GenericLinkageStatus_Construction)
		{
			int numberOfRows = 4;
			int numberOfColumns = 4;

			float* data = new float[16]
			{
				0.25f, 0.74f, 0.36f, 0.45f,
				0.12f, 0.94f, 0.38f, 0.39f,
				0.93f, 0.54f, 0.69f, 0.49f,
				0.39f, 0.96f, 0.39f, 0.20f,
			};

			float* linkage = new float[9];

			GenericLinkageStatus status = GenericLinkageStatus(numberOfRows, numberOfColumns, data, linkage);

			// Labels
			std::unordered_set<int> labels = *status.GetClusterLabels();
			Assert::AreEqual(4, (int)labels.size());
			for (int i = 0; i < 4; i++)
			{
				Assert::AreEqual(1, (int)labels.count(i));
			}

			// Data
			float* actualData = status.GetData();
			float actualTotal = 0;
			float expectedTotal = 0;
			for (int i = 0; i < 16; i++)
			{
				actualTotal += actualData[i];
				expectedTotal += data[i];
			}
			Assert::AreEqual(expectedTotal, actualTotal);

			// Minimum distances - compare with the minimum entries in the result from scipy.spatial.distance.pdist
			int* expectedNearestNeighbours = new int[3]   { 1             ,             3,              3 };
			float* expectedMinimumDistances = new float[3]{ 0.24677925358f,0.33090784215f, 0.80131142511f };

			int* actualNearestNeighbours = status.GetNearestNeighbours();
			float* actualMinimumDistances = status.GetMinimumDistances();

			for (int i = 0; i < 3; i++)
			{
				Assert::AreEqual(expectedNearestNeighbours[i], actualNearestNeighbours[i]);
				Assert::IsTrue(std::abs(expectedMinimumDistances[i] - actualMinimumDistances[i]) < this->tolerance);
			}

			// Queue
			PriorityQueue *queue = status.GetQueue();

			int index;
			float distance;
			(*queue).GetMinimum(&index, &distance);
			Assert::AreEqual(0, index);
			Assert::IsTrue(std::abs(0.24677925358f - distance) < this->tolerance);

			// Sizes
			for (int i = 0; i < 4; i++)
			{
				Assert::AreEqual(1, status.GetSize(i));
			}

			delete[] expectedMinimumDistances;
			delete[] expectedNearestNeighbours;
			delete[] linkage;
			delete[] data;
		}

		TEST_METHOD(TestGenericLinkage_GenericLinkageStatus_CombineSizes)
		{
			//Arrange
			int numberOfRows = 4;
			int numberOfColumns = 4;

			float* data = new float[16]
			{
				0.25f, 0.74f, 0.36f, 0.45f,
				0.12f, 0.94f, 0.38f, 0.39f,
				0.93f, 0.54f, 0.69f, 0.49f,
				0.39f, 0.96f, 0.39f, 0.20f,
			};

			float* linkage = new float[9];

			GenericLinkageStatus status = GenericLinkageStatus(numberOfRows, numberOfColumns, data, linkage);

			//Act
			status.CombineSizes(0, 1, 4);

			//Assert
			Assert::AreEqual(2, status.GetSize(4));

			delete[] data;
			delete[] linkage;
		}

		TEST_METHOD(TestGenericLinkage_GenericLinkageStatus_SetLinkage_Example)
		{
			//Arrange
			int numberOfRows = 4;
			int numberOfColumns = 4;

			float* data = new float[16]
			{
				0.25f, 0.74f, 0.36f, 0.45f,
				0.12f, 0.94f, 0.38f, 0.39f,
				0.93f, 0.54f, 0.69f, 0.49f,
				0.39f, 0.96f, 0.39f, 0.20f,
			};

			float* linkage = new float[9];

			GenericLinkageStatus status = GenericLinkageStatus(numberOfRows, numberOfColumns, data, linkage);

			//Act
			status.SetLinkage(0, 0, 1, 0.0609f);
			status.SetLinkage(1, 2, 3, 0.6421f);

			//Assert

			Assert::AreEqual(0.0f   , linkage[0]);
			Assert::AreEqual(1.0f   , linkage[1]);
			Assert::AreEqual(0.0609f, linkage[2]);


			Assert::AreEqual(2.0f, linkage[3]);
			Assert::AreEqual(3.0f, linkage[4]);
			Assert::AreEqual(0.6421f, linkage[5]);

			delete[] data;
			delete[] linkage;
		}

		TEST_METHOD(TestGenericLinakge_GenericLinakgeStatus_InsertNewCluster)
		{
			//Arrange
			int numberOfRows = 4;
			int numberOfColumns = 4;

			float* data = new float[20]
			{
				0.25f, 0.74f, 0.36f, 0.45f,
				0.12f, 0.94f, 0.38f, 0.39f,
				0.93f, 0.54f, 0.69f, 0.49f,
				0.39f, 0.96f, 0.39f, 0.20f,
				0.00f, 0.00f, 0.00f, 0.00f, // for the new centre
			};

			float* linkage = new float[12];

			GenericLinkageStatus status = GenericLinkageStatus(numberOfRows, numberOfColumns, data, linkage);

			//Act
			status.InsertNewCluster(0, 0, 1, 0.0609f);

			//Assert - linkage
			Assert::AreEqual(0.0f, linkage[0]);
			Assert::AreEqual(1.0f, linkage[1]);
			Assert::AreEqual(0.0609f, linkage[2]);

			//Assert - sizes
			Assert::AreEqual(2, status.GetSize(4));

			//Assert - new centre in this case average of point 0 and 1
			float* currentData = status.GetData();
			Assert::IsTrue(std::abs(0.185f - currentData[16]) < this->tolerance); 
			Assert::IsTrue(std::abs(0.84f - currentData[17]) < this->tolerance);
			Assert::IsTrue(std::abs(0.37f - currentData[18]) < this->tolerance);
			Assert::IsTrue(std::abs(0.42f - currentData[19]) < this->tolerance);

			//Assert - labels
			std::unordered_set<int> labels = *status.GetClusterLabels();
			Assert::AreEqual(1, (int)labels.count(4)); // new label should be created
			Assert::AreEqual(0, (int)labels.count(0)); // old label should be deleted
			Assert::AreEqual(0, (int)labels.count(1)); // old label should be deleted

			delete[] data;
			delete[] linkage;
		}

		#pragma endregion

		#pragma region GetNextClustersToMerge
		TEST_METHOD(TestGenericLinkage_GetNextClustersToMerge_NearestNeighbourStillExists)
		{
			// Arrange

			float* data = new float[16]
			{
				0.25f, 0.74f, 0.36f, 0.45f,
				0.12f, 0.94f, 0.38f, 0.39f,
				0.93f, 0.54f, 0.69f, 0.49f,
				0.39f, 0.96f, 0.39f, 0.20f,
			};

			float* linkage = new float[9];

			auto status = GenericLinkageStatus(4, 4, data, linkage);

			int firstCluster;
			int secondCluster;
			float distance;

			// Act
			status.GetNextClustersToMerge(&firstCluster, &secondCluster, &distance);

			// Assert
			Assert::AreEqual(0, firstCluster);
			Assert::AreEqual(1, secondCluster);
			Assert::IsTrue(std::abs(distance - 0.24677925358f) < this->tolerance);
			
			delete[] data;
			delete[] linkage;
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

			float* linkage = new float[12];

			auto status = GenericLinkageStatus(4, 4, data, linkage);
			status.InsertNewCluster(0, 1, 3, 0.1f);

			int firstCluster;
			int secondCluster;
			float distance;

			// Act
			status.GetNextClustersToMerge(&firstCluster, &secondCluster, &distance);

			// Assert - should realise that 1 is no longer in the labels and that 2 is the next closest point
			Assert::AreEqual(0, firstCluster);
			Assert::AreEqual(2, secondCluster);
			Assert::IsTrue(std::abs(distance - 0.24677925358f) < this->tolerance);

			delete[] data;
			delete[] linkage;
		}

		TEST_METHOD(TestGenericLinkage_GetNextClustersToMerge_NearestNeighbourMissing_NewNearestNeighbour)
		{
			// Arrange
			float* data = new float[24]
			{
				0.25f, 0.74f, 0.36f, 0.45f,
				1.25f, 1.74f, 1.36f, 1.45f,
				0.12f, 0.94f, 0.38f, 0.39f,
				1.12f, 1.94f, 1.38f, 1.39f,
				0.93f, 0.54f, 0.69f, 0.49f,
				0.39f, 0.96f, 0.39f, 0.20f,
			};

			float* linkage = new float[15];

			auto status = GenericLinkageStatus(4, 4, data, linkage);
			status.InsertNewCluster(0, 1, 3, 12.5);

			int firstCluster;
			int secondCluster;
			float distance;

			// Act
			status.GetNextClustersToMerge(&firstCluster, &secondCluster, &distance);

			// Assert - should realise that 1 and 3 is no longer in the labels and that 0 compared with 2 is the next closest point
			Assert::AreEqual(0, firstCluster);
			Assert::AreEqual(2, secondCluster);
			Assert::IsTrue(std::abs(distance - 0.24677925358f) < this->tolerance);

			delete[] data;
			delete[] linkage;
		}

		#pragma endregion

		#pragma region UpdateNearestNeighbours

		TEST_METHOD(TestGenericLinkage_UpdateNearestNeighbours_WorkedExample)
		{
			// Arrange
			float* data = new float[16]
			{
				0.25f, 0.74f, 0.36f, 0.45f,
				0.12f, 0.94f, 0.38f, 0.39f,
				0.93f, 0.54f, 0.69f, 0.49f,
				0.39f, 0.96f, 0.39f, 0.20f,
			};

			float* linkage = new float[9];

			GenericLinkageStatus status = GenericLinkageStatus(4, 4, data, linkage);

			//Act I
			status.UpdateNearestNeighbourOfMinimumPoint(0);

			//Assert I
			int index;
			float minimumDistance;
			PriorityQueue* queue = status.GetQueue();
			(*queue).GetMinimum(&index, &minimumDistance);

			Assert::AreEqual(0, index);

			// point 1 should be the point closest to point 0 which has a euclidean distance of 0.24677925358
			Assert::IsTrue(std::abs(minimumDistance - 0.24677925358f) < this->tolerance);
			Assert::AreEqual(1, status.GetNearestNeighbours()[0]);

			////Act II

			// remove point 0 from the queue so point 1 is now first in the queue.
			queue = status.GetQueue();
			(*queue).RemoveMinimum();

			(*status.GetClusterLabels()).erase(0);

			status.UpdateNearestNeighbourOfMinimumPoint(1);

			//Assert II
			queue = status.GetQueue();
			(*queue).GetMinimum(&index, &minimumDistance);

			Assert::AreEqual(1, index);

			// point 3 should be the point closest to point 1 which has a squared euclidean distance of 0.33090784215f
			Assert::IsTrue(std::abs(minimumDistance - 0.33090784215f) < this->tolerance);
			Assert::AreEqual(3, status.GetNearestNeighbours()[1]);

			//Act III

			// remove point 1 from queue so point 2 is now first in queue
			queue = status.GetQueue();
			(*queue).RemoveMinimum();
			(*status.GetClusterLabels()).erase(1);
			status.UpdateNearestNeighbourOfMinimumPoint(2);

			//Assert 
			queue = status.GetQueue();
			(*queue).GetMinimum(&index, &minimumDistance);
			
			Assert::AreEqual(2, index);

			// point 2 should be closest to point 3 (only two points remaining) with euclidean distance 0.8013114251

			Assert::IsTrue(std::abs(minimumDistance - 0.8013114251) < this->tolerance);
			Assert::AreEqual(3, status.GetNearestNeighbours()[2]);

			delete[] data;
			delete[] linkage;
		}

		#pragma endregion

		#pragma region GenericLinkage
		TEST_METHOD(TestGenericLinkage_GenericLinkage_Example_Rows10)
		{
			runGenericLinkageOnTestFile(SOLUTION_DIRECTORY"TestFiles\\data_10.csv", SOLUTION_DIRECTORY"TestFiles//linkage_10.csv");
		}

		TEST_METHOD(TestGenericLinkage_GenericLinkage_Example_Rows1000)
		{
			runGenericLinkageOnTestFile(SOLUTION_DIRECTORY"TestFiles\\data_1000.csv", SOLUTION_DIRECTORY"TestFiles//linkage_1000.csv");
		}

		#pragma endregion

	private:

		float tolerance = 1e-6f;
		std::function<float(float*, int, int, int)> defaultMetric = SquaredEuclidean;

		void runGenericLinkageOnTestFile(char* inputFilename, char* linkageFilename)
		{
			int m, n;
			
			Assert::IsTrue(TryGetArraySize(inputFilename, &m, &n), L"Could not determine the array size from the input file");

			int vecSize = m * (2 * n + 1);
			float* vec = new float[vecSize];
			Assert::IsTrue(TryGetArrayFromFile(inputFilename, m, n, vec), L"Could not read in the array from the file");

			int linkageSize = (m - 1) * 3;
			float* actualLinkage = new float[linkageSize];

			//Act
			GenericLinkage(vec, actualLinkage, m, n);

			//Assert
			int linkageNumberOfRows;
			int linkageNumberOfColumns;
			Assert::IsTrue(TryGetArraySize(linkageFilename, &linkageNumberOfRows, &linkageNumberOfColumns), L"Could not determine the array size for the linkage");
			Assert::AreEqual(m - 1, linkageNumberOfRows);
			Assert::AreEqual(4, linkageNumberOfColumns);

			int expectedLinkageSize = linkageNumberOfRows * linkageNumberOfColumns;
			float* expectedLinkage = new float[expectedLinkageSize];
			Assert::IsTrue(TryGetArrayFromFile(linkageFilename, linkageNumberOfRows, linkageNumberOfColumns, expectedLinkage));

			wchar_t buffer[100];
			for (int i = 0; i < linkageNumberOfRows; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					int expectedIndex = i * linkageNumberOfColumns + j;
					int actualIndex = i * 3 + j;
					
					float expected = expectedLinkage[expectedIndex];
					float actual = actualLinkage[actualIndex];
					swprintf(buffer, 100, L"Linkage element %d,%d did not match: expected %f but got %f", i, j, expected, actual);
					Assert::IsTrue(std::abs(expectedLinkage[expectedIndex] - actualLinkage[actualIndex]) < this->tolerance, buffer);
				}
			}

			delete[] vec;
			delete[] actualLinkage;
			delete[] expectedLinkage;
		}
	};
}
