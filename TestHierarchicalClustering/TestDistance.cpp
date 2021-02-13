#include "pch.h"
#include "CppUnitTest.h"
#include "../HierarchicalClustering/Distance.h"
#include "../HierarchicalClustering/Distance.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TestHierarchicalClustering
{
	TEST_CLASS(TestDistance)
	{
	public:

		#pragma region SquaredEuclidean

		TEST_METHOD(TestDistance_SquaredEuclidean_ExampleData)
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
			float result = SquaredEuclidean<float>(data, 0, 3, 4);

			// Assert
			float difference = std::abs(0.1314f - result);
			Assert::IsTrue(difference < this->tolerance);

			delete[] data;
		}

		#pragma endregion

		#pragma region CentroidUpdate

		TEST_METHOD(TestDistance_CentroidUpdate)
		{
			//Arrange
			float* data = new float[20]
			{
				0.25f, 0.74f, 0.36f, 0.45f,
				0.12f, 0.94f, 0.38f, 0.39f,
				0.93f, 0.54f, 0.69f, 0.49f,
				0.39f, 0.96f, 0.39f, 0.20f,
				0.00f, 0.00f, 0.00f, 0.00f,
			};

			int* sizes = new int[5]{ 1, 1, 1, 1, 2 };

			//Act
			CentroidUpdate<float>(data, sizes, 4, 0, 1, 4);

			//Assert - new centre should be the average of point 0, and 1 and stored in row 4
			float* expectedNewCentre = new float[4]{0.185f, 0.84f, 0.37f, 0.42f };

			wchar_t buffer[100];
			for (int i = 0; i < 4; i++)
			{
				swprintf(buffer, 100, L"There was a problem with element %d", i);
				Assert::IsTrue(std::abs(data[i + 16] - expectedNewCentre[i]) < this->tolerance, buffer);
			}

			delete[] data;
		}

		#pragma endregion

	private:

		float tolerance = 1e-7f;
	};
}
