#pragma once
#include <fstream>

bool TryGetArraySize(char* filename, int* numberOfRows, int* numberOfColumns);
bool TryGetArraySize(std::ifstream* stream, int* numberOfRows, int* numberOfColumns);
bool TryGetArrayFromFile(char* filename, int numberOfRows, int numberOfColumns, float* arr);
void ReadArrayFromFile(std::ifstream* stream, int numberOfRows, int numberOfColumns, float* arr);