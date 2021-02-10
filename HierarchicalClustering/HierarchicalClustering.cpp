#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <ctime>
#include <cmath>
#include <cstdlib>
#include "GenericLinkage.h"
#include "Queue.h"
#include "Initialise.h"
#include "Read.h"
#include "Distance.h"

int main()
{
    // Reads in the file
    std::cout << "Enter file name:" << std::endl;
	std::string filename;
    std::cin >> filename;

    int m, n;
    if (!TryGetArraySize(filename, &m, &n))
    {
        return 0;
    }

    int vecSize = m * (2 * n + 1);
    float* vec = new float[vecSize];
    if(!TryGetArrayFromFile(filename, m, n, vec))
    {
        return 0;
    }

    GenericLinkageStatus status = GenericLinkage(vec, m, n);
    status.PrintLinkage();
	delete[] vec;
}
