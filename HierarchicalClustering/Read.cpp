#include "read.h"
#include <fstream>
#include <iostream>
#include <sstream>

bool TryGetArraySize(char* filename, int* numberOfRows, int* numberOfColumns)
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

bool TryGetArrayFromFile(char* filename, int numberOfRows, int numberOfColumns, float* arr)
{
    std::ifstream inputFile(filename);

    if (inputFile.is_open())
    {
        ReadArrayFromFile(&inputFile, numberOfRows, numberOfColumns, arr);
        return true;
    }
    else
    {
        std::cerr << "Could not read file" << std::endl;
        return false;
    }
}

void ReadArrayFromFile(std::ifstream* stream, int numberOfRows, int numberOfColumns, float* arr)
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
            arr[currentLocation] = std::strtof(token.c_str(), NULL);
        }
    }
}






