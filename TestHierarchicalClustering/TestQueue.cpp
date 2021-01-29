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
		TEST_METHOD(TestQueueConstructor_TwoNodesInOrder_CreatesMinHeap)
		{
			// Act
			int size = 2;
			float* minimumDistances = new float[2]{ 0.35f, 0.45f };
			PriorityQueue queue = PriorityQueue(minimumDistances, size);
			Assert::IsTrue(this->IsMinimumHeap(queue));
		}

		TEST_METHOD(TestQueueConstructor_TwoNodesOutOfOrder_CreatesMinHeap)
		{
			// Act
			int size = 2;
			float* minimumDistances = new float[2]{ 0.45f, 0.35f };
			PriorityQueue queue = PriorityQueue(minimumDistances, size);
			Assert::IsTrue(this->IsMinimumHeap(queue));
		}

		TEST_METHOD(TestQueueConstructor_ThreeNodesAlreadyCorrect_CreatesMinHeap)
		{
			// Act
			int size = 3;
			float* minimumDistances = new float[3]{ 0.35f, 0.45f, 0.55f };
			PriorityQueue queue = PriorityQueue(minimumDistances, size);
			Assert::IsTrue(this->IsMinimumHeap(queue));
		}

		TEST_METHOD(TestQueueConstructor_ThreeNodes_ChildrenOutOfOrder_CreatesMinHeap)
		{
			// Act
			int size = 3;
			float* minimumDistances = new float[3]{ 0.35f, 0.55f, 0.45f };
			PriorityQueue queue = PriorityQueue(minimumDistances, size);
			Assert::IsTrue(this->IsMinimumHeap(queue));
		}

		TEST_METHOD(TestQueueConstructor_RandomDistances_CreatesMinHeap)
		{
			std::srand(8455624);
			int numberOfRepititions = 1000;

			for (int repition = 0; repition < numberOfRepititions; repition++)
			{
				int size = rand() % 100;
				float* minimumDistances = new float[size];
				
				for (int i = 0; i < size; i++)
				{
					minimumDistances[i] = float(rand()) / float((RAND_MAX)) * 0.75f;
				}

				PriorityQueue queue = PriorityQueue(minimumDistances, size);

				Assert::IsTrue(this->IsMinimumHeap(queue));
			}
		}

		TEST_METHOD(IsMinHeapSanityCheck_MaximumHeap_ReturnsFalse)
		{
			int size = 9;
			int* indices = new int[9] {0, 1, 2, 3, 4, 5, 6, 7, 8};
			float* minimumDistances = new float[9] {100, 19, 36, 17, 3, 25, 1, 2, 7};

			Assert::IsFalse(IsMinimumHeap(size, indices, minimumDistances));
		}

		TEST_METHOD(IsMinHeapSanityCheck_MinimumHeap_ReturnsTrue)
		{
			int size = 9;
			int* indices = new int[9]{ 0, 1, 2, 3, 4, 5, 6, 7, 8 };
			float* minimumDistances = new float[9]{ 1, 2, 3, 17, 19, 36, 7, 25, 100 };

			Assert::IsTrue(IsMinimumHeap(size, indices, minimumDistances));
		}

		TEST_METHOD(IsMinHeapSanityCheck_ModificationOnMinHeap_ReturnsFalse)
		{
			int size = 9;
			int* indices = new int[9]{ 0, 1, 2, 3, 4, 5, 6, 7, 8 };
			float* minimumDistances = new float[9]{ 1, 2, 3, 17, 19, 36, 7, 25, 100 };

			minimumDistances[0] = 3;
			Assert::IsFalse(IsMinimumHeap(size, indices, minimumDistances));

			minimumDistances[0] = 1;
			minimumDistances[1] = 20;
			Assert::IsFalse(IsMinimumHeap(size, indices, minimumDistances));

			minimumDistances[1] = 2;
			minimumDistances[2] = 8;
			Assert::IsFalse(IsMinimumHeap(size, indices, minimumDistances));

			minimumDistances[2] = 3;
			minimumDistances[3] = 26;
			Assert::IsFalse(IsMinimumHeap(size, indices, minimumDistances));

			minimumDistances[3] = 17;
			minimumDistances[4] = 1;
			Assert::IsFalse(IsMinimumHeap(size, indices, minimumDistances));

			minimumDistances[4] = 19;
			minimumDistances[5] = 1;
			Assert::IsFalse(IsMinimumHeap(size, indices, minimumDistances));

			minimumDistances[5] = 36;
			minimumDistances[6] = 1;
			Assert::IsFalse(IsMinimumHeap(size, indices, minimumDistances));

			minimumDistances[6] = 7;
			minimumDistances[7] = 1;
			Assert::IsFalse(IsMinimumHeap(size, indices, minimumDistances));

			minimumDistances[7] = 25;
			minimumDistances[8] = 1;
			Assert::IsFalse(IsMinimumHeap(size, indices, minimumDistances));
		}

		TEST_METHOD(IsMinHeapSanityCheck_EverythingEqual_ReturnsTrue)
		{
			int size = 9;
			int* indices = new int[9]{ 0, 1, 2, 3, 4, 5, 6, 7, 8 };
			float* minimumDistances = new float[9]{ 1, 1, 1, 1, 1, 1, 1, 1, 1 };
			Assert::IsTrue(IsMinimumHeap(size, indices, minimumDistances));
		}

	private: 
		static bool IsMinimumHeap(PriorityQueue queue)
		{
			int size = queue.GetSize();
			int* indices = queue.GetIndices();
			float* minimumDistances = queue.GetMinimumDistances();
		
			return IsMinimumHeap(size, indices, minimumDistances);
		}

		static bool IsMinimumHeap(int size, int* indices, float* minimumDistances)
		{
			for (int i = 0; i < size / 2; i++)
			{
				int leftIndex = 2 * i + 1;
				bool hasLeftIndex = leftIndex < size;
				
				if (hasLeftIndex)
				{
					if (minimumDistances[indices[leftIndex]] < minimumDistances[indices[i]])
					{
						return false;
					}
				}

				int rightIndex = 2 * i + 2;
				bool hasRightIndex = rightIndex < size;

				if (hasRightIndex)
				{
					if (minimumDistances[indices[rightIndex]] < minimumDistances[indices[i]])
					{
						return false;
					}
				}
			}
			return true;
		}
	};
}
