#include <vector>
#include "Queue.h"

Queue::Queue() {

}

void Queue::AddToArray1(int value) {
    this->array1.push_back(value);
}

void Queue::AddToArray2(int value) {
    this->array2.push_back(value);
}

int Queue::GetValueAtLocation(int index) {
    if (index >= 0 && index < this->array1.size()) {
        return array1[index];
    } else {
        // Handle out-of-bounds index
        // You can throw an exception or return a default value
        // For example, throw std::out_of_range("Invalid index");
        // or return a default value like 0
        return 0;
    }
}