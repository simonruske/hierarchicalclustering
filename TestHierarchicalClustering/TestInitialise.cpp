#include "pch.h"
#include "CppUnitTest.h"
#include "../HierarchicalClustering/Initialise.h"
#include "../HierarchicalClustering/Initialise.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TestHierarchicalClustering
{
	TEST_CLASS(TestInitialise)
	{
	public:
		
		#pragma region InitialiseNearestNeighbours

		TEST_METHOD(TestInitialise_InitialiseNearestNeighbours_ExampleProblem)
		{
			// Arrange
			float* data = new float[16]
			{
				0.25f, 0.74f, 0.36f, 0.45f,
				0.12f, 0.94f, 0.38f, 0.39f,
				0.93f, 0.54f, 0.69f, 0.49f,
				0.39f, 0.96f, 0.39f, 0.20f,
			};

			// Act
			int numberOfRows = 4;
			int numberOfColumns = 4;

			int* nearestNeighbours = new int[3];
			float* minimumDistances = new float[3];

			InitialiseNearestNeighbours(numberOfRows, numberOfColumns, nearestNeighbours, minimumDistances, data);

			// Assert - compare with the minimum entries in the result from scipy.spatial.distance.pdist
			int* expectedNearestNeighbours  = new int[3]   {       1,      3,      3 };
			float* expectedMinimumDistances = new float[3] { 0.0609f, 0.1095, 0.6421 };

			for (int i = 0; i < 3; i++)
			{
				Assert::AreEqual(expectedNearestNeighbours[i], nearestNeighbours[i]);
				Assert::IsTrue(std::abs(expectedMinimumDistances[i] - minimumDistances[i]) < this->tolerance);
			}
		}




		#pragma endregion

	private:
		float tolerance = 1e-7f;
	};
}