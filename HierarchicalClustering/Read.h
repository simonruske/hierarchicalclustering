#pragma once
#include <fstream>

bool TryGetArraySize(std::string filename, int* numberOfRows, int* numberOfColumns);
bool TryGetArraySize(std::ifstream* stream, int* numberOfRows, int* numberOfColumns);
bool TryGetArrayFromFile(std::string filename, int numberOfRows, int numberOfColumns, float* arr);
void ReadArrayFromFile(std::ifstream* stream, int numberOfRows, int numberOfColumns, float* arr);