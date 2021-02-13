#include "pch.h"
#include "CppUnitTest.h"
#include "../HierarchicalClustering/Read.h"
#include "../HierarchicalClustering/Read.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
namespace TestHierarchicalClustering
{
	TEST_CLASS(TestRead)
	{
	public:

		TEST_METHOD(TestRead_ReadFile_ExampleFile)
		{

			std::string filename = SOLUTION_DIRECTORY"TestFiles\\data.csv";

			int numberOfRows;
			int numberOfColumns;

			Assert::IsTrue(TryGetArraySize(filename, &numberOfRows, &numberOfColumns));
			Assert::AreEqual(5, numberOfRows);
			Assert::AreEqual(5, numberOfColumns);

			float* arr = new float[25];

			Assert::IsTrue(TryGetArrayFromFile(filename, numberOfRows, numberOfColumns, arr));

			Assert::AreEqual(0.667f, arr[12]);
			Assert::AreEqual(0.56f, arr[18]);
		}

	private:
	
		char* PrintFullPath(char* partialPath)
		{
			char full[_MAX_PATH];
			if (_fullpath(full, partialPath, _MAX_PATH) != NULL)
				return full;
			else
				return "Invalid path\n";;
		}
	};
}