#pragma once
#include <fstream>

bool TryGetArraySize(std::string filename, int* numberOfRows, int* numberOfColumns);
bool TryGetArraySize(std::ifstream* stream, int* numberOfRows, int* numberOfColumns);

template <typename T> bool TryGetArrayFromFile(std::string filename, int numberOfRows, int numberOfColumns, T* arr);
template <typename T> void ReadArrayFromFile(std::ifstream* stream, int numberOfRows, int numberOfColumns, T* arr);
