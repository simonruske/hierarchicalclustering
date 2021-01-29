#include "pch.h"
#include "CppUnitTest.h"
#include "../HierarchicalClustering/Queue.h"
#include "../HierarchicalClustering/Queue.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TestHierarchicalClustering
{
	TEST_CLASS(TestNode)
	{
	public:

		TEST_METHOD(TestQueueConstructor_SimpleExample_CreatesMinHeap)
		{
			// Act
			int size = 2;
			float* minimumDistances = new float[2]{ 0.45, 0.35 };
			PriorityQueue queue = PriorityQueue(minimumDistances, size);

			// minimum distances should persist
			float* actualMinimumDistances = queue.GetMinimumDistances();
			Assert::AreEqual(0.45f, actualMinimumDistances[0]);
			Assert::AreEqual(0.35f, actualMinimumDistances[1]);

			// should switch 0 and 1 since 0.35 < 0.45
			int* actualIndices = queue.GetIndices();
			Assert::AreEqual(1, actualIndices[0]);
			Assert::AreEqual(0, actualIndices[1]);

			Assert::AreEqual(size, queue.GetSize());
		}
	};
}
