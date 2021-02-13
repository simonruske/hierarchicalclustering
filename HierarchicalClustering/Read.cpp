#include "read.h"
#include <fstream>
#include <iostream>
#include <sstream>

bool TryGetArraySize(std::string filename, int* numberOfRows, int* numberOfColumns)
{
    std::ifstream inputFile(filename);

    if (inputFile.is_open())
    {
        return TryGetArraySize(&inputFile, numberOfRows, numberOfColumns);
    }
    else
    {
        std::cerr << "Could not read file" << std::endl;
        return false;
    }
}

bool TryGetArraySize(std::ifstream *stream, int* numberOfRows, int* numberOfColumns)
{
    int currentNumberOfColumns;
    *numberOfRows = 0;
    *numberOfColumns = 0;

    std::string line;
    while (std::getline(*stream, line))
    {
        currentNumberOfColumns = 0;
        (*numberOfRows)++;
        
        std::stringstream ss(line);
        std::string item;
        
        while (std::getline(ss, item, ','))
        {
            currentNumberOfColumns++;
        }
        if (*numberOfColumns == 0)
        {
            *numberOfColumns = currentNumberOfColumns;
        }

        else if (*numberOfColumns != currentNumberOfColumns)
        {
            return false;
        }
    }

    return true;
}

template<typename T>
bool TryGetArrayFromFile(std::string filename, int numberOfRows, int numberOfColumns, T* arr)
{
    std::ifstream inputFile(filename);

    if (inputFile.is_open())
    {
        ReadArrayFromFile<T>(&inputFile, numberOfRows, numberOfColumns, arr);
        return true;
    }
    else
    {
        std::cerr << "Could not read file" << std::endl;
        return false;
    }
}

template <typename T>
void ReadArrayFromFile(std::ifstream* stream, int numberOfRows, int numberOfColumns, T* arr)
{
    std::string line;

    int currentLocation;
    for (int i = 0; i < numberOfRows; i++)
    {
        std::getline(*stream, line);
        std::stringstream ss(line);

        for (int j = 0; j < numberOfColumns; j++)
        {
            std::string token;
            std::getline(ss, token, ',');
            currentLocation = numberOfColumns * i + j;
            arr[currentLocation] = (T)std::strtod(token.c_str(), NULL);
        }
    }
}

template bool TryGetArrayFromFile<double>(std::string filename, int numberOfRows, int numberOfColumns, double* arr);
template bool TryGetArrayFromFile<float>(std::string filename, int numberOfRows, int numberOfColumns, float* arr);





