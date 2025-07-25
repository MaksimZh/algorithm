#include <iostream>
#include <iomanip>
#include <vector>

#include "vectors.hpp"

void swap(int& x, int& y) {
    int tmp = x;
    x = y;
    y = tmp;
}

size_t partition(IntVec& a, size_t lo, size_t up) {
    int pivot = a[up - 1];
    size_t i = lo;

    for (size_t j = lo; j < up - 1; ++j) {
        if (a[j] <= pivot) {
            swap(a[i], a[j]);
            ++i;
        }
    }
    swap(a[i], a[up - 1]);
    return i;
}

void quickSort(IntVec& a, size_t lo, size_t up) {
    if (up - lo < 2) {
        return;
    }

    size_t p = partition(a, lo, up);
    quickSort(a, lo, p);
    quickSort(a, p, up);
}

int main() {
    IntVec a = randomVector(20);
    printVector(a);
    quickSort(a, 0, a.size());
    printVector(a);
    return 0;
}
