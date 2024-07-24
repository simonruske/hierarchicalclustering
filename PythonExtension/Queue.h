#include <vector>

class Queue {
public:
    std::vector<int> array1;
    std::vector<int> array2;

    Queue();
    void AddToArray1(int value);
    void AddToArray2(int value);
    int GetValueAtLocation(int index);
};