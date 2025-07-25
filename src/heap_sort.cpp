#include <iostream>
#include <iomanip>
#include <vector>

#include "vectors.hpp"

void makeHeap(IntVec& a, size_t limit, size_t index) {
    size_t leftIndex = index * 2 + 1;
    size_t rightIndex = index * 2 + 2;

    size_t largestIndex = index;
    if (leftIndex < limit && a[leftIndex] > a[largestIndex]) {
        largestIndex = leftIndex;
    }
    if (rightIndex < limit && a[rightIndex] > a[largestIndex]) {
        largestIndex = rightIndex;
    }

    if (largestIndex == index) {
        return;
    }

    int tmp = a[index];
    a[index] = a[largestIndex];
    a[largestIndex] = tmp;
    makeHeap(a, limit, largestIndex);
}


int main() {
    IntVec a = randomVector(20);
    printVector(a);
    std::cout << std::endl;
    std::cout << "make heap" << std::endl;
    for (int i = a.size() / 2 - 1; i >= 0; --i) {
        makeHeap(a, a.size(), i);
        printVector(a);
    }
    std::cout << std::endl;
    std::cout << "sort" << std::endl;
    for (int i = a.size() - 1; i >= 1; --i) {
        int tmp = a[0];
        a[0] = a[i];
        a[i] = tmp;
        makeHeap(a, i, 0);
        printVector(a);
    }
    return 0;
}
