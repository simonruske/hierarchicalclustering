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

			delete[] minimumDistances;
		}

		TEST_METHOD(TestQueue_Constructor_TwoNodesInOrder_CreatesMinHeap)
		{
			// Act
			int size = 2;
			float* minimumDistances = new float[2]{ 0.35f, 0.45f };
			PriorityQueue queue = PriorityQueue(minimumDistances, size);
			Assert::IsTrue(this->IsMinimumHeap(queue));
			delete[] minimumDistances;
		}

		TEST_METHOD(TestQueue_Constructor_TwoNodesOutOfOrder_CreatesMinHeap)
		{
			// Act
			int size = 2;
			float* minimumDistances = new float[2]{ 0.45f, 0.35f };
			PriorityQueue queue = PriorityQueue(minimumDistances, size);
			Assert::IsTrue(this->IsMinimumHeap(queue));
			delete[] minimumDistances;
		}

		TEST_METHOD(TestQueue_Constructor_ThreeNodesAlreadyCorrect_CreatesMinHeap)
		{
			// Act
			int size = 3;
			float* minimumDistances = new float[3]{ 0.35f, 0.45f, 0.55f };
			PriorityQueue queue = PriorityQueue(minimumDistances, size);
			Assert::IsTrue(this->IsMinimumHeap(queue));
			delete[] minimumDistances;
		}

		TEST_METHOD(TestQueue_Constructor_ThreeNodes_ChildrenOutOfOrder_CreatesMinHeap)
		{
			// Act
			int size = 3;
			float* minimumDistances = new float[3]{ 0.35f, 0.55f, 0.45f };
			PriorityQueue queue = PriorityQueue(minimumDistances, size);
			Assert::IsTrue(this->IsMinimumHeap(queue));
			delete[] minimumDistances;
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

				delete[] minimumDistances;
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
			delete[] minimumDistances;
		}

		TEST_METHOD(TestQueue_Constructor_Empty_CreatesEmptyHeap)
		{
			int size = 0;
			float* minimumDistances = new float[0]{};
			PriorityQueue queue = PriorityQueue(minimumDistances, size);

			Assert::AreEqual(0, queue.GetCapacity());
			Assert::AreEqual(0, queue.GetCurrentSize());
			delete[] minimumDistances;
		}

		#pragma endregion

		#pragma region RemoveMinimum

		TEST_METHOD(TestQueue_RemoveMinimum_RemoveSuccessiveElements_MinimumIsCorrectThroughout)
		{
			int size = 9;
			float* minimumDistances = new float[size] { 100, 19, 36, 17, 3, 25, 1, 2, 7 };
			PriorityQueue queue = PriorityQueue(minimumDistances, size);

			int index;
			float minimumDistance;

			queue.RemoveMinimum();
			queue.GetMinimum(&index, &minimumDistance);
			Assert::AreEqual(7, index);
			Assert::AreEqual(2.0f, minimumDistance);
			Assert::AreEqual(8, queue.GetCurrentSize());

			queue.RemoveMinimum();
			queue.GetMinimum(&index, &minimumDistance);
			Assert::AreEqual(4, index);
			Assert::AreEqual(3.0f, minimumDistance);
			Assert::AreEqual(7, queue.GetCurrentSize());

			queue.RemoveMinimum();
			queue.GetMinimum(&index, &minimumDistance);
			Assert::AreEqual(8, index);
			Assert::AreEqual(7.0f, minimumDistance);
			Assert::AreEqual(6, queue.GetCurrentSize());

			queue.RemoveMinimum();
			queue.GetMinimum(&index, &minimumDistance);
			Assert::AreEqual(3, index);
			Assert::AreEqual(17.0f, minimumDistance);
			Assert::AreEqual(5, queue.GetCurrentSize());

			queue.RemoveMinimum();
			queue.GetMinimum(&index, &minimumDistance);
			Assert::AreEqual(1, index);
			Assert::AreEqual(19.0f, minimumDistance);
			Assert::AreEqual(4, queue.GetCurrentSize());

			queue.RemoveMinimum();
			queue.GetMinimum(&index, &minimumDistance);
			Assert::AreEqual(5, index);
			Assert::AreEqual(25.0f, minimumDistance);
			Assert::AreEqual(3, queue.GetCurrentSize());

			queue.RemoveMinimum();
			queue.GetMinimum(&index, &minimumDistance);
			Assert::AreEqual(2, index);
			Assert::AreEqual(36.0f, minimumDistance);
			Assert::AreEqual(2, queue.GetCurrentSize());

			queue.RemoveMinimum();
			queue.GetMinimum(&index, &minimumDistance);
			Assert::AreEqual(0, index);
			Assert::AreEqual(100.0f, minimumDistance);
			Assert::AreEqual(1, queue.GetCurrentSize());

			queue.RemoveMinimum();
			Assert::AreEqual(0, queue.GetCurrentSize());
			delete[] minimumDistances;
		}

		TEST_METHOD(TestQueue_RemoveMinimum_EmptyQueue_ThrowsError)
		{
			auto action = [] {
				int size = 1;
				float* minimumDistances = new float[size] { 1 };
				PriorityQueue queue = PriorityQueue(minimumDistances, size);

				queue.RemoveMinimum();
				Assert::AreEqual(0, queue.GetCurrentSize());

				queue.RemoveMinimum();
				delete[] minimumDistances;
			};

			Assert::ExpectException<std::out_of_range>(action);
		}

		#pragma endregion

		#pragma region GetMinimum

		TEST_METHOD(TestQueue_GetMinimum_EmptyQueue_ThrowsError)
		{
			auto action = [] {
				int size = 1;
				float* minimumDistances = new float[size] { 1 };
				PriorityQueue queue = PriorityQueue(minimumDistances, size);

				queue.RemoveMinimum();
				Assert::AreEqual(0, queue.GetCurrentSize());

				int index;
				float minimumDistance;
				queue.GetMinimum(&index, &minimumDistance);
				delete[] minimumDistances;

			};

			Assert::ExpectException<std::out_of_range>(action);
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
			delete[] minimumDistances;
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
			delete[] minimumDistances;
		}

		TEST_METHOD(TestQueue_UpdateMinimum_EmptyQueue_ThrowsError)
		{
			auto action = [] {

				int size = 1;
				float* minimumDistances = new float[size] { 1 };
				PriorityQueue queue = PriorityQueue(minimumDistances, size);

				queue.RemoveMinimum();
				Assert::AreEqual(0, queue.GetCurrentSize());

				queue.UpdateMinimum(0.25);
				delete[] minimumDistances;

			};

			Assert::ExpectException<std::out_of_range>(action);
		}

		#pragma endregion

		#pragma region Replace Element

		TEST_METHOD(TestQueue_ReplaceElement_ReplaceNonMinimalElementWithValueBiggerThanMinimum)
		{
			//Arrange
			int size = 10;
			float* minimumDistances = new float[size] { 100, 19, 36, 17, 3, 25, 1, 2, 7, -1, -1 };
			PriorityQueue queue = PriorityQueue(minimumDistances, size - 1);

			//Act
			minimumDistances[9] = 37.0f;
			queue.ReplaceElement(2, 9);

			//Assert
			Assert::AreEqual(37.0f, queue.GetMinimumDistances()[9]);
			Assert::AreEqual(9, queue.GetCurrentSize());

			bool containsNewElement = false;
			bool containsOldElement = false;
			int* indices = queue.GetIndices();
			for (int i = 0; i < 9; i++)
			{
				if (indices[i] == 9)
				{
					containsNewElement = true;
				}

				if (indices[i] == 2)
				{
					containsOldElement = true;
				}
			}

			Assert::IsTrue(containsNewElement, L"Queue does not contain new element");
			Assert::IsFalse(containsOldElement, L"Queue wrongly includes the old element");

			float currentMinimum;
			int currentMinimumIndex;
			queue.GetMinimum(&currentMinimumIndex, &currentMinimum);

			Assert::AreEqual(1.0f, currentMinimum);
			Assert::AreEqual(6, currentMinimumIndex);
			Assert::IsTrue(this->IsMinimumHeap(queue), L"Queue is not minimum heap");
		}

		TEST_METHOD(TestQueue_ReplaceElement_ReplaceNonMinimalElementWithNewMinimum)
		{
			//Arrange
			int size = 10;
			float* minimumDistances = new float[size] { 100, 19, 36, 17, 3, 25, 1, 2, 7, std::numeric_limits<float>().max()};
			PriorityQueue queue = PriorityQueue(minimumDistances, size - 1);

			//Act
			minimumDistances[9] = 0.0f;
			queue.ReplaceElement(2, 9);

			//Assert
			Assert::AreEqual(0.0f, queue.GetMinimumDistances()[9]);
			Assert::AreEqual(9, queue.GetCurrentSize());

			bool containsNewElement = false;
			bool containsOldElement = false;
			int* indices = queue.GetIndices();
			for (int i = 0; i < 9; i++)
			{
				if (indices[i] == 9)
				{
					containsNewElement = true;
				}

				if (indices[i] == 2)
				{
					containsOldElement = true;
				}
			}

			Assert::IsTrue(containsNewElement, L"Queue does not contain new element");
			Assert::IsFalse(containsOldElement, L"Queue wrongly includes the old element");

			float currentMinimum;
			int currentMinimumIndex;
			queue.GetMinimum(&currentMinimumIndex, &currentMinimum);

			Assert::AreEqual(0.0f, currentMinimum);
			Assert::AreEqual(9, currentMinimumIndex);
			Assert::IsTrue(this->IsMinimumHeap(queue), L"Queue is not minimum heap");
		}

		TEST_METHOD(TestQueue_ReplaceElement_ReplaceMinimumWithNewMinimum)
		{
			//Arrange
			int size = 10;
			float* minimumDistances = new float[size] { 100, 19, 36, 17, 3, 25, 1, 2, 7, -1 };
			PriorityQueue queue = PriorityQueue(minimumDistances, size - 1);

			//Act
			minimumDistances[9] = 1.5f;
			queue.ReplaceElement(6, 9);

			//Assert
			Assert::AreEqual(1.5f, queue.GetMinimumDistances()[9]);
			Assert::AreEqual(9, queue.GetCurrentSize());

			bool containsNewElement = false;
			bool containsOldElement = false;
			int* indices = queue.GetIndices();
			for (int i = 0; i < 9; i++)
			{
				if (indices[i] == 9)
				{
					containsNewElement = true;
				}

				if (indices[i] == 6)
				{
					containsOldElement = true;
				}
			}

			Assert::IsTrue(containsNewElement, L"Queue does not contain new element");
			Assert::IsFalse(containsOldElement, L"Queue wrongly includes the old element");

			float currentMinimum;
			int currentMinimumIndex;
			queue.GetMinimum(&currentMinimumIndex, &currentMinimum);

			Assert::AreEqual(1.5f, currentMinimum);
			Assert::AreEqual(9, currentMinimumIndex);
			Assert::IsTrue(this->IsMinimumHeap(queue), L"Queue is not minimum heap");
		}

		TEST_METHOD(TestQueue_ReplaceElement_ReplaceMinimumSoItIsNoLongerMinimum)
		{
			//Arrange
			int size = 10;
			float* minimumDistances = new float[size] { 100, 19, 36, 17, 3, 25, 1, 2, 7, -1 };
			PriorityQueue queue = PriorityQueue(minimumDistances, size - 1);

			//Act
			minimumDistances[9] = 2.5;
			queue.ReplaceElement(6, 9);

			//Assert
			Assert::AreEqual(2.5f, queue.GetMinimumDistances()[9]);
			Assert::AreEqual(9, queue.GetCurrentSize());

			bool containsNewElement = false;
			bool containsOldElement = false;
			int* indices = queue.GetIndices();
			for (int i = 0; i < 9; i++)
			{
				if (indices[i] == 9)
				{
					containsNewElement = true;
				}

				if (indices[i] == 6)
				{
					containsOldElement = true;
				}
			}

			Assert::IsTrue(containsNewElement, L"Queue does not contain new element");
			Assert::IsFalse(containsOldElement, L"Queue wrongly includes the old element");

			float currentMinimum;
			int currentMinimumIndex;
			queue.GetMinimum(&currentMinimumIndex, &currentMinimum);

			Assert::AreEqual(2.0f, currentMinimum);
			Assert::AreEqual(7, currentMinimumIndex);
			Assert::IsTrue(this->IsMinimumHeap(queue), L"Queue is not minimum heap");
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
