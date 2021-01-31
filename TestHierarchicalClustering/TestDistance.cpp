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

		TEST_METHOD(TestGenericLinkage_SquaredEuclidean_ExampleData)
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
			float result = SquaredEuclidean(data, 0, 3, 4);

			// Assert
			float difference = std::abs(0.1314f - result);
			Assert::IsTrue(difference < this->tolerance);
		}

		#pragma endregion

	private:

		float tolerance = 1e-7f;
	};
}
