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

		#pragma region Constructor

		TEST_METHOD(TestQueue_Constructor_SimplExample_HasCorrectProperties)
		{
			int size = 2;
			float* minimumDistances = new float[2]{ 0.45f, 0.35f };
			PriorityQueue queue = PriorityQueue(minimumDistances, size);

			int* indices = queue.GetIndices();
			Assert::AreEqual(1, indices[0]);
			Assert::AreEqual(0, indices[1]);

			float* distances = queue.GetMinimumDistances();
			Assert::AreEqual(0.45f, distances[0]);
			Assert::AreEqual(0.35f, distances[1]);

			Assert::AreEqual(2, queue.GetCurrentSize());
			Assert::AreEqual(2, queue.GetCapacity());
		}

		TEST_METHOD(TestQueue_Constructor_TwoNodesInOrder_CreatesMinHeap)
		{
			// Act
			int size = 2;
			float* minimumDistances = new float[2]{ 0.35f, 0.45f };
			PriorityQueue queue = PriorityQueue(minimumDistances, size);
			Assert::IsTrue(this->IsMinimumHeap(queue));
		}

		TEST_METHOD(TestQueue_Constructor_TwoNodesOutOfOrder_CreatesMinHeap)
		{
			// Act
			int size = 2;
			float* minimumDistances = new float[2]{ 0.45f, 0.35f };
			PriorityQueue queue = PriorityQueue(minimumDistances, size);
			Assert::IsTrue(this->IsMinimumHeap(queue));
		}

		TEST_METHOD(TestQueue_Constructor_ThreeNodesAlreadyCorrect_CreatesMinHeap)
		{
			// Act
			int size = 3;
			float* minimumDistances = new float[3]{ 0.35f, 0.45f, 0.55f };
			PriorityQueue queue = PriorityQueue(minimumDistances, size);
			Assert::IsTrue(this->IsMinimumHeap(queue));
		}

		TEST_METHOD(TestQueue_Constructor_ThreeNodes_ChildrenOutOfOrder_CreatesMinHeap)
		{
			// Act
			int size = 3;
			float* minimumDistances = new float[3]{ 0.35f, 0.55f, 0.45f };
			PriorityQueue queue = PriorityQueue(minimumDistances, size);
			Assert::IsTrue(this->IsMinimumHeap(queue));
		}

		TEST_METHOD(TestQueue_Constructor_RandomDistances_CreatesMinHeap)
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

		TEST_METHOD(TestQueue_Constructor_MaxHeapInput_CreatesMinHeapInQueue)
		{
			//Arrange
			int size = 9;
			float* minimumDistances = new float[size] { 100, 19, 36, 17, 3, 25, 1, 2, 7 };
			PriorityQueue queue = PriorityQueue(minimumDistances, size);

			//Queue should original have minimum 1 from position 6 in the distance array
			float originalMinimum;
			int originalMinimumIndex;
			queue.GetMinimum(&originalMinimumIndex, &originalMinimum);

			Assert::AreEqual(1.0f, originalMinimum);
			Assert::AreEqual(6, originalMinimumIndex);
			Assert::IsTrue(this->IsMinimumHeap(queue));
		}

		#pragma endregion

		#pragma region UpdateMinimum

		TEST_METHOD(TestQueue_UpdateMinimum_UpdatedToLessThanChildren_KeepsSameMinimum)
		{
			//Arrange
			int size = 9;
			float* minimumDistances = new float[size]{ 100, 19, 36, 17, 3, 25, 1, 2, 7 };
			PriorityQueue queue = PriorityQueue(minimumDistances, size);

			queue.UpdateMinimum(1.5f);

			float currentMinimum;
			int currentMinimumIndex;
			queue.GetMinimum(&currentMinimumIndex, &currentMinimum);

			Assert::AreEqual(1.5f, currentMinimum);
			Assert::AreEqual(6, currentMinimumIndex);
			Assert::IsTrue(this->IsMinimumHeap(queue));
		}

		TEST_METHOD(TestQueue_UpdateMinimum_UpdatedToGreaterThanSecondMinimum_GetsUpdatedMinimum)
		{
			//Arrange
			int size = 9;
			float* minimumDistances = new float[size] { 100, 19, 36, 17, 3, 25, 1, 2, 7 };
			PriorityQueue queue = PriorityQueue(minimumDistances, size);

			queue.UpdateMinimum(2.5f);

			float currentMinimum;
			int currentMinimumIndex;
			queue.GetMinimum(&currentMinimumIndex, &currentMinimum);

			Assert::AreEqual(2.0f, currentMinimum);
			Assert::AreEqual(7, currentMinimumIndex);
			Assert::IsTrue(this->IsMinimumHeap(queue));
		}

		#pragma endregion

		#pragma region IsMinimumHeap

		TEST_METHOD(IsMinimumHeapSanityCheck_MaximumHeap_ReturnsFalse)
		{
			int size = 9;
			int* indices = new int[9] {0, 1, 2, 3, 4, 5, 6, 7, 8};
			float* minimumDistances = new float[9] {100, 19, 36, 17, 3, 25, 1, 2, 7};

			Assert::IsFalse(IsMinimumHeap(size, indices, minimumDistances));
		}

		TEST_METHOD(IsMinimumHeapSanityCheck_MinimumHeap_ReturnsTrue)
		{
			int size = 9;
			int* indices = new int[9]{ 0, 1, 2, 3, 4, 5, 6, 7, 8 };
			float* minimumDistances = new float[9]{ 1, 2, 3, 17, 19, 36, 7, 25, 100 };

			Assert::IsTrue(IsMinimumHeap(size, indices, minimumDistances));
		}

		TEST_METHOD(IsMinimumHeapSanityCheck_ModificationOnMinHeap_ReturnsFalse)
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

		TEST_METHOD(IsMinimumHeapSanityCheck_EverythingEqual_ReturnsTrue)
		{
			int size = 9;
			int* indices = new int[9]{ 0, 1, 2, 3, 4, 5, 6, 7, 8 };
			float* minimumDistances = new float[9]{ 1, 1, 1, 1, 1, 1, 1, 1, 1 };
			Assert::IsTrue(IsMinimumHeap(size, indices, minimumDistances));
		}

		#pragma endregion

	private: 
		static bool IsMinimumHeap(PriorityQueue queue)
		{
			int size = queue.GetCurrentSize();
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
